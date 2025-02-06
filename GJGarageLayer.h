#pragma once
#include "pch.h"

namespace GJGarageLayer {
	inline bool(__thiscall* init_)(gd::GJGarageLayer*);
	bool __fastcall init_H(gd::GJGarageLayer* self);

	inline void(__thiscall* setupIconSelect)(gd::GJGarageLayer*);
	void __fastcall setupIconSelectH(gd::GJGarageLayer* self);

	inline void(__thiscall* selectPage)(gd::GJGarageLayer*, int);
	void __fastcall selectPageH(gd::GJGarageLayer* self, void* edx, int iconType);

	inline void(__thiscall* backButtonMenu)();
	void __fastcall backButtonMenuH();

	class Callback {
	public:
		void onDartIcon(CCObject*);
		void onNext(CCObject*);
		void onPrev(CCObject*);
		void onGarageRopeBack(CCObject*);
	};

	void switchToPage(int page);
	
	void mem_init();
}

struct Fields {
	gd::CCMenuItemToggler* m_dartBtn = nullptr;
	CCSprite* m_dartSelector = nullptr;
	CCMenu* m_dartPage = nullptr;
	CCMenu* m_arrowMenu = nullptr;

	int m_wavePage = 0;
	int m_totalWavePages = 0;
};
