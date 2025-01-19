#pragma once
#include "pch.h"

namespace GJScoreCell {
	inline void(__thiscall* loadFromScore)(gd::GJScoreCell*, gd::GJUserScore*);
	void __fastcall loadFromScoreH(gd::GJScoreCell* self, void*, gd::GJUserScore* userScore);

	class Callback {
	public:
		void onMoreInfo(CCObject*);
	};
}