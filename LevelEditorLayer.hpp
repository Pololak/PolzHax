#pragma once
#include "pch.h"

struct GDColor {
	uint8_t r, g, b;
	bool blending = false;

	GDColor() {}
	constexpr GDColor(uint8_t r, uint8_t g, uint8_t b, bool blending) : r(r), g(g), b(b), blending(blending) {}
	constexpr GDColor(const ccColor3B color, bool blending = false) : r(color.r), g(color.g), b(color.b), blending(blending) {}
	GDColor(gd::GameObject* object) : GDColor(object->m_triggerColor, object->m_triggerBlending) {}
	GDColor(gd::SettingsColorObject* color) : GDColor(color->m_color, color->m_blending) {}
	operator ccColor3B() const { return { r, g, b }; }

	bool operator==(const GDColor& other) { return std::tie(r, g, b, blending) == std::tie(other.r, other.g, other.b, other.blending); }
};

namespace LevelEditorLayer {
	inline std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges;

	inline bool(__thiscall* init)(gd::LevelEditorLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::LevelEditorLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* addSpecial)(gd::LevelEditorLayer*, gd::GameObject*);
	void __fastcall addSpecialH(gd::LevelEditorLayer*, void*, gd::GameObject*);

	inline void(__thiscall* removeSpecial)(gd::LevelEditorLayer*, gd::GameObject*);
	void __fastcall removeSpecialH(gd::LevelEditorLayer*, void*, gd::GameObject*);

	inline void(__thiscall* removeObject)(gd::LevelEditorLayer*, gd::GameObject*, bool);
	void __fastcall removeObjectH(gd::LevelEditorLayer*, void*, gd::GameObject*, bool);

	inline void(__thiscall* updateVisibility)(gd::LevelEditorLayer*, float);
	void __fastcall updateVisibilityH(gd::LevelEditorLayer*, void*, float);

	inline gd::GameObject*(__thiscall* addObjectFromString)(gd::LevelEditorLayer*, std::string);
	gd::GameObject* __fastcall addObjectFromStringH(gd::LevelEditorLayer*, void*, std::string);

	inline gd::GameObject*(__thiscall* createObject)(gd::LevelEditorLayer*, int, CCPoint);
	gd::GameObject* __fastcall createObjectH(gd::LevelEditorLayer*, void*, int, CCPoint);

	inline void(__thiscall* flipGravity)(gd::LevelEditorLayer*, gd::PlayerObject*, bool, bool);
	void __fastcall flipGravityH(gd::LevelEditorLayer*, void*, gd::PlayerObject*, bool, bool);

	inline void(__thiscall* onPlaytest)(gd::LevelEditorLayer*);
	void __fastcall onPlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* onResumePlaytest)(gd::LevelEditorLayer*);
	void __fastcall onResumePlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* onPausePlaytest)(gd::LevelEditorLayer*);
	void __fastcall onPausePlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* onStopPlaytest)(gd::LevelEditorLayer*);
	void __fastcall onStopPlaytestH(gd::LevelEditorLayer*);

	inline void(__thiscall* update)(gd::LevelEditorLayer*, float);
	void __fastcall updateH(gd::LevelEditorLayer*, void*, float);

	inline void(__thiscall* pushButton)(gd::LevelEditorLayer*, int, bool);
	void __fastcall pushButtonH(gd::LevelEditorLayer*, void*, int, bool);

	inline void(__thiscall* releaseButton)(gd::LevelEditorLayer*, int, bool);
	void __fastcall releaseButtonH(gd::LevelEditorLayer*, void*, int, bool);

	inline void(__thiscall* playMusic)(gd::LevelEditorLayer*);
	void __fastcall playMusicH(gd::LevelEditorLayer*);

	inline void(__thiscall* draw)(gd::LevelEditorLayer*);
	void __fastcall drawH(gd::LevelEditorLayer*);

	inline void(__thiscall* addToSection)(gd::LevelEditorLayer*, gd::GameObject*);
	void __fastcall addToSectionH(gd::LevelEditorLayer*, void*, gd::GameObject*);

	inline void(__thiscall* destructor)(gd::LevelEditorLayer*);
	void __fastcall destructorH(gd::LevelEditorLayer*);

	void updatePreviewMode();
	void resetColors();
	bool getIsEditorPaused();
	void setIsEditorPaused(bool);
	bool isColorBlending(gd::GJCustomColorMode);
	void setLastPos(float);
	void moveTrigger(gd::GameObject*);
	void removeTrigger(gd::GameObject*);
	void insertTrigger(gd::GameObject*);
	void updateOrientedHitboxes(gd::LevelEditorLayer*);
	void createGroundLayer();
	void removeGroundLayer();
	void updateGroundWidth();

	void updateShowHitboxes();

	gd::LevelEditorLayer* get();
	gd::StartPosObject* getPlaytestStartPos();
	void setPlaytestStartPos(gd::StartPosObject* val);
	GDColor getColor01();
	GDColor getColor02();
	GDColor getColor03();
	GDColor getColor04();
	GDColor getColor3DL();
	unsigned int getCurrentFrame();

	class LevelEditorLayerExt : public gd::LevelEditorLayer {
	public:
		void updateGroundWidth(float);
	};

	void mem_init();
}