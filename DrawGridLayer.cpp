#include "DrawGridLayer.hpp"
#include "Setting.hpp"
#include "utils.hpp"
#include <unordered_map>

//#define MAKE_MIDHOOK(addr, func)        \
//    if (MH_CreateHook(                  \
//        reinterpret_cast<LPVOID>(gd::base + addr),    \
//        reinterpret_cast<LPVOID>(func##_midHook),     \
//        reinterpret_cast<LPVOID*>(&func##_retAddr)    \
//    ) != MH_OK) return false;           \
//                                        \
//    if (MH_EnableHook(                  \
//        reinterpret_cast<LPVOID>(gd::base + addr)     \
//    ) != MH_OK)                         \
//        return false;

struct EffectObjectCache {
	float m_objectXPos;
	float m_duration;
	float m_durationEndXPos;
};

static std::unordered_map<gd::GameObject*, EffectObjectCache> triggerDurationCache;

float xPosForTriggerDuration(gd::DrawGridLayer* gridLayer, gd::GameObject* object) {
	auto objectXPos = object->getPositionX();
	auto triggerDuration = object->m_triggerDuration;

	auto it = triggerDurationCache.find(object);
	if (it != triggerDurationCache.end()) {
		auto& triggerCache = it->second;
		if ((triggerCache.m_objectXPos == objectXPos) && (triggerCache.m_duration == triggerDuration)) {
			return triggerCache.m_durationEndXPos;
		}
	}

	auto res = gridLayer->xPosForTime(gridLayer->timeForXPos(objectXPos) + triggerDuration);
	triggerDurationCache[object] = {
		objectXPos,
		triggerDuration,
		res
	};

	return res;
}

void __fastcall DrawGridLayer::drawH(gd::DrawGridLayer* self) {
	DrawGridLayer::draw(self);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	auto editorLayer = self->m_levelEditorLayer;

	float screenBorderTop = editorLayer->m_gameLayer->convertToNodeSpace({ 0.f, director->getScreenTop()}).y;
	float screenBorderBottom = editorLayer->m_gameLayer->convertToNodeSpace({ 0.f, director->getScreenBottom()}).y;
	float screenBorderLeft = editorLayer->m_gameLayer->convertToNodeSpace({ director->getScreenLeft(), 0.f }).x;
	float screenBorderRight = editorLayer->m_gameLayer->convertToNodeSpace({director->getScreenRight(), 0.f}).x;

	if (setting().onDurationLines) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glLineWidth(2);
		ccDrawColor4B(100, 100, 100, 75);

		if (self->m_effectObjects->count()) {
			for (auto effectObject : CCArrayExt<gd::GameObject*>(self->m_effectObjects)) {
				if (effectObject) {
					switch (effectObject->m_objectID) {
						case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743: {
							if (effectObject->m_triggerDuration > 0.f) {
								float triggerDurationEndPoint = xPosForTriggerDuration(self, effectObject);
								if ((effectObject->getPositionX() < screenBorderRight) && (triggerDurationEndPoint > screenBorderLeft) && (screenBorderBottom < effectObject->getPositionY() < screenBorderTop)) {
									ccDrawLine(effectObject->getPosition(), { triggerDurationEndPoint, effectObject->getPositionY() });
								}
							}
						}
					}
				}
			}
		}
	}
}

void __fastcall DrawGridLayer::loadTimeMarkersH(gd::DrawGridLayer* self, void*, gd::string markers) { // taken from Zmx https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/src/modules/editor.cpp#L23
	DrawGridLayer::loadTimeMarkers(self, markers);

	switch (self->m_levelEditorLayer->m_levelSettings->m_startSpeed) {
	case 0:
	default:
		self->m_guidelineSpacing = self->m_normalGuidelineSpacing;
		break;
	case 1:
		self->m_guidelineSpacing = self->m_slowGuidelineSpacing;
		break;
	case 2:
		self->m_guidelineSpacing = self->m_fastGuidelineSpacing;
		break;
	case 3:
		self->m_guidelineSpacing = self->m_fasterGuidelineSpacing;
		break;
	}
}

//void DrawGridLayer_draw_midHook() {
//    gd::GameObject* obj;
//
//    __asm {
//        mov obj, eax
//    }
//
//    switch (obj->m_objectID) {
//    case 12:  ccDrawColor4B(88, 255, 100, 255); break; // cube
//    case 13:  ccDrawColor4B(255, 150, 255, 255); break; // ship
//    case 47:  ccDrawColor4B(255, 34, 0, 255); break; // ball
//    case 111: ccDrawColor4B(255, 214, 85, 255); break; // ufo
//    case 660: ccDrawColor4B(31, 221, 255, 255); break; // wave
//    }
//}
//
//void (*DrawGridLayer_draw_midHook_retAddr)();
//__declspec(naked) void DrawGridLayer_draw_midHook_midHook() {
//    __asm {
//        pushad
//        pushfd
//        call DrawGridLayer_draw_midHook
//        popfd
//        popad
//        jmp DrawGridLayer_draw_midHook_retAddr
//    }
//}
//
//bool loadDrawGridLayerMidHook() {
//    MAKE_MIDHOOK(0x93f5d, DrawGridLayer_draw_midHook);
//
//    return true;
//}

void __fastcall DrawGridLayer::destructorH(gd::DrawGridLayer* self) {
	DrawGridLayer::destructor(self);
	triggerDurationCache.clear();
}

void DrawGridLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93710), DrawGridLayer::drawH, reinterpret_cast<void**>(&DrawGridLayer::draw));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93030), DrawGridLayer::loadTimeMarkersH, reinterpret_cast<void**>(&DrawGridLayer::loadTimeMarkers));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x92ca0), DrawGridLayer::destructorH, reinterpret_cast<void**>(&DrawGridLayer::destructor));
    //loadDrawGridLayerMidHook();
}