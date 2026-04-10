#pragma once
#include "pch.h"

namespace MoreSearchLayer {
	inline bool(__thiscall* init)(gd::MoreSearchLayer*);
	bool __fastcall initH(gd::MoreSearchLayer*);

	class Callback : public gd::MoreSearchLayer {
	public:
		void onExtraLong(CCObject*);
	};

	void mem_init();
}