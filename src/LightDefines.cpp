#include "LightDefines.h"
#include "cave_story.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>

//These arrays are way bigger than need be
LightingData gNPCLighting[NPC_MAX];
LightingData gCaretLighting[CARET_MAX];
LightingData gMyCharLighting;
LightingData gBulletLighting[BULLET_MAX];
LightingData gBossLighting[BOSS_MAX];
void SetDynamicLightingData(LightBlitType type, int id, int r, int g, int b, int a);
LightBlitType GetLightBlitType(const std::string& str){
    std::string lbt = str.substr(0, str.find('_'));
    if (lbt == "NPC"){
        return LBT_NPC;
    }
    else if(lbt == "MYCHAR")
    {
        return LBT_PLAYER;
    }
    else if(lbt == "BULLET")
    {
        return LBT_BULLET;
    }
    else if(lbt == "CARET")
    {
        return LBT_CARET;
    }
    else if (lbt == "BOSS")
    {
        return LBT_BOSS;
    }

    return LBT_PLAYER;
}
template<typename T2>
inline T2 lexical_cast(const std::string &in) {
    T2 out;
    std::stringstream ss;
    if(in.substr(0, 2) == "0x"){
        ss << std::hex;
        ss << in.substr(2, in.length());
    }else{
        ss << in;
    }
    ss >> out;
    return out;
}
void GetLightTypeId(const std::string& str, LightBlitType* type, int* id){
    *type = GetLightBlitType(str);
    *id = lexical_cast<int>(str.substr(str.find('_') + 1, str.length() ));
}

void GetLightingData(const std::string& str, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a, float* size){
    *r = lexical_cast<int>("0x" + str.substr(1, 2));
    *g = lexical_cast<int>("0x" + str.substr(3, 2));
    *b = lexical_cast<int>("0x" + str.substr(5, 2));

    *a = lexical_cast<int>("0x" + str.substr(9, 2));

    *size = lexical_cast<float>(str.substr(13, 10));
}
void LoadLightingData() {
    std::ifstream infile("data/lightdefs.txt");
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] != '#' && line[0] != '\0') {
            std::string key = line.substr(0, line.find(" = "));
            std::string value = line.substr(line.find(" = ") + 3, line.length());
            LightBlitType type;
            int id;
            LightingData* data;
            GetLightTypeId(key, &type, &id);

            uint8_t r, g, b, a;
            float size;
            GetLightingData(value, &r, &g, &b, &a, &size);
            data = GetLightDataArray(type, id);
            if(data == NULL){
                throw std::exception();
            }

            data->r = r;
            data->g = g;
            data->b = b;
            data->a = a;
            data->size = size;
            data->exists = TRUE;

        }
    }
}
int HexToInt(char c){
    switch(c){
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            return 0;

    }
}
LightingData* GetLightDataArray(LightBlitType type, int id){
//    switch (type){
//        case LBT_PLAYER:
//            return &gMyCharLighting;
//    }
    LightingData* data;
    if (type == LBT_NPC)
    {
        data = &gNPCLighting[id];
    }
    else if (type == LBT_PLAYER)
    {
        data = &gMyCharLighting;
    }
    else if (type == LBT_BULLET)
    {
        data = &gBulletLighting[id];
    }
    else if (type == LBT_CARET)
    {
        data = &gCaretLighting[id];
    }
    else if (type == LBT_BOSS) {
        data = &gBossLighting[id];
    }
    else
    {
        return NULL;
    }
    return data;
}

