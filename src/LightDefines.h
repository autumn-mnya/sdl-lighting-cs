//
// Created by Jake on 5/11/2021.
//

#pragma once

#include <stdint.h>
#include "cave_story.h"

enum LightBlitType{
    LBT_PLAYER,
    LBT_NPC,
    LBT_CARET,
    LBT_BULLET,
    LBT_BOSS,
};
void LoadLightingData();
typedef struct LightingData{
    BOOL exists;
    LightBlitType type;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    float size;
} LightingData;

extern LightingData gNPCLighting[NPC_MAX];
extern LightingData gCaretLighting[CARET_MAX];
extern LightingData gMyCharLighting;
extern LightingData gBulletLighting[BULLET_MAX];
extern LightingData gBossLighting[BOSS_MAX];

LightingData* GetLightDataArray(LightBlitType type, int id);

int HexToInt(char c);
