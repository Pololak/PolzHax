#pragma once
#include "pch.h"

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer* self, gd::GJGameLevel* level);
	bool __fastcall init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level);

	inline bool(__thiscall* update)(gd::PlayLayer* self, float dt);
	void __fastcall update_H(gd::PlayLayer* self, void*, float dt);

	inline bool(__thiscall* resume)(CCLayer* self);
	bool __fastcall resume_H(CCLayer* self);

	inline bool(__thiscall* onQuit)(CCNode* self);
	void __fastcall onQuit_H(CCNode* self);

	void mem_init();
}
