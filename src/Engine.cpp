
#include "../include/Engine.h"

float3 pos1 = { 0.2f, 0.0f, -0.2f };
float3 pos2 = { -0.2f, 0.0f, -0.2f };
float3 pos3 = { -0.2f, 0.1f, 0.2f };
float3 pos4 = { 0.2f, 0.1f, 0.2f };
float3 pos5 = { 0.0f, 0.5f, 0.0f };

float speed = 0.001f;

// Engine Methods

Engine::Engine(char* windowName, int screenWidth, int screenHeight) {
	properties = new GAME_PROPERTIES;

	int flag;
	flag = this->eInitializeWindow(windowName, screenWidth, screenHeight);

	if (flag == 1) {
		std::cout << ": Could not start game [!]\n";
	}
	else {
		std::cout << ": Game started successsfully [*]\n";
	}
}

Engine::~Engine() {


	delete properties;
	properties = nullptr;

	window = nullptr;
	renderer = nullptr;
	texture = nullptr;
}

int Engine::eInitializeWindow(char* windowName, int width, int height) {


	// 1. Set game properties
	properties->screenHeight = height;
	properties->screenWidth = width;
	properties->windowName = windowName;

	// 2. Initializing SDL & error check
	if (SDL_Init(SDL_INIT_EVERYTHING) == 1)
	{
		std::cout << ": Could not initialize SDL [!]\n";
		return 1;
	}
	else
	{
		std::cout << ": SDL Initialized [*]\n";
	}

	// 3. Creating window
	window = SDL_CreateWindow(
		(char*)properties->windowName,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		properties->screenWidth,
		properties->screenHeight,
		SDL_WINDOW_SHOWN
	);

	// 3a. Error check for window
	if (window == nullptr) {
		std::cout << ": Window could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Window Created [*]\n";
	}

	// 4. Creating renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	// 4a. Error check for renderer
	if (renderer == nullptr) {
		std::cout << ": Renderer could not be initialized [!]\n";
		return 1;
	}
	else {
		std::cout << ": Renderer Created [*]\n";
	}

	return 0;
}

int Engine::eRunGame() {


	// 1 Run ePreload() before game loop
	ePreload();

	// 2. Start game loop
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
	mesh.verts.reserve(5);
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
	Engine::eTranslateMesh(mesh, float3{0, 0, speed});
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
		(x + 1) / 2 * Engine::properties->screenWidth,
		(y + 1) / 2 * properties->screenHeight
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