
#include "Engine.h"

// Entry Point

int main(int argc, char* argv[]) {

	{
		Engine engine((char*)"New Window\0", 800, 600);
		engine.eRunGame();
	}
	return 0;
}