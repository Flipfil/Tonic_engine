#pragma once

#include "engine.h"
#include "app.h"

// To be implemented in client app
// The client returns a pointer to an instance of a class derived from tonic::App
// The ownership of the returnedpointer belongs to TONIC, and will be managed as such
// 
// Example:
// class ClientApp : public tonic::App {};
// tonic::App* CreateApp() { return new ClientApp(); }
tonic::App* CreateApp();

int main()
{
	tonic::App* app = CreateApp();
	tonic::Engine::GetInstance().Run(app);
	delete app;
	return 0;
}