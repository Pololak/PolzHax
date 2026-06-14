#include "GameObjectHook.h"
#include "LevelEditorLayerHook.h"

ccColor3B GameObjectHook::getEditorColorH(GameObject* self) {
    int color = static_cast<int>(self->m_customColorMode);
    switch (color) {
    case 8:
        return ccYELLOW; break;
    default:
        return GameObjectHook::getEditorColor(self); break;
    }
}

bool GameObjectHook::shouldBlendColorH(GameObject* self) {
    if (GameManager::sharedState()->getPlayLayer()) {
        return GameObjectHook::shouldBlendColor(self);
    }
    else {
        if (!LevelEditorLayerHook::get()) return false;

        switch (self->getColorMode()) {
        case GJCustomColorMode::Col1:
        case GJCustomColorMode::Col2:
        case GJCustomColorMode::Col3:
        case GJCustomColorMode::Col4:
        case GJCustomColorMode::Col3DL:
            return LevelEditorLayerHook::isColorBlending(self->getColorMode());
            break;
        default:
            break;
        }

        return false;
    }
}

void GameObjectHook::mem_init() {
    HOOK("_ZN10GameObject14getEditorColorEv", GameObjectHook::getEditorColorH, GameObjectHook::getEditorColor);
    HOOK("_ZN10GameObject16shouldBlendColorEv", GameObjectHook::shouldBlendColorH, GameObjectHook::shouldBlendColor);
}