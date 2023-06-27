#include "tonic/main.h"
#include "space_invaders.h"

// core code is in space_invaders.h and space_invaders*.cpp files to allow for compilation dependencies

tonic::App* CreateApp()
{
	return new SpaceInvaders();
}