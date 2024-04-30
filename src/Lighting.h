#pragma once

#include "cave_story.h"
#include <SDL.h>

struct Surface
{
	struct SDL_Texture* texture;
	unsigned width;
	unsigned height;
	bool render_target;
	bool lost;
};

#define SURFACE_ID_LIGHT 41
#define SURFACE_ID_LIGHT_MAP 43

extern int APPROXIMATE_STAGE_LIGHTING;
extern unsigned int gCurrentDarkLevel;
extern unsigned int gStageLightLevel;

void DoModeActionLighting();