#include <iostream>
#include "2NK_engine/engine.h"

int main (int argc, char* argv[])
{
    TNK::Engine& engine = TNK::Engine::GetInstance();
    engine.Run();
    

    std::cout << "Press ENTER to continue...";
    std::cin.ignore();
    return 0;
}