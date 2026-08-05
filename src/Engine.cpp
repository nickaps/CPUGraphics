
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

	// Find all Paths to a new .obj file and add them to the loadedMeshes vectors
	for (int i = 0; i < this->pathsToMeshes.size(); i++) {
		mesh* m;
		m = this->eParseMesh(this->pathsToMeshes[i]);
		this->loadedMeshes.push_back(m);
	}

	return 0;
}

int Engine::eFreeMeshes() {

	delete this->mPyramid;
	
	for (int i = 0; i < loadedMeshes.size(); i++) {
		delete loadedMeshes[i];
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
	this->eTranslateMesh(mPyramid, float3{ 0, -2.0f, 10.0f });
	this->eTranslateMesh(loadedMeshes[0], float3{ 5.0, -2.0f, 10.0f});
	this->eTranslateMesh(loadedMeshes[1], float3{ 3.0, -2.0f, 10.0f });
	this->eTranslateMesh(loadedMeshes[2], float3{ -3.0, -2.0f, 10.0f });
	this->eTranslateMesh(loadedMeshes[3], float3{ -5.0, -2.0f, 10.0f });
	return 0;
}

int Engine::eOnUpdate() {			// Implementation determined by user
	sSetDrawColor(foreground);

	// Renders
	this->sRenderMesh(*mPyramid, red);
	this->sRenderMesh(*loadedMeshes[0], green);
	this->sRenderMesh(*loadedMeshes[1], blue);
	this->sRenderMesh(*loadedMeshes[2], red);
	this->sRenderMesh(*loadedMeshes[3], foreground);

	// Translates
	this->eTranslateMesh(mPyramid, float3{ 0, (sinf(this->time) * 0.001f), 0});
	this->eTranslateMesh(loadedMeshes[0], float3{ 0, (cosf(this->time) * 0.001f), 0 });
	this->eTranslateMesh(loadedMeshes[1], float3{ 0, (sinf(this->time) * 0.001f), 0 });
	this->eTranslateMesh(loadedMeshes[2], float3{ 0, (cosf(this->time) * 0.001f), 0 });
	this->eTranslateMesh(loadedMeshes[3], float3{ 0, (sinf(this->time) * 0.001f), 0 });

	return 0;
}

int Engine::ePreload() {
	std::cout << ": Executing Preload Methods [*]\n";

	int flag = this->eLoadMeshes();;

	if (flag == 1) {
		std::cout << ": Failed to load meshes [!]\n";
	}
	else {
		std::cout << ": Successfully loaded meshes [*]\n";
	}

	this->eOnGameStart();

	return 0;
}

int Engine::eGameStep() {
	// Clear the screen
	sFillScreen(background);
	// Call update behavior
	eOnUpdate();
	// Present the renderer
	SDL_RenderPresent(renderer);

	this->time += 0.001f;

	return 0;
}

float2 Engine::eProject(float x, float y, float z) {

	float zpm = z;
	if (z <= 0.001f) zpm = 0.001f;
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

void Engine::sDrawLine(float x1, float y1, float x2, float y2) {

	int flag = SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	return;
}

void Engine::sDrawTriangle(float2 p1, float2 p2, float2 p3, SDL_Color c) {
	// Sutherman-Hodgman Algorithm...
	// 
	// I'm sure there is a better way to do
	// this but were gonna roll for now.

	if ((p1.y > properties->screenHeight || p1.y < 0) || (p1.x > properties->screenWidth || p1.x < 0) &&
		(p2.y > properties->screenHeight || p2.y < 0) || (p2.x > properties->screenWidth || p2.x < 0) &&
		(p3.y > properties->screenHeight || p3.y < 0) || (p3.x > properties->screenWidth || p3.x < 0)) return;

	if (p1.y > properties->screenHeight) p1.y = properties->screenHeight + 1;
	if (p2.y > properties->screenHeight) p2.y = properties->screenHeight + 1;
	if (p3.y > properties->screenHeight) p3.y = properties->screenHeight + 1;

	if (p1.y < 0) p1.y = -1;
	if (p2.y < 0) p2.y = -1;
	if (p3.y < 0) p3.y = -1;

	if (p1.x > properties->screenWidth) p1.x = properties->screenWidth + 1;
	if (p2.x > properties->screenWidth) p2.x = properties->screenWidth + 1;
	if (p3.x > properties->screenWidth) p3.x = properties->screenWidth + 1;

	if (p1.x < 0) p1.x = -1;
	if (p2.x < 0) p2.x = -1;
	if (p3.x < 0) p3.x = -1;

	SDL_Vertex vertices[3];

	vertices[0].color = c;
	vertices[0].color.r *= 0.5;
	vertices[0].color.b *= 0.8;
	vertices[0].color.g *= 0.5;
	vertices[0].position = { p1.x, p1.y };
	vertices[0].tex_coord = { 0.0f, 0.0f };

	vertices[1].color = c;
	vertices[0].color.r *= 0.9;
	vertices[0].color.b *= 0.8;
	vertices[0].color.g *= 0.9;
	vertices[1].position = { p2.x, p2.y };
	vertices[1].tex_coord = { 0.0f, 0.0f };

	vertices[2].color = c;
	vertices[2].position = { p3.x, p3.y };
	vertices[2].tex_coord = { 0.0f, 0.0f };

	SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
}

void Engine::sRenderTriangle(float3 v1, float3 v2, float3 v3, SDL_Color c) {
	float2 p1 = sProjectPoint(v1);
	float2 p2 = sProjectPoint(v2);
	float2 p3 = sProjectPoint(v3);

	this->sDrawTriangle(p1, p2, p3, c);
}

void Engine::sRenderMesh(mesh m, SDL_Color c) {

	std::vector<triangle> triangles = m.tris;

	int count = triangles.size();

	int i1; int i2; int i3;
	float2 p1; float2 p2; float2 p3;

	std::vector<triangle> tris_cpy = triangles;
	std::sort(tris_cpy.begin(), tris_cpy.end(), [m](const auto& a, const auto& b) {
		return ((m.verts[a.v1].z + m.verts[a.v2].z + m.verts[a.v3].z) / 3) > ((m.verts[b.v1].z + m.verts[b.v2].z + m.verts[b.v3].z) / 3);
		});

	for (int i = 0; i < count; i++) {
		i1 = tris_cpy[i].v1;
		i2 = tris_cpy[i].v2;
		i3 = tris_cpy[i].v3;

		this->sRenderTriangle(m.verts[i1], m.verts[i2], m.verts[i3], c);
	}
}

void Engine::sDrawWireTriangle(float2 p1, float2 p2, float2 p3) {

	// Sutherman-Hodgman Algorithm...
	// 
	// I'm sure there is a better way to do
	// this but were gonna roll for now.

	if ( (p1.y > properties->screenHeight || p1.y < 0) || (p1.x > properties->screenWidth || p1.x< 0) &&
		(p2.y > properties->screenHeight || p2.y < 0) || (p2.x > properties->screenWidth || p2.x < 0) &&
		(p3.y > properties->screenHeight || p3.y < 0) || (p3.x > properties->screenWidth || p3.x < 0)) return;

	if (p1.y > properties->screenHeight) p1.y = properties->screenHeight + 1;
	if (p2.y > properties->screenHeight) p2.y = properties->screenHeight + 1;
	if (p3.y > properties->screenHeight) p3.y = properties->screenHeight + 1;

	if (p1.y < 0) p1.y = -1;
	if (p2.y < 0) p2.y = -1;
	if (p3.y < 0) p3.y = -1;

	if (p1.x > properties->screenWidth) p1.x = properties->screenWidth + 1;
	if (p2.x > properties->screenWidth) p2.x = properties->screenWidth + 1;
	if (p3.x > properties->screenWidth) p3.x = properties->screenWidth + 1;

	if (p1.x < 0) p1.x = -1;
	if (p2.x < 0) p2.x = -1;
	if (p3.x < 0) p3.x = -1;

	this->sDrawLine(p1.x, p1.y, p2.x, p2.y);
	this->sDrawLine(p2.x, p2.y, p3.x, p3.y);
	this->sDrawLine(p3.x, p3.y, p1.x, p1.y);
}

void Engine::sRenderWireTriangle(float3 v1, float3 v2, float3 v3) {

	float2 p1 = sProjectPoint(v1);
	float2 p2 = sProjectPoint(v2);
	float2 p3 = sProjectPoint(v3);

	this->sDrawWireTriangle(p1, p2, p3);
}

void Engine::sRenderWireframeMesh(mesh m) {

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

		/*printf("x%f x%f x%f", p1.x, p2.x, p3.x);
		printf("y%f y%f y%f", p1.y, p2.y, p3.y);*/

		this->sRenderWireTriangle(m.verts[i1], m.verts[i2], m.verts[i3]);
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

mesh* Engine::eParseMesh(std::string filePath) {
	if (filePath == "") return nullptr;

	mesh* m;
	m = new mesh;

	std::string text;
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cout << ": Could not load mesh " << filePath << " [!]\n";
		return nullptr;
	}

	int latestV = 0; int latestF = 0;

	while (getline(file, text)) {

		std::stringstream ss(text);
		std::string prefix;

		ss >> prefix;

		if (prefix == "v") {
			std::string xstr;	float x;
			std::string ystr;	float y;
			std::string zstr;	float z;

			ss >> xstr;
			ss >> ystr;
			ss >> zstr;

			x = std::stof(xstr);
			y = std::stof(ystr);
			z = std::stof(zstr);

			m->verts.push_back(float3 {x, y, z});
		}

		if (prefix == "f") {

			int i = 0;
			triangle tri;

			std::string tmpstr = text;
			std::replace(tmpstr.begin(), tmpstr.end(), '/', ' ');

			std::stringstream tokenStream(tmpstr);
			std::string token;

			tokenStream >> token;

			while (tokenStream >> token) {
				if (i == 0) tri.v1 = std::stoi(token) - 1;
				if (i == 3) tri.v2 = std::stoi(token) - 1;
				if (i == 6) tri.v3 = std::stoi(token) - 1;
				i++;
			}

			m->tris.push_back(tri);
		}
	}

	file.close();

	std::cout << ": Successfully loaded mesh " << filePath << " [*]\n";
	return m;
}
