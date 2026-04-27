#pragma once

struct mesh;
struct vertex;
struct float2;
struct float3;

int eInitializeWindow();
int eRunGame(char* windowName, int width, int height);
int eGameStep(SDL_Event *e);
mesh eImportObject(char* filepath);