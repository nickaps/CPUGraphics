

/*
/ / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

	CPUGraphics by Nick Chapman
	Project started on 4/23/2026

	A simple graphics rasterizer (sans hardware acceleration) that
	uses SDL and is written in C++. The goal is to be able to render
	any mesh with UV data and textures.

/ / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
*/

#ifndef CPUGRAPHICSENG
#define CPUGRAPHICSENG
#endif

#include <SDL\SDL.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string.h>


// Structs
struct GAME_PROPERTIES {
	char* windowName;
	int screenWidth;
	int screenHeight;
};

struct float2 {
	float x;
	float y;
};

struct float3 {
	float x;
	float y;
	float z;
};

struct float4 {
	float x;
	float y;
	float z;
	float w;
};

class Engine {
public:

	// Screen methods
	void sDrawPoint(float2 point);
	void sClearScreen(float3 color);

	// Static methods
	static float2 eScreenPosition(float x, float y);
	static float2 eProject(float x, float y, float z);

	// Virtual methods
	virtual int eInitializeWindow();
	virtual int eRunGame(char* windowName, int width, int height);
	virtual int ePreload();
	virtual int eGameStep(SDL_Event* e);

	virtual int eOnGameStart();
	virtual int eOnUpdate(SDL_Event* e);
};



// Globals

GAME_PROPERTIES properties = { (char*)"New Window", 800, 600 };
SDL_Window* window;
SDL_Renderer* renderer;


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

	// 2. Creating renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	// 3a. Error check for renderer
	if (renderer == NULL) {
		std::cout << ": Renderer could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Renderer Created [*]\n";
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

	// 2.a Run ePreload() before game loop
	ePreload();

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

int Engine::eOnGameStart() {		// Implementation determined by user
	return 0;
}

int Engine::eOnUpdate(SDL_Event* e) {			// Implementation determined by user

	return 0;
}

int Engine::ePreload() {
	std::cout << ": Executing Preload Methods [*]\n";

	this->eOnGameStart();

	return 0;
}

int Engine::eGameStep(SDL_Event* e) {
	// Clear the screen
	this->sClearScreen(float3{ 10,10,60 });
	// Play update behavior
	this->eOnUpdate(e);
	// Present the renderer
	SDL_RenderPresent(renderer);

	return 0;
}

float2 Engine::eProject(float x, float y, float z) {
	return float2{
		x / z,
		y / z
	};
}

float2 Engine::eScreenPosition(float x, float y) {
	return float2{
		((x + 1) / 2) * 2,
		((1 - (y + 1)) / 2) * 2
	};
}


// Screen Methods

void Engine::sClearScreen(float3 color) {
	SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
	SDL_RenderClear(renderer);
}

void Engine::sDrawPoint(float2 point) {
	
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderDrawPoint(renderer, point.x, point.y);
}

