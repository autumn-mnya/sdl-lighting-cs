#include <Windows.h>
#include <stdio.h>
#include "cave_story.h"
#include <SDL.h>
#include "AutPI.h"
#include "SDL_API.h"
#include "Lighting.h"
#include "LightDefines.h"
#include "Main.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static auto& gCrt = *reinterpret_cast<CARET(*)[64]>(0x49BCA8); // idk if i did this right actually

void BlitLightRaw(SDL_Renderer* gRenderer, int fx, int fy, int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    
    void* surfaceVoidPtr = Get_Surface(SURFACE_ID_LIGHT);

    Surface* surfacePtr = static_cast<Surface*>(surfaceVoidPtr);
    SDL_Texture* texture = surfacePtr->texture;

    SDL_SetTextureColorMod(texture, r, g, b);
    SDL_SetTextureAlphaMod(texture, a);
    SDL_Rect rcPlace = { (((x - fx) / 0x200) - width) * window_magnification, (((y - fy) / 0x200) - (height)) * window_magnification, width * window_magnification * 2, height * window_magnification * 2 };
    SDL_RenderCopy(gRenderer, texture, NULL, &rcPlace);
}

void BlitLight(SDL_Renderer* gRenderer, int fx, int fy, LightBlitType type, RECT* framerect, int x, int y, int entity_id) {

    void* surfaceVoidPtr = Get_Surface(SURFACE_ID_LIGHT);

    Surface* surfacePtr = static_cast<Surface*>(surfaceVoidPtr);
    SDL_Texture* texture = surfacePtr->texture;

    //LT RB
    double fwidth = framerect->right - framerect->left;
    double fheight = framerect->bottom - framerect->top;
    LightingData* data = GetLightDataArray(type, entity_id);
    float light_window_magnification = 1.5F;
    if (data->exists) {
        light_window_magnification = data->size;
    }
    fwidth *= light_window_magnification;
    fheight *= light_window_magnification;

    int width = fwidth;
    int height = fheight;
    if (data->exists) {
        SDL_SetTextureColorMod(texture, data->r, data->g, data->b);
        SDL_SetTextureAlphaMod(texture, data->a);
    }
    else {
        SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
        SDL_SetTextureAlphaMod(texture, 0xFF);
    }
    SDL_Rect rcPlace = { (((x - fx) / 0x200) - width) * window_magnification, (((y - fy) / 0x200) - (height)) * window_magnification, width * window_magnification * 2, height * window_magnification * 2 };
    SDL_RenderCopy(gRenderer, texture, NULL, &rcPlace);
}

void RendLightNPC(int fx, int fy)
{
    void* surfaceVoidPtr = Get_Surface(SURFACE_ID_LIGHT);

    Surface* surfacePtr = static_cast<Surface*>(surfaceVoidPtr);
    SDL_Texture* texture = surfacePtr->texture;


    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect clipRect = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
    if (clipRect.w < 0)
        clipRect.w = 0;
    if (clipRect.h < 0)
        clipRect.h = 0;
    clipRect.x *= window_magnification;
    clipRect.y *= window_magnification;
    clipRect.w *= window_magnification;
    clipRect.h *= window_magnification;
    SDL_Renderer* gRenderer = Get_SDL_Renderer();
    SDL_RenderSetClipRect(gRenderer, &clipRect);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    for (int n = 0; n < CARET_MAX; n++) {
        if (gCrt[n].cond & 0x80) {
            BlitLight(gRenderer, fx, fy, LBT_CARET, &gCrt[n].rect, gCrt[n].x, gCrt[n].y, gCrt[n].code);
        }
    }
    for (int n = 0; n < NPC_MAX; n++) {
        if (gNPC[n].cond & 0x80) {
            BlitLight(gRenderer, fx, fy, LBT_NPC, &gNPC[n].rect, gNPC[n].x, gNPC[n].y, gNPC[n].code_char);
        }
    }
    for (int n = 0; n < BULLET_MAX; n++) {
        if (gBul[n].cond & 0x80) {
            BlitLight(gRenderer, fx, fy, LBT_BULLET, &gBul[n].rect, gBul[n].x, gBul[n].y, gBul[n].code_bullet);
        }
    }
    for (int n = 0; n < BOSS_MAX; n++) {
        if (gBoss[n].cond & 0x80) {
            BlitLight(gRenderer, fx, fy, LBT_BOSS, &gBoss[n].rect, gBoss[n].x, gBoss[n].y, gBoss[n].code_char);
        }
    }
    BlitLight(gRenderer, fx, fy, LBT_PLAYER, &gMC.rect, gMC.x, gMC.y, 0);
    SDL_RenderSetClipRect(gRenderer, NULL);
}

unsigned char GetAttributeOrNeg(int x, int y)
{
    size_t a;

    if (x < 0 || y < 0 || x >= gMap.width || y >= gMap.length)
        return -1;

    a = *(gMap.data + x + (y * gMap.width));	// Yes, the original code really does do this instead of a regular array access
    return gMap.atrb[a];
}

void PutStage_Light(int fx, int fy)
{
    int i, j;
    int offset;
    // Get range to draw
    int extra_tiles_drawn = 8;
    int num_x = MIN(gMap.width, ((WINDOW_WIDTH + (16 - 1)) / 16) + 1 + (extra_tiles_drawn * 2));
    int num_y = MIN(gMap.length, ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1 + (extra_tiles_drawn * 2));
    int put_x = MAX(0, ((fx / 0x200) + 8) / 16 - extra_tiles_drawn);
    int put_y = MAX(0, ((fy / 0x200) + 8) / 16 - extra_tiles_drawn);

    int atrb;

    for (j = put_y; j < put_y + num_y; ++j)
    {
        for (i = put_x; i < put_x + num_x; ++i)
        {
            // Get attribute
            offset = (j * gMap.width) + i;
            atrb = GetAttributeOrNeg(i, j);
            if (atrb == 0 || atrb == 0x40 || atrb == 0x01 || atrb == 0x60) {
                //skrrt
                int pwr = 45;
                BlitLightRaw(Get_SDL_Renderer(), fx, fy, i * 0x10 * 0x200, j * 0x10 * 0x200, pwr, pwr, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
}

int APPROXIMATE_STAGE_LIGHTING = 0;
int frame_x = 0, frame_y = 0;
unsigned int gCurrentDarkLevel = 255;
unsigned int gStageLightLevel = 0;

void DoModeActionLighting()
{
    GetFramePosition(&frame_x, &frame_y);
    //===== REDSANDS LIGHTING
    SDL_Renderer* gRenderer = Get_SDL_Renderer();
    SDL_Texture* rendTarg = SDL_GetRenderTarget(gRenderer);
    SDL_BlendMode rendBlendMode;
    SDL_GetRenderDrawBlendMode(gRenderer, &rendBlendMode);

    SDL_SetRenderTarget(gRenderer, darkMap);

    // Clear the renderer (Black, solid alpha)
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, NULL);

    SDL_SetTextureBlendMode(darkMap, SDL_BLENDMODE_MUL);
    RendLightNPC(frame_x, frame_y);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, gCurrentDarkLevel);
    SDL_RenderFillRect(gRenderer, NULL);

    SDL_SetRenderTarget(gRenderer, rendTarg);
    SDL_RenderCopy(gRenderer, darkMap, NULL, NULL);

    // Now that we have drawn npc light, draw tile light
    if (APPROXIMATE_STAGE_LIGHTING) {
        SDL_SetRenderTarget(gRenderer, darkMap);

        // Clear the renderer (Black, solid alpha)
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(gRenderer, NULL);

        SDL_SetTextureBlendMode(darkMap, SDL_BLENDMODE_MUL);
        PutStage_Light(frame_x, frame_y);
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, gStageLightLevel);
        SDL_RenderFillRect(gRenderer, NULL);

        SDL_SetRenderTarget(gRenderer, rendTarg);
        SDL_RenderCopy(gRenderer, darkMap, NULL, NULL);
    }

    //==== END OF REDSANDS LIGHTING
}