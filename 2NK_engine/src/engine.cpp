#include "engine.h"
#include <iostream>

int Add(int a, int b) { return a + b; }
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