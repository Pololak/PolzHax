#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

namespace PolzLevelCell2 {
    std::vector<GJGameLevel*> getSelectedLevels();
}

class PolzLevelCell : public geode::Modify<PolzLevelCell, LevelCell> {
public:
    void loadLocalLevelCell();
    void loadCustomLevelCell();

    void onSelectLevel(CCObject*);
};