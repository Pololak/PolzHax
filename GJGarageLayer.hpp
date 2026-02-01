#pragma once
#include "pch.h"

namespace GJGarageLayer {
	inline bool(__thiscall* init)(gd::GJGarageLayer*);
	bool __fastcall initH(gd::GJGarageLayer*);

	inline void(__thiscall* setupIconSelect)(gd::GJGarageLayer*);
	void __fastcall setupIconSelectH(gd::GJGarageLayer*);

	inline void(__thiscall* selectPage)(gd::GJGarageLayer*, gd::IconType);
	void __fastcall selectPageH(gd::GJGarageLayer*, void*, gd::IconType);

	inline void(__thiscall* destructor)(gd::GJGarageLayer*);
	void __fastcall destructorH(gd::GJGarageLayer*);

	inline void(__thiscall* iconTypeMenu)();
	void __fastcall iconTypeMenuH();

	void switchToPage(int);

	class Callback : public gd::GJGarageLayer {
	public:
		void onIconPreview(CCObject*);
		void onDartIcon(CCObject*);
		void onNext(CCObject*);
		void onPrev(CCObject*);

		void onGarageRopeBack(CCObject*);
	};

	void mem_init();
}