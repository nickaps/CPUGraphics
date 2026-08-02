

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
#include <filesystem>
#include <vector>
#include <string.h>

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

struct vertices {
	std::vector<float3> verts;
};


class Engine {
private:
	GAME_PROPERTIES* properties;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	vertices mesh;
public:

	// Screen methods
	void sSetDrawColor(SDL_Color c);
	void sFillScreen(SDL_Color c);
	void sDrawPoint(float2 point);
	void sRenderVertex(float3 pos);
	void sRenderVertices(vertices v);
	float2 sProjectPoint(float3 pos);

	// Engine Methods
	int eInitializeWindow(char* windowName, int width, int height);
	int eRunGame();
	int ePreload();
	int eGameStep();

	virtual int eOnGameStart();
	virtual int eOnUpdate();

	float2 eScreenPosition(float x, float y);
	float2 eProject(float x, float y, float z);
	void eTranslateMesh(vertices& v, float3 dsplcmnt);

	SDL_Color background{
		10,10,80,255
	};
	SDL_Color foreground{
		255,200,200,255
	};

	Engine(char* windowName, int screenWidth, int screenHeight);
	~Engine();
};