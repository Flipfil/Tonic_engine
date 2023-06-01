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
                void main()
                {
                    vpos = position + vec3(offset, 0);
                    gl_Position = vec4(position, 1.0);
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
        
    }

    void Shutdown() override
    {
        TONIC_TRACE("Editor: Shutdown()");
    }

    void Update() override
    {
        int window_w = 0;
        int window_h = 0;

        Engine::GetInstance().GetWindow().GetSize(window_w, window_h);

        float x_norm = (float)input::Mouse::X() / window_w;
        float y_norm = (float)(window_h - input::Mouse::Y()) / window_h;

        if (input::Keyboard::KeyHeld(TONIC_KEY_LEFT))  x_key_offset -= key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_RIGHT)) x_key_offset += key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_UP))    y_key_offset += key_speed;
        if (input::Keyboard::KeyHeld(TONIC_KEY_DOWN))  y_key_offset -= key_speed;

        if (input::Keyboard::KeyPressed(TONIC_KEY_LEFT)) x_key_offset -= key_speed * 100;
        if (input::Keyboard::KeyPressed(TONIC_KEY_RIGHT)) x_key_offset += key_speed * 100;

        m_shader->SetUniformFloat2("offset", x_norm + x_key_offset, y_norm + y_key_offset);
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

        if (ImGui::Begin("Rect Pos X"))
        {
            ImGui::DragFloat("Rect Pos X", &x_key_offset, 0.1f);
        } 
        ImGui::End();

        if (ImGui::Begin("Rect Pos Y"))
        {
            ImGui::DragFloat("Rect Pos Y", &y_key_offset, 0.1f);
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