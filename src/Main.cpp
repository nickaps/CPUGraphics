
#define _CRTDBG_MAP_ALLOC
#include "Engine.h"

// Entry Point

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		Engine engine((char*)"New Window\0", 800, 600);
		engine.eRunGame();
	}

	_CrtDumpMemoryLeaks();

	return 0;
}