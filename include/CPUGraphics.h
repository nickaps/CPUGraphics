#pragma once

int eInitializeWindow();
int eRunGame(char* windowName, int width, int height);
int eGameStep(SDL_Event *e);