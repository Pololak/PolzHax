#pragma once
#include "pch.h"

class GarageLayer : public CCLayer {
public:
	static inline bool(__thiscall* init)(CCLayer* self);
	static bool __fastcall hook(CCLayer* self);
	void check(CCObject* sender) {
		std::cout << "something" << std::endl;
	};
};