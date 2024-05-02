#include <Windows.h>
#include <stdio.h>
#include "cave_story.h"
#include <SDL.h>
#include "AutPI.h"
#include "SDL_API.h"
#include "Lighting.h"
#include "LightDefines.h"
#include "Main.h"
#include "Lighting_PauseMenu.h"
#include "API_Pause.h"

const char* const gLightConfigName = "SDL_Lighting.dat";
const char* const gLightProof = "SDL2L   20240430";

LIGHTCONFIG gLightConfig;

LIGHTCONFIG* GetLightConf()
{
	return &gLightConfig;
}

void DefaultLightConfigData(LIGHTCONFIG* conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(LIGHTCONFIG));

	strcpy(conf->proof, gLightProof);

	conf->disable_lighting = false;
}

BOOL LoadLightConfigData(LIGHTCONFIG* conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(LIGHTCONFIG));

	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s\\%s\\%s", gDataPath, "Config", gLightConfigName);

	// Open file
	FILE* fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;

	// Read the version id
	fread(conf->proof, sizeof(conf->proof), 1, fp);

	// Read framerate toggle
	conf->disable_lighting = fgetc(fp);

	// Close file
	fclose(fp);

	// Check if version is not correct, and return if it failed
	if (strcmp(conf->proof, gLightProof))
	{
		memset(conf, 0, sizeof(LIGHTCONFIG));
		return FALSE;
	}

	printf("%s", "sdl config loaded successfully\n");
	return TRUE;
}

BOOL SaveLightConfigData(const LIGHTCONFIG* conf)
{
	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s\\%s\\%s", gDataPath, "Config", gLightConfigName);

	// Open file
	FILE* fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;

	// Write the version id
	fwrite(conf->proof, sizeof(conf->proof), 1, fp);

	// Write framerate toggle
	fputc(conf->disable_lighting, fp);

	// Close file
	fclose(fp);

	return TRUE;
}

static int Callback_Lighting(OptionsMenu* parent_menu, size_t this_option, CallbackAction action)
{
	LIGHTCONFIG* conf = (LIGHTCONFIG*)parent_menu->options[this_option].user_data;

	const char* strings[] = { "Enabled", "Disabled" };

	switch (action)
	{
	case ACTION_INIT:
		parent_menu->options[this_option].value = conf->disable_lighting;
		parent_menu->options[this_option].value_string = strings[conf->disable_lighting];
		break;

	case ACTION_DEINIT:
		conf->disable_lighting = parent_menu->options[this_option].value;
		break;

	case ACTION_OK:
	case ACTION_LEFT:
	case ACTION_RIGHT:
		// Increment value (with wrapping)
		parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

		gDisableLighting = parent_menu->options[this_option].value;

		PlaySoundObject(4, SOUND_MODE_PLAY);

		parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
		break;

	case ACTION_UPDATE:
		break;
	}

	return CALLBACK_CONTINUE;
}

void SaveLightingConfig()
{
	SaveLightConfigData(&gLightConfig);
}

void InitLightingPauseMenuCalls()
{
	if (!LoadLightConfigData(&gLightConfig))
	{
		DefaultLightConfigData(&gLightConfig);
	}
	else
	{
		// Set config values if they exist here
		gDisableLighting = gLightConfig.disable_lighting;
	}

	if (DisallowLightingDisable == false)
		add_pause_entry(GetOptionsMenu(), "Lighting", Callback_Lighting, GetLightConf(), NULL, 0, FALSE, GetNumEntriesAddedMenu());
}