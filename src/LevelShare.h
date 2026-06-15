#pragma once
#include "include.h"

namespace LevelShare {
    bool exportLevel(GJGameLevel*, std::string, bool replace = false);
    bool importLevel(std::string);
}