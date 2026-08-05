
#include "Engine.h"

// Entry Point

int main(int argc, char* argv[]) {

	{


		Engine engine((char*)"New Window\0", 360, 360);

		engine.pathsToMeshes.push_back("../resources/test.obj");
		engine.pathsToMeshes.push_back("../resources/house.obj");
		engine.pathsToMeshes.push_back("../resources/monkey.obj");
		engine.pathsToMeshes.push_back("../resources/cube.obj");

		engine.eRunGame();
	}

	return 0;
}