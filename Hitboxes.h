#pragma once
#include "pch.h"
#include "HitboxNode.h"

namespace Hitboxes {
	void drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col);
	void drawTriangleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col);
	void drawRectObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col);
	void drawCircleObj(CCDrawNode* drawer, gd::GameObject* ob, ccColor4F col);
	void drawPlayerHitbox(gd::PlayerObject* player, CCDrawNode* drawNode);
	void drawSolidsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode);
	void drawObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode);
	void drawHazardsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode);
	void drawSpecialsObjectHitbox(gd::GameObject* obj, CCDrawNode* drawNode);
}