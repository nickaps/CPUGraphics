
#include "../include/Engine.h"

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

	flag = this->eLoadMeshes();

	if (flag == 1) {
		std::cout << ": Failed to load meshes [!]\n";
	}
	else {
		std::cout << ": Successfully loaded meshes [*]\n";
	}
}

Engine::~Engine() {

	eFreeMeshes();

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

int Engine::eLoadMeshes() {
	
	// Default Built-in Pyramid
	mPyramid = new mesh;
	mPyramid->verts.reserve(5);

	mPyramid->verts.push_back(float3{ 0.5, -0.5, 0.5 });
	mPyramid->verts.push_back(float3{ 0.5, -0.5, -0.5 });
	mPyramid->verts.push_back(float3{ -0.5, -0.5, -0.5 });
	mPyramid->verts.push_back(float3{ -0.5, -0.5, 0.5 });
	mPyramid->verts.push_back(float3{ 0, 0.5, 0 });

	mPyramid->tris.reserve(6);

	mPyramid->tris.push_back(triangle{ 0, 4, 3 });
	mPyramid->tris.push_back(triangle{ 1, 4, 0 });
	mPyramid->tris.push_back(triangle{ 2, 4, 1 });
	mPyramid->tris.push_back(triangle{ 3, 4, 2 });
	mPyramid->tris.push_back(triangle{ 1, 3, 0 });
	mPyramid->tris.push_back(triangle{ 3, 1, 2 });

	return 0;
}

int Engine::eFreeMeshes() {

	delete this->mPyramid;

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

	return 0;
}

int Engine::eOnUpdate() {			// Implementation determined by user
	sSetDrawColor(foreground);
	this->sRenderMesh(*mPyramid);
	this->eTranslateMesh(mPyramid, float3{ 0, 0, 0.01f });

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
	float zpm = z;
	if (zpm <= 0.01f) {
		zpm = 0.001f;
	}
	return float2{
		x / zpm,
		y / zpm
	};
}

float2 Engine::eScreenPosition(float x, float y) {
	return float2{
		(x + 1) / 2 * properties->screenWidth,
		(1 - y) / 2 * properties->screenHeight
	};
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
	float2 proj = eProject(pos.x, pos.y, pos.z);
	float2 sPos = eScreenPosition(proj.x, proj.y);
	return sPos;
}

void Engine::sRenderVertex(float3 pos) {
	float2 sPos = Engine::sProjectPoint(pos);
	this->sDrawPoint(sPos);
}

void Engine::sRenderLine(float x1, float y1, float x2, float y2) {

	int flag = SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	return;
}

void Engine::sRenderMesh(mesh m) {

	size_t count = m.verts.size();
	for (int i = 0; i < count; i++) {
		this->sRenderVertex(m.verts[i]);
	}

	count = m.tris.size();

	int i1; int i2; int i3;
	float2 p1; float2 p2; float2 p3;

	for (int i = 0; i < count; i++) {

		i1 = m.tris[i].v1;
		i2 = m.tris[i].v2;
		i3 = m.tris[i].v3;

		/*printf("%d %d %d", i1, i2, i3);*/

		p1 = sProjectPoint({ m.verts[i1].x, m.verts[i1].y, m.verts[i1].z });
		p2 = sProjectPoint({ m.verts[i2].x, m.verts[i2].y, m.verts[i2].z });
		p3 = sProjectPoint({ m.verts[i3].x, m.verts[i3].y, m.verts[i3].z });
		/*printf("x%f x%f x%f", p1.x, p2.x, p3.x);
		printf("y%f y%f y%f", p1.y, p2.y, p3.y);*/

		if (m.verts[i1].z <= 0.1f && m.verts[i2].z <= 0.1f && m.verts[i3].z <= 0.1f) return;

		this->sRenderLine(p1.x, p1.y, p2.x, p2.y);
		this->sRenderLine(p2.x, p2.y, p3.x, p3.y);
		this->sRenderLine(p3.x, p3.y, p1.x, p1.y);

	}

}

void Engine::eTranslateMesh(mesh* m, float3 dsplcmnt)
{
	if (m == nullptr || &m->verts[0] == nullptr) return;

	int count = m->verts.size();

	for (int i = 0; i < count; i++) {
		m->verts[i].x += dsplcmnt.x;
		m->verts[i].y += dsplcmnt.y;
		m->verts[i].z += dsplcmnt.z;
	}
}

mesh* Engine::eParseMesh(char* filePath) {
	if (filePath == nullptr) return nullptr;

	mesh* m;

	m = new mesh;



	return m;
}
