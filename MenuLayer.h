#pragma once
#include "pch.h"

namespace MenuLayer {
	inline bool(__thiscall* init)(gd::MenuLayer* self);
	bool __fastcall init_H(gd::MenuLayer* self, void* edx);

	class Callback : public gd::MenuLayer {
	public:
		void onReload(CCObject*);
		//void onSmallEditorStep(CCObject*);
	};

	void mem_init();
}

//namespace GJMoreGamesLayer {
//	inline CCArray*(__thiscall* getMoreGamesList)(gd::GJMoreGamesLayer*);
//	CCArray* __fastcall getMoreGamesList_H(gd::GJMoreGamesLayer*);
//
//	void mem_init();
//}
