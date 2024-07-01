#pragma once
#include "pch.h"


class CreatorLayer : public CCLayer {
public:
	static inline bool(__thiscall* init)(CCLayer* self);
	static bool __fastcall hook(CCLayer* self);
	void MenuLayerLayer(CCScene*);
};