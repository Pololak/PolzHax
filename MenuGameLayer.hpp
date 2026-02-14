#pragma once
#include "pch.h"

namespace MenuGameLayer {
	inline void(__thiscall* tryJump)(gd::MenuGameLayer*, float);
	void __fastcall tryJumpH(gd::MenuGameLayer*, void*, float);

	inline bool(__thiscall* ccTouchBegan)(gd::MenuGameLayer*, CCTouch*, CCEvent*);
	bool __fastcall ccTouchBeganH(gd::MenuGameLayer*, void*, CCTouch*, CCEvent*);

	inline void(__thiscall* update)(gd::MenuGameLayer*, float);
	void __fastcall updateH(gd::MenuGameLayer*, void*, float);

	void mem_init();
}