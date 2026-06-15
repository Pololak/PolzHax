#pragma once
#include "../include.h"

namespace EditButtonBarHook {
    inline bool(*init)(EditButtonBar*, CCArray*, CCPoint, int, bool);
    bool initH(EditButtonBar*, CCArray*, CCPoint, int, bool);

    void mem_init();
}