#include <iostream>
#include "2NK_engine/engine.h"

int main (int argc, char* argv[])
{
    std::cout << "Hello" << std::endl;
    int a = Add(10, 5);
    int b = Sub(10, 5);
    std::cout << a << ", " << b << std::endl;
    GetInfo();
    return 0;
}