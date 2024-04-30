#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "Lua.h"


#include "../main.h"
#include "../mod_loader.h"
#include "../cave_story.h"

#include "../AutPI.h"
#include "../Lighting.h"
#include "../LightDefines.h"

#define FUNCTION_TABLE_LIGHTING_SIZE 4


void SetLightingGlobalString()
{

}

void PushLightingMetadata()
{

}

static int lua_GetLightingLevel(lua_State* L)
{
	lua_pushnumber(L, gCurrentDarkLevel);

	return 1;
}

static int lua_SetLightingLevel(lua_State* L)
{
	int no = (int)luaL_checknumber(L, 1);
	gCurrentDarkLevel = no;

	return 0;
}

static int lua_GetStageLighting(lua_State* L)
{
	if (APPROXIMATE_STAGE_LIGHTING)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

static int lua_SetStageLighting(lua_State* L)
{
	int booleanValue = lua_toboolean(L, 1);

	APPROXIMATE_STAGE_LIGHTING = booleanValue;

	return 0;
}
FUNCTION_TABLE LightingFunctionTable[FUNCTION_TABLE_LIGHTING_SIZE] =
{
	{"GetLight", lua_GetLightingLevel},
	{"SetLight", lua_SetLightingLevel},
	{"GetStageLighting", lua_GetStageLighting},
	{"SetStageLighting", lua_SetStageLighting}
};


void SetLightingLua()
{
	PushFunctionTable(GetLuaL(), "Lighting", LightingFunctionTable, FUNCTION_TABLE_LIGHTING_SIZE, TRUE);;
}