//#include "Assessment.h"
#include "PhysicsApp.h"

//Needed for memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	//Memory Leak Detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//auto app = new Assessment();
	auto app = new PhysicsApp();
	app->run("Physics Simulation", 1920, 1080, false);
	delete app;
	
	return 0;
}