#pragma once
#include "pch.h"

namespace EndLevelLayer {
	inline void(__thiscall* customSetup)(gd::EndLevelLayer*);
	void __fastcall customSetupH(gd::EndLevelLayer*);

	class Callback : public gd::EndLevelLayer {
	public:
		void onLastCheckpoint(CCObject*);
	};

	void mem_init();
}