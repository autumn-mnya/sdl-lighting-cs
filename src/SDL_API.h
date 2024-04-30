#pragma once

#include <Windows.h>
#include <SDL.h>

extern HMODULE sdlDLL;

typedef void (*SdlStartDirectDrawElementHandler)();

void LoadSdlDll();
SDL_Window* Get_SDL_Window();
SDL_Renderer* Get_SDL_Renderer();
void* Get_Surface(int surfaceID);
void RegisterSDLStartDirectDrawElement(SdlStartDirectDrawElementHandler handler);