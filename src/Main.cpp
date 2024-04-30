#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "mod_loader.h"
#include "cave_story.h"
#include "AutPI.h"
#include "SDL_API.h"

#include <SDL.h>

#include "Lighting.h"
#include "LightDefines.h"
#include "lua/Lua.h"

SDL_Texture* darkMap;

int mag = 2;

void InitSurfaces()
{
	MakeSurface_File("Light", SURFACE_ID_LIGHT);
	MakeSurface_Generic(WINDOW_WIDTH, WINDOW_HEIGHT, SURFACE_ID_LIGHT_MAP, FALSE);
}

void DirectDraw()
{
	SDL_Renderer* gRenderer = Get_SDL_Renderer();

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	darkMap = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH * window_magnification, WINDOW_HEIGHT * window_magnification);
	//    SDL_SetTextureBlendMode(darkMap, SDL_BLENDMODE_MOD);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void InitMod(void)
{
	LoadAutPiDll();
	LoadSdlDll();
	RegisterSDLStartDirectDrawElement(DirectDraw);
	RegisterPreModeElement(InitSurfaces);
	LoadLightingData();

	RegisterOpeningAbovePutCaretElement(DoModeActionLighting);
	RegisterAbovePutCaretElement(DoModeActionLighting);

	RegisterLuaFuncElement(SetLightingLua);
}
