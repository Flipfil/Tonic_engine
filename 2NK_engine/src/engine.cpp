#include "engine.h"
#include <iostream>

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

        while (is_running)
        {
            m_window.PumpEvents();
        }

        Shutdown();
    }

    // private
    //============================================
    bool Engine::Initialize() // initialize everything
    {
        bool rval = true;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
            rval = false;
        }
       

        if (rval && !m_window.Create())
        {
            std::cout << "Error creating SDL_Window: " << SDL_GetError() << std::endl;
            rval = false;
        }
       

        if (!rval)
            Shutdown();
        else
            is_running = true;
      
        return rval;
    }

    //============================================
    void Engine::Shutdown() // destroy everything
    {
        m_window.Shutdown(); // close window
        SDL_Quit(); // deactivate SDL
    }

    //============================================
    void Engine::GetInfo()
    {
        SDL_version version;
        SDL_VERSION(&version);

        std::cout << "SDL " << (int)version.major
            << "." << (int)version.minor
            << "." << (int)version.patch
            << std::endl;

        #ifdef  _2NK_CONFIG_DEBUG
        std::cout << "Configuration: DEBUG" << std::endl;
        #endif
        #ifdef  _2NK_CONFIG_RELEASE
        std::cout << "Configuration: RELEASE" << std::endl;
        #endif
        #ifdef  _2NK_PLATFORM_WINDOWS
        std::cout << "Platform: WINDOWS" << std::endl;
        #endif
        #ifdef  _2NK_PLATFORM_MAC
        std::cout << "Platform: MAC" << std::endl;
        #endif
        #ifdef  _2NK_PLATFORM_LINUX
        std::cout << "Platform: LINUX" << std::endl;
        #endif
    }

    // Singleton
    //============================================
    Engine* Engine::m_instance = nullptr;

    //============================================
    Engine::Engine() : is_running(false)
    {
        GetInfo();
    }
}