#pragma once
#include "pch.h"

class MoreOptionsLayer : public CCLayer {
public:
	static inline bool(__thiscall* init)(CCLayer* self);
	static bool __fastcall hook(CCLayer* self);
};