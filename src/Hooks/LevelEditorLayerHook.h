#pragma once
#include "../include.h"

struct GDColor {
	uint8_t r, g, b;
	bool blending = false;

	GDColor() {}
	constexpr GDColor(uint8_t r, uint8_t g, uint8_t b, bool blending) : r(r), g(g), b(b), blending(blending) {}
	constexpr GDColor(const ccColor3B color, bool blending = false) : r(color.r), g(color.g), b(color.b), blending(blending) {}
	GDColor(GameObject* object) : GDColor(object->m_triggerColor, object->m_triggerBlending) {}
	GDColor(SettingsColorObject* color) : GDColor(color->m_color, color->m_blending) {}
	operator ccColor3B() const { return { r, g, b }; }

	bool operator==(const GDColor& other) { return std::tie(r, g, b, blending) == std::tie(other.r, other.g, other.b, other.blending); }
};

namespace LevelEditorLayerHook {
    LevelEditorLayer* get();

    inline bool(*init)(LevelEditorLayer*, GJGameLevel*);
    bool initH(LevelEditorLayer*, GJGameLevel*);

    inline void(*addSpecial)(LevelEditorLayer*, GameObject*);
    void addSpecialH(LevelEditorLayer*, GameObject*);

    inline void(*removeSpecial)(LevelEditorLayer*, GameObject*);
    void removeSpecialH(LevelEditorLayer*, GameObject*);

    inline void(*updateVisibility)(LevelEditorLayer*, float);
    void updateVisibilityH(LevelEditorLayer*, float);

    inline void(*update)(LevelEditorLayer*, float);
    void updateH(LevelEditorLayer*, float);

    inline GameObject*(*addObjectFromString)(LevelEditorLayer*, std::string);
    GameObject* addObjectFromStringH(LevelEditorLayer*, std::string);

    inline GameObject*(*createObject)(LevelEditorLayer*, int, CCPoint);
    GameObject* createObjectH(LevelEditorLayer*, int, CCPoint);

    inline void(*flipGravity)(LevelEditorLayer*, PlayerObject*, bool, bool);
    void flipGravityH(LevelEditorLayer*, PlayerObject*, bool, bool);

    inline void(*destructor)(LevelEditorLayer*);
    void destructorH(LevelEditorLayer*);

    void updatePreviewMode();
    void resetColors();
    bool getIsEditorPaused();
    void setIsEditorPaused(bool);
    bool isColorBlending(GJCustomColorMode);
    void setLastPos(float);
    void moveTrigger(GameObject*);
    void removeTrigger(GameObject*);
    void insertTrigger(GameObject*);
    void updateOrientedHitboxes(LevelEditorLayer*);
    void createGroundLayer();
    void removeGroundLayer();
    void updateGroundWidth();
    void updateShowHitboxes();

    StartPosObject* getPlaytestStartPos();
    void setPlaytestStartPos(StartPosObject* val);
    GDColor getColor01();
    GDColor getColor02();
    GDColor getColor03();
    GDColor getColor04();
    GDColor getColor3DL();

    void mem_init();
}