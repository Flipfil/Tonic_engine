#include "tonic/engine.h"
#include "tonic/app.h"
#include "tonic/log.h"

#include "tonic/core/asset_library.h"

#include "tonic/input/keyboard.h"
#include "tonic/input/mouse.h"
#include "tonic/input/controller.h"

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/graphics/texture.h"
#include "tonic/graphics/vertex.h"
#include "tonic/graphics/camera.h"

#include "tonic/assets/assets.h"

#include "tonic/main.h"

#include "external/imgui/imgui.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include <iostream>

#include "object_manip/rect_creator.h"


using namespace tonic;

class Object
{
public:
    enum class State
    {
        Active,
        NotActive
    };

    Object()
        : m_name("")
        , m_VA(nullptr)
        , m_shader(nullptr)
        , m_texture(nullptr)
        , m_position(glm::vec3(0.f))
        , m_size(glm::vec3(1.f))
        , m_rotation(glm::vec3(0.f))
        , m_rotation_momentum(glm::vec3(0.f))
        , m_model(1.f)
        , m_drag(0.95f)
        , m_state(State::NotActive)
    {
    }

    Object(const std::string& name, std::shared_ptr<graphics::VertexArray> vertex_array, std::shared_ptr<graphics::Shader> shader)
        : Object()
    {
        m_name = name;
        m_VA = vertex_array;
        m_shader = shader;
    }
    Object(const std::string& name, std::shared_ptr<graphics::VertexArray> vertex_array, std::shared_ptr<graphics::Shader> shader, std::shared_ptr<graphics::Texture> texture)
        : Object(name, vertex_array, shader)
    {
        m_texture = texture;
    }
    ~Object() {}

    void Rotate(const glm::vec3& by) { if (by == glm::vec3(0.f)) return; m_rotation += by; CalculateModelMatrix(); }
    void Move(const glm::vec3& by)   { if (by == glm::vec3(0.f)) return; m_position += by; CalculateModelMatrix(); }
    void Scale(const glm::vec3& by)  { if (by == glm::vec3(0.f)) return; m_size     += by; CalculateModelMatrix(); }

    void SetRotationMomentum(const glm::vec3& rm) { m_rotation_momentum = rm; }
    void SetDrag(float drag) { m_drag = drag; }

    void SetVertexArray(std::shared_ptr<graphics::VertexArray> VA) { m_VA = VA; }
    void SetShader(std::shared_ptr<graphics::Shader> shader)       { m_shader = shader; }
    void SetTexture(std::shared_ptr<graphics::Texture> texture)    { m_texture = texture; }

    const glm::vec3& Position() { return m_position; }
    const glm::vec3& Size()     { return m_size; }
    const glm::vec3& Rotation() { return m_rotation; }
    const float      Drag()     { return m_drag; }

    void SetState(State state) { m_state = state; }
    State GetState() { return m_state; }

    virtual void Render()
    {
        if (m_state != State::Active)
            return;

        m_shader->SetUniformMat4("model", m_model);
        Engine::GetInstance().GetRenderManager().Submit(std::move(
            m_texture 
                ? std::make_unique<graphics::RENDER_COMMANDS::RenderVertexArrayTextured>(m_VA, m_texture, m_shader)
                : std::make_unique<graphics::RENDER_COMMANDS::RenderVertexArray>(m_VA, m_shader)
        ));
    }

    virtual void Update()
    {
        if (m_state != State::Active)
            return;
        
        if (m_rotation_momentum == glm::vec3(0.f))
            return;

        m_rotation += m_rotation_momentum;
        m_rotation_momentum *= m_drag;
        if (glm::length(m_rotation_momentum) < 0.01f)
            m_rotation_momentum = glm::vec3(0.f);

        CalculateModelMatrix();
    }

    virtual void ImguiRender()
    {
        if (ImGui::Begin(m_name.c_str()))
        {
            glm::vec3 cur_pos = m_position;
            glm::vec3 cur_size = m_size;
            glm::vec3 cur_rotation = m_rotation;

            ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.01f);
            ImGui::DragFloat3("Size", glm::value_ptr(m_size), 0.01f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(m_rotation), 0.01f);

            if (cur_pos != m_position || cur_rotation != m_rotation || cur_size != m_size)
                CalculateModelMatrix();

            ImGui::DragFloat("Drag", &m_drag, 0.001f);
            if (m_state == State::Active)
            {
                if (ImGui::Button("Deactivate"))
                    m_state = State::NotActive;
            }
            else
            {
                if (ImGui::Button("Activate"))
                    m_state = State::Active;
            }
        } 
        ImGui::End();
    }

private:
    void CalculateModelMatrix()
    {
        m_model = glm::mat4(1.f);
        m_model = glm::translate(m_model, m_position);
        m_model = glm::rotate   (m_model, m_rotation.x, { 1,0,0 });
        m_model = glm::rotate   (m_model, m_rotation.y, { 0,1,0 });
        m_model = glm::rotate   (m_model, m_rotation.z, { 0,0,1 });
        m_model = glm::scale    (m_model, m_size);
    }

private:
    std::string m_name;

    std::shared_ptr<graphics::VertexArray> m_VA;
    std::shared_ptr<graphics::Shader> m_shader;
    std::shared_ptr<graphics::Texture> m_texture;

    glm::vec3 m_position;
    glm::vec3 m_size;
    glm::vec3 m_rotation;
    glm::vec3 m_rotation_momentum;
    glm::mat4 m_model;

    float m_drag;

    State m_state;
};

class Editor : public App
{
private:
    
    std::unordered_map<std::string, Object>   m_objects;
    std::shared_ptr<graphics::Camera>         m_camera;
    core::AssetLibrary<graphics::VertexArray> m_VA_lib;
    core::AssetLibrary<graphics::Shader>      m_shader_lib;
    core::AssetLibrary<graphics::Texture>     m_texture_lib;
    RectCreator m_rect_creator;

public:

    // ---------------------------------------------
    core::WindowProperties GetWindowProperties()
    {
        core::WindowProperties props;
        props.width = 800;
        props.height = 600;
        props.title = "TonicEditor";
        props.imgui_props.IsDockingEnabled = true;
        return props;
    }

    // ---------------------------------------------
    void Initialize() override
    {
        InitAssetLibs();
        
        m_camera = std::make_shared<graphics::Camera>();
        m_camera->SetHeight(2.0f);

        m_objects["Ammo crate"] = Object("Ammo crate", m_VA_lib.Get("TexturedRect"), m_shader_lib.Get("TexturedRect"), m_texture_lib.Get("Ammo crate"));
        m_objects["Invader"] = Object("Invader", m_VA_lib.Get("TexturedRect"), m_shader_lib.Get("TexturedRect"), m_texture_lib.Get("Invader"));
        m_objects["Rect"] = Object("Rect", m_VA_lib.Get("Rect"), m_shader_lib.Get("Rect"));

        m_rect_creator.SetShader(m_shader_lib.Get("Rect"));
        m_rect_creator.SetVA(m_VA_lib.Get("Rect"));
        m_rect_creator.SetFrameBuffer();
    }

    // ---------------------------------------------
    void Shutdown() override
    {
        TONIC_TRACE("Editor: Shutdown()");
    }

    // ---------------------------------------------
    void Update() override
    {
        auto window_size = Engine::GetInstance().GetWindow().GetSize();

        float x_norm = (float)input::Mouse::X() / window_size.x;
        float y_norm = (float)(window_size.y - input::Mouse::Y()) / window_size.y;

        for (auto& [name, obj] : m_objects)
        {
            if (obj.GetState() == Object::State::Active)
            {
                if (input::Mouse::HoldingButton(TONIC_M_BUTTON_LEFT))
                    obj.SetRotationMomentum(glm::vec3(-input::Mouse::YDelta(), -input::Mouse::XDelta(), 0.f) / 500.f);

            }
            obj.Update();
        }
    }

    // ---------------------------------------------
    void Render() override
    {
        Engine::GetInstance().GetRenderManager().Submit(TONIC_SUBMIT_RENDER_CMD(PushCamera, m_camera));

        for (auto& [name, obj] : m_objects)
            obj.Render();

        m_rect_creator.Render();
    
        Engine::GetInstance().GetRenderManager().Submit(TONIC_SUBMIT_RENDER_CMD(PopCamera));
    }

    // ---------------------------------------------
    void ImguiRender() override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        for (auto& [name, obj] : m_objects)
            obj.ImguiRender();
        
        m_rect_creator.ImguiRender();

        if (ImGui::Begin("Game view"))
        {
            if (ImGui::IsWindowHovered())
            {
                ImGui::CaptureMouseFromApp(false);
            }

            ImVec2 size = { 500,400 };
            ImVec2 uv0 = { 0,1 };
            ImVec2 uv1 = { 1,0 };
            ImGui::Image((void*)(intptr_t)Engine::GetInstance().GetWindow().GetFrameBuffer()->GetTextureID(), size, uv0, uv1);
        }
        ImGui::End();

        if (ImGui::Begin("Asset library view"))
        {
            ImVec4 data_col(0, 1, 0, 1);
            ImVec4 error_col(1, 0, 0, 1);
            if (ImGui::TreeNode("Textures"))
            {
                for (const auto& [k, v] : m_texture_lib.GetAll())
                {
                    if (ImGui::TreeNode(k.c_str()))
                    {
                        graphics::Texture* tex = v.get();
                        if (tex)
                        {
                            ImGui::TextColored(data_col, "Use count: "); ImGui::SameLine();
                            ImGui::Text("%03d", (int)v.use_count());
                            ImGui::TextColored(data_col, "Size: "); ImGui::SameLine();
                            ImGui::Text("%dx%d", tex->GetWidth(), tex->GetHeight());
                            ImGui::TextColored(data_col, "Channels: "); ImGui::SameLine();
                            ImGui::Text("%d", tex->GetNumChannels());
                            ImGui::TextColored(data_col, "File: "); ImGui::SameLine();
                            ImGui::Text("%s", tex->GetFileName().c_str());
                            ImVec2 size{ (float)tex->GetWidth(), (float)tex->GetHeight() };
                            ImGui::Image((void*)(intptr_t)tex->GetID(), size, { 0,1 }, {1, 0});
                        }
                        else
                        {
                            ImGui::TextColored(error_col, "Invalid texture: %s", k.c_str());
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

private:

    // ---------------------------------------------
    void InitAssetLibs()
    {
    /* vertex arrays */
        // textured rect
        {
            std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
            
            TONIC_CREATE_VERTEX_BUFFER(vb_v, float);
            vb_v->PushVertex({ -0.5f,  0.5f, 0.f });
            vb_v->PushVertex({ -0.5f, -0.5f, 0.f });
            vb_v->PushVertex({  0.5f, -0.5f, 0.f });
            vb_v->PushVertex({  0.5f,  0.5f, 0.f });
            vb_v->SetLayout({ 3 });

            TONIC_CREATE_VERTEX_BUFFER(vb_t, short);
            vb_t->PushVertex({ 0,1 });
            vb_t->PushVertex({ 0,0 });
            vb_t->PushVertex({ 1,0 });
            vb_t->PushVertex({ 1,1 });
            vb_t->SetLayout({ 2 });


            VA->PushBuffer(std::move(vb_v));
            VA->PushBuffer(std::move(vb_t));
            VA->SetElements({ 0,3,1,1,3,2 });
            VA->Upload();

            m_VA_lib.Load("TexturedRect", VA);
        }
        // rect
        {
            std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();

            TONIC_CREATE_VERTEX_BUFFER(vb_v, float);
            vb_v->PushVertex({ -0.5f,  0.5f, 0.f });
            vb_v->PushVertex({ -0.5f, -0.5f, 0.f });
            vb_v->PushVertex({ 0.5f, -0.5f, 0.f });
            vb_v->PushVertex({ 0.5f,  0.5f, 0.f });
            vb_v->SetLayout({ 3 });
            VA->PushBuffer(std::move(vb_v));

            VA->SetElements({ 0,3,1,1,3,2 });
            VA->Upload();

            m_VA_lib.Load("Rect", VA);
        }

    /* shaders */
        // rect
        {
            const char* vertex_shader = R"(
                #version 410
                layout (location = 0) in vec3 position;
                
                uniform mat4 proj = mat4(1.0);
                uniform mat4 model = mat4(1.0);
                
                void main()
                {
                    gl_Position = proj * model * vec4(position, 1.0);
                }
            )";
            const char* fragment_shader = R"(
                #version 410
                out vec4 out_color;
                
                uniform vec4 col = vec4(1.0);
                
                void main()
                {
                    out_color = col;
                }
            )";
            m_shader_lib.Load("Rect", std::make_shared<graphics::Shader>(vertex_shader, fragment_shader));
        }
        // textured rect
        {
            const char* vertex_shader = R"(
                #version 410
                
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec2 tex_coords;

                out vec2 uvs;
                
                uniform mat4 proj = mat4(1.0);
                uniform mat4 model = mat4(1.0);

                void main()
                {
                    uvs = tex_coords;
                    gl_Position = proj * model * vec4(position, 1.0);
                }
            )";
            const char* fragment_shader = R"(
                #version 410
                
                in vec2 uvs;

                out vec4 out_color;
                
                uniform sampler2D tex;
                void main()
                {
                    out_color = texture(tex, uvs);
                }
            )";
            m_shader_lib.Load("TexturedRect", std::make_shared<graphics::Shader>(vertex_shader, fragment_shader));
        }

    /* Textures */
        // ammo crate
        {
            std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("post_build_copy/res/ammo_crate.png");
            texture->SetTextureFilter(graphics::TextureFilter::Linear);
            m_texture_lib.Load("Ammo crate", texture);
        }
        // invader
        {
            std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("post_build_copy/res/invader.png");
            texture->SetTextureFilter(graphics::TextureFilter::Linear);
            m_texture_lib.Load("Invader", texture);
        }
    }
};

App* CreateApp()
{
    return new Editor();
}