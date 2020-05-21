#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event* g_event;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT =640;
const int SCREEN_BPP = 32;

#endif // COMMONFUNCTION_H
