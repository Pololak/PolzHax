#pragma once
#include "pch.h"

namespace GameObject {
	inline ccColor3B(__thiscall* getEditorColor)(gd::GameObject*);
	ccColor3B __fastcall getEditorColorH(gd::GameObject*);

	inline bool(__thiscall* shouldBlendColor)(gd::GameObject*);
	bool __fastcall shouldBlendColorH(gd::GameObject*);

	inline void(__thiscall* playShineEffect)(gd::GameObject*);
	void __fastcall playShineEffectH(gd::GameObject*);

	void mem_init();
}

namespace RingObject {
	inline void(__thiscall* spawnCircle)(gd::RingObject*);
	void __fastcall spawnCircleH(gd::RingObject*);

	void mem_init();
}