#include "engine.h"
#include "tnk_assert.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"

#include "SDL.h"

namespace TNK
{
    // public
    //============================================
    Engine& Engine::GetInstance() // object access
    {
        if (!m_instance) m_instance = new Engine();

        return *m_instance;
    }

    //============================================
    void Engine::Run() // runs the engine
    {
        if (!Initialize())
            return;

        // Test mesh
        float vertices[]
        {
              0.5f,  0.5f,  0.f,  // up-right
              0.5f, -0.5f,  0.f,  // down-right
             -0.5f, -0.5f,  0.f,  // down-left
             -0.5f,  0.5f,  0.f   // up-left
        };

        uint32_t elements[]
        {
            0,3,1,
            1,3,2
        };

        std::shared_ptr<GRAPHICS::Mesh> mesh = std::make_shared<GRAPHICS::Mesh>(vertices, 4, 3, &elements[0],6);

        // Test shader
        const char* vertex_shader = R"(
            #version 410 core
            layout (location = 0) in vec3 position;
            out vec3 vpos;
            void main()
            {
                vpos = position;
                gl_Position = vec4(position, 1.0);
            }
        )";

        const char* fragment_shader = R"(
            #version 410 core
            in vec3 vpos;
            out vec4 out_color; 

            void main()
            {
                out_color = vec4(vpos + vec3(0.5,0.5,0), 1.0);
            }
        )";
        std::shared_ptr<GRAPHICS::Shader> shader = std::make_shared<GRAPHICS::Shader>(vertex_shader, fragment_shader);

        m_render_manager.SetWireframeMode(true);

        while (m_is_running)
        {
            m_window.PumpEvents();
            m_window.BeginRender();

            auto rc = std::make_unique<GRAPHICS::RENDER_COMMANDS::RenderMesh>(mesh, shader);
            m_render_manager.Submit(std::move(rc));
            m_render_manager.Flush();

            m_window.EndRender();
        }

        Shutdown();
    }

    // private
    //============================================
    bool Engine::Initialize() // initialize everything
    {
        bool succeeded = true;

        // assert initialization once
        TNK_ASSERT(!m_is_initialized, "Called Engine::Initialize() more than once");
        if (m_is_initialized)
            succeeded = false;
        
        // SDL
        if (succeeded && SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            TNK_ERROR("Error initializing SDL2: {}", SDL_GetError());
            succeeded = false;
        }
        else
        {
            SDL_version v;
            SDL_VERSION(&v);
            TNK_INFO("SDL2 v{}.{}.{}", v.major, v.minor, v.patch);
        }

        // Window
        if (succeeded && !m_window.Create())
            succeeded = false;
       

        if (!succeeded)
        {
            TNK_ERROR("Engine initialization failed. Shitting down.");
            Shutdown();
            return false;
        }
        
        // Managers
        m_log_manager.Initialize();
        GetInfo();
        m_render_manager.Initialize();

        return m_is_initialized = m_is_running = true;
    }

    //============================================
    void Engine::Shutdown() // destroy everything
    {
        m_window.Shutdown(); // close window

        //managers - in reverse order of initialization
        m_render_manager.Shutdown();
        m_log_manager.Shutdown();

        //flags
        m_is_initialized = m_is_running = false;

        SDL_Quit(); // deactivate SDL
    }

    //============================================
    void Engine::GetInfo()
    {
        TNK_TRACE("TNK_Engine v{}.{}", 0, 1);

        #ifdef TNK_CONFIG_DEBUG
            TNK_DEBUG("Configuration: DEBUG");
        #endif
        #ifdef TNK_CONFIG_RELEASE
            TNK_DEBUG("Configuration: RELEASE")
        #endif
        #ifdef TNK_PLATFORM_WINDOWS
            TNK_WARN("Platform: WINDOWS");
        #endif
        #ifdef TNK_PLATFORM_MAC
            TNK_WARN("Platform: MAC");
        #endif
        #ifdef TNK_PLATFORM_LINUX
            TNK_WARN("Platform: LINUX");
        #endif
    }

    // Singleton
    //============================================
    Engine* Engine::m_instance = nullptr;

    //============================================
    Engine::Engine() 
        : m_is_running(false)
        , m_is_initialized(false)
    {}
}