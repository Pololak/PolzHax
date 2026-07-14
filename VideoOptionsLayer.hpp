#pragma once
#include "pch.h"

namespace VideoOptionsLayer {
	inline bool(__thiscall* init)(gd::VideoOptionsLayer*);
	bool __fastcall initH(gd::VideoOptionsLayer*);

	class Callback : public gd::VideoOptionsLayer {
	public:
		void onTextures(CCObject*);
	};

	void mem_init();
}