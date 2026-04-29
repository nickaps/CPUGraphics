#pragma once

struct mesh;
struct vertex;
struct float2;
struct float3;

class Engine {
	public:
		// Virtual Methods
		virtual int eInitializeWindow();
		virtual int ePreload();
		virtual int eRunGame(char* windowName, int width, int height);
		virtual int eGameStep(SDL_Event* e);
};