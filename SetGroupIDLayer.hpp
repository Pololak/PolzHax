#pragma once
#include "pch.h"

namespace SetGroupIDLayer {
	inline bool(__thiscall* init)(gd::SetGroupIDLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::SetGroupIDLayer*, void*, gd::GameObject*, CCArray*);

	inline void(__thiscall* updateGroupID)(gd::SetGroupIDLayer*);
	void __fastcall updateGroupIDH(gd::SetGroupIDLayer*);

	class Callback : public gd::SetGroupIDLayer {
	public:
		void onCurrentGroup(CCObject*);
		void onNextFree(CCObject*);
	};

	void mem_init();
}