#pragma once
#include "pch.h"

namespace ColorSelectPopup {
	inline bool(__thiscall* init)(gd::ColorSelectPopup*, gd::GameObject*, int, int, int);
	bool __fastcall initH(gd::ColorSelectPopup*, void*, gd::GameObject*, int, int, int);

	inline void(__thiscall* sliderChanged)(gd::ColorSelectPopup*, CCObject*);
	void __fastcall sliderChangedH(gd::ColorSelectPopup*, void*, CCObject*);

	class Callback : public gd::ColorSelectPopup {
	public:
		void onDefault(CCObject*);
		void onPulseHelper(CCObject*);
	};

	void mem_init();
}