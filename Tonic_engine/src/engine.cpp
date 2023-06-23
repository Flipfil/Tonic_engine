#include "tonic/engine.h"
#include "tonic/log.h"
#include "tonic/app.h"

#include "tonic/core/time.h"

#include "tonic/input/mouse.h"
#include "tonic/input/keyboard.h"
#include "tonic/input/controller.h"

#include "tonic/graphics/shader.h"
#include "tonic/graphics/mesh.h"

#include "SDL.h"


namespace tonic
{
    // public
    //============================================
    Engine& Engine::GetInstance() // object access
    {
        if (!m_instance) m_instance = new Engine();

        return *m_instance;
    }

    //============================================
    void Engine::Run(App* app) // runs the engine
    {
        // Log
        m_log_manager.Initialize();

        TONIC_ASSERT(!m_app, "Attempting to call Engine::Run when a valid App already exists.");
        if (m_app) return;

        m_app = app;

        if (!Initialize())
            return;

        while (m_is_running == true)
        {
            Update();
            Render();
        }
        
        Shutdown();
    }

    // private
    //============================================
    bool Engine::Initialize() // initialize everything
    {
        // assert initialization once
        TONIC_ASSERT(!m_is_initialized, "Called tonic::Engine::Initialize() more than once");
        if (m_is_initialized) 
            return false;

        // SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            TONIC_ERROR("SDL initialization error: {}", SDL_GetError());
            return false;
        }
        else
        {
            SDL_version v;
            SDL_VERSION(&v);
            TONIC_INFO("SDL2 v{}.{}.{}", v.major, v.minor, v.patch);
        }

        // Core
        if (!m_window.Create(m_app->GetWindowProperties()))
        {
            TONIC_ERROR("Engine initialization failed. Shutting down.");
            Shutdown();
            return false;
        }
        core::Time::Initialize();
        
        // Input
        input::Mouse::Initialize();
        input::Keyboard::Initialize();
        
        m_app->Initialize();

        // Graphics
        m_render_manager.Initialize();

        GetInfo();
        return m_is_initialized = m_is_running = true;
    }

    //============================================
    void Engine::Shutdown() // destroy everything
    {
        // Flags
        m_is_initialized = m_is_running = false;

        // Client
        m_app->Shutdown();

        // Input
        input::Controller::Shutdown();

        //managers - in reverse order of initialization except log_manager
        m_render_manager.Shutdown();

        // Window
        m_window.Shutdown();

        // SDL
        SDL_Quit(); // deactivates SDL

        m_log_manager.Shutdown();
    }

    //============================================
    void Engine::GetInfo()
    {
        TONIC_TRACE("TONIC_Engine v{}.{}", 0, 1);

        #ifdef TONIC_CONFIG_DEBUG
            TONIC_DEBUG("Configuration: DEBUG");
        #endif
        #ifdef TONIC_CONFIG_RELEASE
            TONIC_DEBUG("Configuration: RELEASE");
        #endif
        #ifdef TONIC_PLATFORM_WINDOWS
            TONIC_WARN("Platform: WINDOWS");
        #endif
        #ifdef TONIC_PLATFORM_MAC
            TONIC_WARN("Platform: MAC");
        #endif
        #ifdef TONIC_PLATFORM_LINUX
            TONIC_WARN("Platform: LINUX");
        #endif
    }

    void Engine::Update()
    {
        core::Time::Update();
        m_window.PumpEvents();
        m_app->Update();
    }

    void Engine::Render()
    {
        m_window.BeginRender();
        m_app->Render();
        m_window.EndRender();
    }

    // Singleton
    //============================================
    Engine* Engine::m_instance = nullptr;

    //============================================
    Engine::Engine() 
        : m_is_running(false)
        , m_is_initialized(false)
        , m_app(nullptr)
    {}
}