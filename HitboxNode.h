#pragma once
#include "pch.h"
;
class HitboxNode : public CCDrawNode {
public:
	static HitboxNode* create();

	void drawCircleObj(gd::GameObject* ob, _ccColor4B col);
	void drawRectObj(gd::GameObject* ob, ccColor4F col);
	void drawOBB2D(gd::OBB2D* obb, ccColor4F col);
	void drawRect(CCRect const& rect, ccColor4F col);
	void drawTriangleObj(gd::GameObject* ob, ccColor4F col);
};