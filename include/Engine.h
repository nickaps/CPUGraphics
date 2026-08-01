

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

struct vertices {
	std::vector<float3> verts;
};


class Engine {
public:

	// Screen methods
	void sSetDrawColor(SDL_Color c);
	void sFillScreen(SDL_Color c);
	void sDrawPoint(float2 point);
	void sRenderVertex(float3 pos);
	void sRenderVertices(vertices v);

	// Static methods
	static float2 eScreenPosition(float x, float y);
	static float2 eProject(float x, float y, float z);
	static float2 sProjectPoint(float3 pos);
	static void eTranslateMesh(vertices& v, float3 dsplcmnt);

	// Virtual methods
	virtual int eInitializeWindow();
	virtual int eRunGame(char* windowName, int width, int height);
	virtual int ePreload();
	virtual int eGameStep();

	virtual int eOnGameStart();
	virtual int eOnUpdate();
};



// Globals

GAME_PROPERTIES properties = { (char*)"New Window", 800, 600 };
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

SDL_Color background{
	10,10,80,255
};
SDL_Color foreground{
	255,200,200,255
};


float3 pos1 = {0.2f, 0.0f, -0.2f};
float3 pos2 = {-0.2f, 0.0f, -0.2f};
float3 pos3 = { -0.2f, 0.1f, 0.2f };
float3 pos4 = { 0.2f, 0.1f, 0.2f };
float3 pos5 = { 0.0f, 0.5f, 0.0f };

vertices mesh;

// Engine Methods

int Engine::eInitializeWindow() {

	// 1. Initializing SDL & error check
	if (SDL_Init(SDL_INIT_EVERYTHING) == 1)
	{
		std::cout << ": Could not initialize SDL [!]\n";
		return 1;
	}
	else
	{
		std::cout << ": SDL Initialized [*]\n";
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
	if (window == nullptr) {
		std::cout << ": Window could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Window Created [*]\n";
	}

	// 3. Creating renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	// 3a. Error check for renderer
	if (renderer == nullptr) {
		std::cout << ": Renderer could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Renderer Created [*]\n";
	}

	return 0;
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
		}
		eGameStep();
	}

	SDL_Quit();

	return 0;
}

int Engine::eOnGameStart() {		// Implementation determined by user
	mesh.verts.push_back(pos1);
	mesh.verts.push_back(pos2);
	mesh.verts.push_back(pos3);
	mesh.verts.push_back(pos4);
	mesh.verts.push_back(pos5);
	return 0;
}

int Engine::eOnUpdate() {			// Implementation determined by user
	this->sSetDrawColor(foreground);
	this->sRenderVertices(mesh);
	Engine::eTranslateMesh(mesh, float3 {0, 0, 0.00001f});
	return 0;
}

int Engine::ePreload() {
	std::cout << ": Executing Preload Methods [*]\n";

	this->eOnGameStart();

	return 0;
}

int Engine::eGameStep() {
	// Clear the screen
	sFillScreen(background);
	// Play update behavior
	eOnUpdate();
	//Present the renderer
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
		(x + 1) / 2 * properties.screenWidth,
		(y + 1) / 2 * properties.screenHeight
	};
}
void Engine::eTranslateMesh(vertices& v, float3 dsplcmnt) {
	size_t count = v.verts.size();
	for (int i = 0; i < count; i++) {
		v.verts[i].x = v.verts[i].x + dsplcmnt.x;
		v.verts[i].y = v.verts[i].y + dsplcmnt.y;
		v.verts[i].z = v.verts[i].z + dsplcmnt.z;
	}
}

// Screen Methods

void Engine::sSetDrawColor(SDL_Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void Engine::sFillScreen(SDL_Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderClear(renderer);
}

void Engine::sDrawPoint(float2 point) {

	SDL_FRect dot{
		point.x,
		point.y,
		3,
		3
	};

	SDL_RenderDrawRectF(renderer, &dot);
}

float2 Engine::sProjectPoint(float3 pos) {
	float2 proj = Engine::eProject(pos.x, pos.y, pos.z);
	float2 sPos = Engine::eScreenPosition(proj.x, proj.y);
	return sPos;
}

void Engine::sRenderVertex(float3 pos) {
	float2 sPos = Engine::sProjectPoint(pos);
	this->sDrawPoint(sPos);
}

void Engine::sRenderVertices(vertices v) {
	size_t count = v.verts.size();
	for (int i = 0; i < count; i++) {
		this->sRenderVertex(v.verts[i]);
	}
}