#pragma once
#include "pch.h"

namespace EndLevelLayer {
	inline void(__thiscall* customSetup)(gd::EndLevelLayer*);
	void __fastcall customSetupH(gd::EndLevelLayer*);

	inline void(__thiscall* completeSprite)();
	void __fastcall completeSpriteH();

	class Callback : public gd::EndLevelLayer {
	public:
		void onLastCheckpoint(CCObject*);
		void onHideEndLayer(CCObject*);
		void onShowEndLayer(CCObject*);
	};

	void mem_init();
}