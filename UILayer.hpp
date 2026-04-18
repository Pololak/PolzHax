#pragma once
#include "pch.h"

namespace UILayer {
	inline bool(__thiscall* init)(gd::UILayer*);
	bool __fastcall initH(gd::UILayer*);

	inline void(__thiscall* keyDown)(gd::UILayer*, enumKeyCodes);
	void __fastcall keyDownH(gd::UILayer*, void*, enumKeyCodes);

	inline void(__thiscall* keyUp)(gd::UILayer*, enumKeyCodes);
	void __fastcall keyUpH(gd::UILayer*, void*, enumKeyCodes);

	class Callback : public gd::UILayer {
	public:
		void onPrevStartPos(CCObject*);
		void onNextStartPos(CCObject*);
	};

	void mem_init();
}