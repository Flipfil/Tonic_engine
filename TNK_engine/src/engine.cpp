#include "engine.h"

#include "tnk_assert.h"

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

        while (m_is_running)
        {
            m_window.PumpEvents();
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

        // Managers
        if (succeeded) {
            m_log_manager.Initialize();
            GetInfo();
        }

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
           
        return m_is_initialized = m_is_running = true;
    }

    //============================================
    void Engine::Shutdown() // destroy everything
    {
        m_window.Shutdown(); // close window

        //managers - in reverse order of initialization
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