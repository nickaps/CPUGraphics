

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

#include "SDL/SDL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <vector>
#include <cmath>
#include <numeric>

using std::iota;

// Structs
struct GAME_PROPERTIES {
	const char* windowName;
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

struct triangle {
	int v1;
	int v2;
	int v3;
};

struct mesh {
	std::vector<float3> verts;
	std::vector<triangle> tris;
};

class Engine {
private:
	GAME_PROPERTIES* properties;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	mesh* mPyramid;
public:
	// Screen methods
	void sSetDrawColor(SDL_Color c);
	void sFillScreen(SDL_Color c);
	void sDrawPoint(float2 point);
	void sRenderVertex(float3 pos);
	void sDrawLine(float x1, float y1, float x2, float y2);
	void sDrawWireTriangle(float2 p1, float2 p2, float2 p3);
	void sRenderWireTriangle(float3 v1, float3 v2, float3 v3);
	void sRenderWireframeMesh(mesh m);
	void sDrawTriangle(float2 p1, float2 p2, float2 p3, SDL_Color c, float p1bright, float p2bright, float p3bright);
	void sRenderTriangle(float3 v1, float3 v2, float3 v3, SDL_Color c);
	void sRenderMesh(mesh m, SDL_Color c);
	void sRenderMeshes(std::vector<mesh*> meshes, std::vector<SDL_Color> colors);
	float2 sProjectPoint(float3 pos);

	// Engine Methods
	int eLoadMeshes();
	int eFreeMeshes();
	int eInitializeWindow(char* windowName, int width, int height);
	int eRunGame();
	int ePreload();
	int eGameStep();
	float eGetDistance(float3 a, float3 b);
	float3 eGetCenterOfTri(float3 v1, float3 v2, float3 v3);
	float3 eFindMeanVertex(std::vector<float3> *vertices);
	float eFindBrightnessFromFog(float3 point);

	mesh* eParseMesh(std::string filePath);
	void eTranslateMesh(mesh* m, float3 dsplcmnt);

	virtual int eOnGameStart();
	virtual int eOnUpdate();

	float2 eScreenPosition(float x, float y);
	float2 eProject(float x, float y, float z);

	std::vector<std::string> pathsToMeshes;
	std::vector<mesh*> loadedMeshes;
	std::vector<mesh*> meshesToRender;
	std::vector<SDL_Color> colorsToRender;

	int fogStart = 9.0f;
	int fogEnd = 40.0f;
	SDL_Color fogColor = SDL_Color{10, 10, 10};

	float time = 0.0f;

	SDL_Color background{
		10,10,80,255
	};
	SDL_Color foreground{
		255,200,200,255
	};

	SDL_Color red{
		255,30,30,255
	};
	SDL_Color green{
		30,255,30,255
	};
	SDL_Color blue{
		30,30,255,255
	};

	Engine(char* windowName, int screenWidth, int screenHeight);
	~Engine();
};
