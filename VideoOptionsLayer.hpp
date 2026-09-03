#pragma once
#include "pch.h"

namespace VideoOptionsLayer {
	inline bool(__thiscall* init)(gd::VideoOptionsLayer*);
	bool __fastcall initH(gd::VideoOptionsLayer*);

	inline void(__thiscall* onApply)(gd::VideoOptionsLayer*, CCObject*);
	void __fastcall onApplyH(gd::VideoOptionsLayer*, void*, CCObject*);

	class Callback : public gd::VideoOptionsLayer {
	public:
		void onTextures(CCObject*);
		void onBorderless(CCObject*);
	};

	void mem_init();
}