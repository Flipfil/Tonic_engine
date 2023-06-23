#include "tonic/main.h"
#include "space_invaders.h"

tonic::App* CreateApp()
{
	return new SpaceInvaders();
}