#include "tonic/engine.h"
#include "tonic/app.h"
#include "tonic/log.h"

#include "tonic/input/keyboard.h"
#include "tonic/input/mouse.h"
#include "tonic/input/controller.h"

#include "tonic/graphics/mesh.h"
#include "tonic/graphics/shader.h"
#include "tonic/graphics/frame_buffer.h"

#include "tonic/main.h"

#include "external/imgui/imgui.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include <iostream>


using namespace tonic;

class Editor : public App
{
private:
    
    std::shared_ptr<graphics::Mesh>   m_mesh;
    std::shared_ptr<graphics::Mesh>   m_mesh2;

    std::shared_ptr<graphics::Shader> m_shader;

    float x_key_offset = 0.f;
    float y_key_offset = 0.f;
    float key_speed = 0.001f;

    glm::vec2 m_rect_pos, m_rect_size;
    

public:
    core::WindowProperties GetWindowProperties()
    {
        core::WindowProperties props;
        props.width = 800;
        props.height = 600;
        props.title = "TonicEditor";
        props.imgui_props.IsDockingEnabled = true;
        return props;
    }

    void Initialize() override
    {
        TONIC_TRACE("Editor: Initialize()");
        
        // Test mesh
        float vertices[]
        {
              0.5f,  0.5f,  0.f,  // up-right
             -0.5f,  0.5f,  0.f,  // down-right
             -0.5f, -0.5f,  0.f,  // down-left
              0.5f, -0.5f,  0.f   // up-left
        };

        uint32_t elements[]
        {
            0,3,1,
            1,3,2
        };

        m_mesh = std::make_shared<graphics::Mesh>(vertices, 4, 3, &elements[0], 6);

        // Test shader
        const char* vertex_shader = R"(
                #version 410 core
                layout (location = 0) in vec3 position;
                out vec3 vpos;
                uniform vec2 offset = vec2(0.5);
                uniform mat4 model = mat4(1.0);
                void main()
                {
                    vpos = position + vec3(offset, 0);
                    gl_Position = model * vec4(position, 1.0);
                }
            )";

        const char* fragment_shader = R"(
                #version 410 core
                in vec3 vpos;
                out vec4 out_color; 
                
                uniform vec3 color = vec3(0.0);
                uniform float blue = 0.5f;
                void main()
                {
                    out_color = vec4(vpos.xy, blue, 1.0);
                }
            )";
        m_shader = std::make_shared<graphics::Shader>(vertex_shader, fragment_shader);
        m_shader->SetUniformFloat3("color", 1, 0, 0);
        
        m_rect_pos = glm::vec2(0.f);
        m_rect_size = glm::vec2(1.f);
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

        if (input::Keyboard::KeyHeld(TONIC_KEY_LEFT))  x_key_offset -= key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_RIGHT)) x_key_offset += key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_UP))    y_key_offset += key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_DOWN))  y_key_offset -= key_speed;

        if (input::Keyboard::KeyPressed(TONIC_KEY_LEFT)) x_key_offset -= key_speed * 100;
        if (input::Keyboard::KeyPressed(TONIC_KEY_RIGHT)) x_key_offset += key_speed * 100;

        m_shader->SetUniformFloat2("offset", x_norm + x_key_offset, y_norm + y_key_offset);
       
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, {m_rect_pos.x, m_rect_pos.y, 0.f});
        model = glm::scale(model, { m_rect_size.x, m_rect_size.y, 0.f });

        m_shader->SetUniformMat4("model", model);
    }

    void Render() override
    {
        auto rc = std::make_unique<graphics::RENDER_COMMANDS::RenderMesh>(m_mesh, m_shader);
 
        Engine::GetInstance().GetRenderManager().Submit(std::move(rc));
 
        Engine::GetInstance().GetRenderManager().Flush();
    }

    void ImguiRender() override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::Begin("Rect Pos"))
        {
            ImGui::DragFloat2("Rect Pos",glm::value_ptr(m_rect_pos), 0.01f);
        } 
        ImGui::End();

        if (ImGui::Begin("Rect Size"))
        {
            ImGui::DragFloat2("Rect Size", glm::value_ptr(m_rect_size), 0.01f);
        }
        ImGui::End();

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