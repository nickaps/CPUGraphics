
/*
#####################################################################

	CPUGraphics by Nick Chapman
	Project started on 4/23/2026

	A simple graphics rasterizer (sans hardware acceleration) that
	uses SDL and is written in C++. The goal is to be able to render
	any mesh with UV data and textures.

	It will be a convention that methods that I write for the engine
	that handle behavior (mostly utilizing SDL) will begin with "e..."

	e.g.	eInitializeWindow();
			eRenderLoop();
			eInitialize();
			eDestroy();
			...

######################################################################
*/

#include "SDL/SDL.h"
#include "CPUGraphics.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string.h>

struct color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct float3 {
	float x;
	float y;
	float z;
};

struct float2 {

	float x;
	float y;

	// (x, y) -> (x, y, 0)
	float3 toFloat3() {
		return float3{
			this->x, this->y, 0,
		};
	}

};

struct vertex {
	int v;
	int vt;
	int vn;
};

struct face {
	std::vector<vertex> vertices;
};

struct mesh {
	std::vector<float3> v_positions;
	std::vector<float2> v_texture;
	std::vector<float3> v_normal;
	std::vector<face> faces;
};

struct GAME_PROPERTIES {

	char* windowName;
	int screenWidth;
	int screenHeight;
	
};


// Globals

GAME_PROPERTIES properties = { (char*)"New Window", 800, 600 };
SDL_Window* window;


// Entry Point

int main(int argc, char* argv[]) {

	Engine engine;
	engine.eRunGame((char*)"New Window\0", 800, 600);

	return 0;
}


// Data Methods

mesh Engine::eImportObject(char* filepath) {
	// TODO
	
	mesh m;
	return m;
}


// Engine Methods

int Engine::eInitializeWindow() {

	// 1. Initializing video & error check
	if (SDL_Init(SDL_INIT_VIDEO) == 1)
	{
		std::cout << ": Could not initialize video [!]\n";
		return 1;
	}
	else
	{
		std::cout << ": Video Initialized [*]\n";
	}

	// 2. Creating window
	window = SDL_CreateWindow(
		(char*)properties.windowName,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		properties.screenWidth,
		properties.screenHeight,
		SDL_WINDOW_SHOWN
	);

	// 2a. Error check for window
	if (window == NULL) {
		std::cout << ": Window could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Window Created [*]\n";
		return 0;
	}
}

int Engine::eRunGame(char* windowName, int width, int height) {

	// 1. Set game properties
	properties = GAME_PROPERTIES{
		windowName,
		width,
		height
	};

	// 2. Run eInitializeWindow method and error check
	int flag = eInitializeWindow();
	if (flag == 1) {
		std::cout << ": Could not start game [!]\n";
	}
	else {
		std::cout << ": Game started successsfully [*]\n";
	}

	// 3. Start game loops
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
			else eGameStep(&e);
		}
	}

	return 0;
}

int Engine::ePreload() {
	// TODO
	return 0;
}

int Engine::eGameStep(SDL_Event* e) {
	// std::cout << e->type << "\n";
	return 0;
}