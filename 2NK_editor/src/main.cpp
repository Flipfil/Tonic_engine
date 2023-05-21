#include <iostream>
#include "2NK_engine/engine.h"

int main (int argc, char* argv[])
{
    std::cout << "Hello" << std::endl;
    tnk::GetInfo();

    tnk::Initialize();
    tnk::Shutdown();
    return 0;
}