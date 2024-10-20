#pragma once
#include "pch.h"

namespace MenuLayer {
	inline bool(__thiscall* init)(gd::MenuLayer* self);
	bool __fastcall init_H(gd::MenuLayer* self, void* edx);

	class Callback {
	public:
		void onReload(CCObject*);
		//void onSmallEditorStep(CCObject*);
	};

	void mem_init();
}

namespace MenuGameLayer {
	inline void(__thiscall* tryJump)(gd::MenuGameLayer*, float);
	void __fastcall tryJumpH(gd::MenuGameLayer* self, void* edx, float idk);

	inline void(__thiscall* update)(gd::MenuGameLayer*, float);
	void __fastcall updateH(gd::MenuGameLayer* self, void* edx, float dt);

	void mem_init();
}

//namespace GJMoreGamesLayer {
//	inline CCArray*(__thiscall* getMoreGamesList)(gd::GJMoreGamesLayer*);
//	CCArray* __fastcall getMoreGamesList_H(gd::GJMoreGamesLayer*);
//
//	void mem_init();
//}
