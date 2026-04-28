#pragma once

struct mesh;
struct vertex;
struct float2;
struct float3;

class Engine {
	public:
		int eInitializeWindow();
		int ePreload();
		int eRunGame(char* windowName, int width, int height);
		int eGameStep(SDL_Event* e);
		mesh eImportObject(char* filepath);
};