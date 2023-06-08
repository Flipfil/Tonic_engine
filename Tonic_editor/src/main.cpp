#include "tonic/engine.h"
#include "tonic/app.h"
#include "tonic/log.h"

#include "tonic/input/keyboard.h"
#include "tonic/input/mouse.h"
#include "tonic/input/controller.h"

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/frame_buffer.h"
#include "tonic/graphics/texture.h"
#include "tonic/graphics/vertex.h"

#include "tonic/assets/assets.h"

#include "tonic/main.h"

#include "external/imgui/imgui.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include <iostream>


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
    ~Object() {}

    void Rotate(const glm::vec3& by) { m_rotation += by; }
    void Move(const glm::vec3& by)   { m_position += by; }
    void Scale(const glm::vec3& by)  { m_size     += by; }

    void SetRotationMomentum(const glm::vec3& rm) { m_rotation_momentum = rm; }
    void SetDrag(float drag) { m_drag = drag; }

    void SetTexture(std::shared_ptr<graphics::Texture> texture) { m_texture = texture; }

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
        
        m_rotation += m_rotation_momentum;
        m_rotation_momentum *= m_drag;
        if (glm::length(m_rotation_momentum) < 0.01f)
            m_rotation_momentum = glm::vec3(0.f);

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, m_position);
        model = glm::rotate(model, m_rotation.x, { 1,0,0 });
        model = glm::rotate(model, m_rotation.y, { 0,1,0 });
        model = glm::rotate(model, m_rotation.z, { 0,0,1 });
        model = glm::scale(model, m_size);

        m_shader->SetUniformMat4("model", model);
    }

    virtual void ImguiRender()
    {
        if (ImGui::Begin(m_name.c_str()))
        {
            ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.01f);
            ImGui::DragFloat3("Size", glm::value_ptr(m_size), 0.01f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(m_rotation), 0.01f);
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
    std::string m_name;

    std::shared_ptr<graphics::VertexArray> m_VA;
    std::shared_ptr<graphics::Shader> m_shader;
    std::shared_ptr<graphics::Texture> m_texture;

    glm::vec3 m_position;
    glm::vec3 m_size;
    glm::vec3 m_rotation;
    glm::vec3 m_rotation_momentum;

    float m_drag;

    State m_state;
};

class Editor : public App
{
private:
    
    std::unordered_map<std::string, Object> m_objects;

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
        TONIC_TRACE("Editor: Initialize()");

        float texcoords[] = {
            1.f, 1.f,
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f
        };
        // Test shader
        const char* vertex_shader = R"(
                #version 410 core

                layout (location = 0) in vec3 position;
                layout (location = 1) in vec3 color;
                layout (location = 2) in vec2 tex_coords;

                out vec4 vpos;
                out vec3 col;
                out vec2 uvs;

                uniform mat4 model = mat4(1.0);

                void main()
                {
                    col = color;
                    uvs = tex_coords;
                    vpos        = model * vec4(position, 1.0);
                    gl_Position = model * vec4(position, 1.0);
                }
            )";

        const char* fragment_shader = R"(
                #version 410 core

                in vec4 vpos;
                in vec3 col;
                in vec2 uvs; 

                out vec4 out_color; 

                uniform sampler2D tex;

                void main()
                {
                    // out_color = vec4(0.5 + (vpos.x + vpos.y - vpos.z)/3.0, 0.5 + (vpos.x + vpos.y - vpos.z)/3.0, 0.0,1.0);
                    out_color = texture(tex, uvs) + vec4(col, 1.0);
                }
            )";

        // Engine::GetInstance().GetRenderManager().SetWireframeMode(true);
        std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vertex_shader, fragment_shader);
        
        TONIC_CREATE_VERTEX_BUFFER(vb_v, float);
        vb_v->PushVertex({ -0.5f,  0.5f, 0.f, 1.f, 1.f, 1.f });
        vb_v->PushVertex({ -0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f });
        vb_v->PushVertex({  0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f });
        vb_v->PushVertex({  0.5f,  0.5f, 0.f, 0.f, 1.f, 1.f });
        vb_v->SetLayout({ 3, 3 });

        TONIC_CREATE_VERTEX_BUFFER(vb_t, short);
        vb_t->PushVertex({0,1});
        vb_t->PushVertex({0,0});
        vb_t->PushVertex({1,0});
        vb_t->PushVertex({1,1});
        vb_t->SetLayout({ 2 });


        std::shared_ptr<graphics::VertexArray> VA = std::make_shared<graphics::VertexArray>();
        VA->PushBuffer(std::move(vb_v));
        VA->PushBuffer(std::move(vb_t));
        VA->SetElements({ 0,3,1,1,3,2 });
        VA->Upload();
        
        m_objects["Rectangle"] = Object("Rectangle", VA, shader);
        m_objects["Rectangle"].SetTexture(std::make_shared<graphics::Texture>("post_build_copy/res/ammo_crate.png"));
        /*
        m_objects["Hexahedron"] = Object("Hexahedron", std::make_shared<graphics::Mesh>(
            assets::mesh_cells::hexahedron::vertices,
            assets::mesh_cells::hexahedron::vertex_count, 3,
            assets::mesh_cells::hexahedron::elements,
            assets::mesh_cells::hexahedron::element_count), shader);
        m_objects["Hexahedron"].SetState(Object::State::Active);

        m_objects["Tetrahedron"] = Object("Tetrahedron", std::make_shared<graphics::Mesh>(
            assets::mesh_cells::tetrahedron::vertices,
            assets::mesh_cells::tetrahedron::vertex_count, 3,
            assets::mesh_cells::tetrahedron::elements,
            assets::mesh_cells::tetrahedron::element_count), shader);

        m_objects["Pyramid"] = Object("Pyramid", std::make_shared<graphics::Mesh>(
            assets::mesh_cells::pyramid::vertices,
            assets::mesh_cells::pyramid::vertex_count, 3,
            assets::mesh_cells::pyramid::elements,
            assets::mesh_cells::pyramid::element_count), shader);

        m_objects["Triangular prism"] = Object("Triangular prism", std::make_shared<graphics::Mesh>(
            assets::mesh_cells::triangular_prism::vertices,
            assets::mesh_cells::triangular_prism::vertex_count, 3,
            assets::mesh_cells::triangular_prism::elements,
            assets::mesh_cells::triangular_prism::element_count), shader);

        m_objects["Triangle"] = Object("Triangle", std::make_shared<graphics::Mesh>(
            assets::mesh_cells::triangle::vertices,
            assets::mesh_cells::triangle::vertex_count, 3,
            assets::mesh_cells::triangle::elements,
            assets::mesh_cells::triangle::element_count), shader);
            */

        // Texture
        // m_texture = std::make_shared<graphics::Texture>("post_build_copy/res/ammo_crate.png");
    }

    void Shutdown() override
    {
        TONIC_TRACE("Editor: Shutdown()");
    }

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

    void Render() override
    {
        for (auto& [name, obj] : m_objects)
            obj.Render();
 
        Engine::GetInstance().GetRenderManager().Flush();
    }

    void ImguiRender() override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        for (auto& [name, obj] : m_objects)
            obj.ImguiRender();
        
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
    }

private:
};

App* CreateApp()
{
    return new Editor();
}