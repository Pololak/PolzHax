#pragma once
#include "pch.h"

namespace BoomScrollLayer {
	inline bool(__thiscall* init)(gd::BoomScrollLayer*, CCArray*, int, bool, CCArray*, gd::DynamicScrollDelegate*);
	bool __fastcall initH(gd::BoomScrollLayer*, void*, CCArray*, int, bool, CCArray*, gd::DynamicScrollDelegate*);

	inline void(__thiscall* updateDots)(gd::BoomScrollLayer*, float);
	void __fastcall updateDotsH(gd::BoomScrollLayer*, void*, float);

	inline void(__thiscall* destructor)(gd::BoomScrollLayer*);
	void __fastcall destructorH(gd::BoomScrollLayer*);

	void updateButtons(gd::BoomScrollLayer*);
	void addButtons(gd::BoomScrollLayer*);

	class Callback : public gd::BoomScrollLayer {
	public:
		void onDot(CCObject*);
	};

	void mem_init();
}