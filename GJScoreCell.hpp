#pragma once
#include "pch.h"

namespace GJScoreCell {
	inline void(__thiscall* loadFromScore)(gd::GJScoreCell*, gd::GJUserScore*);
	void __fastcall loadFromScoreH(gd::GJScoreCell*, void*, gd::GJUserScore*);

	class Callback : public gd::GJScoreCell {
	public:
		void onPlayerScore(CCObject*);
	};

	void mem_init();
}