
#include "Engine.h"

// Entry Point

int main(int argc, char* argv[]) {

	{

		Engine engine((char*)"New Window\0", 1600, 1000);

		engine.pathsToMeshes.push_back("../resources/test.obj");
		engine.pathsToMeshes.push_back("../resources/house.obj");
		engine.pathsToMeshes.push_back("../resources/monkey.obj");
		engine.pathsToMeshes.push_back("../resources/cube.obj");
		engine.pathsToMeshes.push_back("../resources/yard/yard.obj");
		engine.pathsToMeshes.push_back("../resources/yard/shed.obj");
		engine.pathsToMeshes.push_back("../resources/yard/bushes.obj");
		engine.pathsToMeshes.push_back("../resources/yard/bush.obj");

		engine.colorsToRender.reserve(5);
		engine.colorsToRender.push_back(engine.red);
		engine.colorsToRender.push_back(engine.green);
		engine.colorsToRender.push_back(engine.blue);
		engine.colorsToRender.push_back(engine.red);
		engine.colorsToRender.push_back(engine.foreground);
		engine.colorsToRender.push_back(engine.green);
		engine.colorsToRender.push_back(engine.green);

		engine.eRunGame();
	}

	return 0;
}
