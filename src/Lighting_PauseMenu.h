#pragma once

#include "cave_story.h"
#include <SDL.h>
#include "API_Pause.h"

struct LIGHTCONFIG
{
	char proof[0x20];
	bool disable_lighting;
};

void SaveLightingConfig();
void InitLightingPauseMenuCalls();