#pragma once

#include "engine.h"
#include "app.h"

/* CreateApp()
* User should implement this funcion.
* The client returns a pointer to an instance of a class derived from tonic::App
* The ownership of the returned pointer belongs to TONIC, and will be managed as such
* 
* Example:
* 
* #include "tonic/app.h"
* #include "tonic/main.h"
* 
* class MyGame : public tonic::App 
* {
*   ... implement methods ...
* };
* 
* tonic::App* CreateApp() { return new ClientApp(); }
* 
*/
tonic::App* CreateApp();


// Runs an app created by CreateApp function.
int main()
{
	tonic::App* app = CreateApp();
	tonic::Engine::GetInstance().Run(app);
	delete app;
	return 0;
}