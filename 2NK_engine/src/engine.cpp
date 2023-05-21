#include "engine.h"
#include <iostream>

#include "SDL2/SDL.h"

namespace tnk
{
    void GetInfo()
    {
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

    bool Initialize()
    {

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
            return false;
        }
        

        SDL_version version;
        SDL_VERSION(&version);
        std::cout << "SDL " << (int)version.major << "." << (int)version.minor << "." << (int)version.patch << std::endl;
        
        return true;
    }

    void Shutdown()
    {
        SDL_Quit();
    }
}