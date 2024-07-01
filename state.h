#pragma once
#include <cocos2d.h>

struct LayersStruct {
    
};


struct SettingStruct {
    bool onNoclip = false;

    float UISize = 1.0f;

    float Overlaycolor[4] = { 0, 0.61, 0.37, 1 };
    float BGcolor[4] = { 0.3, 0.3, 0.3, 1 };
};

SettingStruct& setting();
LayersStruct& layers();