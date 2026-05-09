#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace Hitboxes {
	void drawRect(CCDrawNode* drawer, CCRect const& rect, ccColor4F col);
	void drawTriangleObj(CCDrawNode* drawer, GameObject* ob, ccColor4F col);
	void drawRectObj(CCDrawNode* drawer, GameObject* ob, ccColor4F col);
	void drawCircleObj(CCDrawNode* drawer, GameObject* ob, ccColor4F col);
	void drawPlayerHitbox(PlayerObject* player, CCDrawNode* drawNode);
	void drawSolidsObjectHitbox(GameObject* obj, CCDrawNode* drawNode);
	void drawHazardsObjectHitbox(GameObject* obj, CCDrawNode* drawNode);
	void drawSpecialsObjectHitbox(GameObject* obj, CCDrawNode* drawNode);
}