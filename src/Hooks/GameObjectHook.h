#pragma once
#include "../include.h"

namespace GameObjectHook {
    inline ccColor3B(*getEditorColor)(GameObject*);
    ccColor3B getEditorColorH(GameObject*);

    inline bool(*shouldBlendColor)(GameObject*);
    bool shouldBlendColorH(GameObject*);

    void mem_init();
}