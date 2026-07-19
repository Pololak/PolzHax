#pragma once
#include "pch.h"

namespace GameObject {
	inline void(__thiscall* customSetup)(gd::GameObject*);
	void __fastcall customSetupH(gd::GameObject*);

	inline ccColor3B(__thiscall* getEditorColor)(gd::GameObject*);
	ccColor3B __fastcall getEditorColorH(gd::GameObject*);

	inline bool(__thiscall* shouldBlendColor)(gd::GameObject*);
	bool __fastcall shouldBlendColorH(gd::GameObject*);

	inline void(__thiscall* playShineEffect)(gd::GameObject*);
	void __fastcall playShineEffectH(gd::GameObject*);

	inline void(__thiscall* triggerObject)(gd::GameObject*);
	void __fastcall triggerObjectH(gd::GameObject*);

	inline void(__thiscall* activatedByPlayer)(gd::GameObject*, gd::GameObject*);
	void __fastcall activatedByPlayerH(gd::GameObject*, void*, gd::GameObject*);

	void mem_init();
}

namespace RingObject {
	inline void(__thiscall* spawnCircle)(gd::RingObject*);
	void __fastcall spawnCircleH(gd::RingObject*);

	void mem_init();
}