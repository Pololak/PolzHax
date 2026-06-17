#include "DrawGridLayerHook.h"
#include "../Setting.h"
#include "../utils.h"
#include <unordered_map>

struct EffectObjectCache {
	float m_objectXPos;
	float m_duration;
	float m_durationEndXPos;
};

static std::unordered_map<GameObject*, EffectObjectCache> triggerDurationCache;

float xPosForTriggerDuration(DrawGridLayer* gridLayer, GameObject* object) {
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

void DrawGridLayerHook::drawH(DrawGridLayer* self) {
    DrawGridLayerHook::draw(self);

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
            CCARRAY_FOREACH_B_TYPE(self->m_effectObjects, effectObject, GameObject) {
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

void DrawGridLayerHook::loadTimeMarkersH(DrawGridLayer* self, std::string markers) { // taken from Zmx https://github.com/qimiko/gdps-public/blob/238b71e9f3cd8fdf855556ce4cc7c498f22cf3c0/src/modules/editor.cpp#L23
	DrawGridLayerHook::loadTimeMarkers(self, markers);

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

void DrawGridLayerHook::destructorH(DrawGridLayer* self) {
    DrawGridLayerHook::destructor(self);
    triggerDurationCache.clear();
}

void DrawGridLayerHook::mem_init() {
    HOOK("_ZN13DrawGridLayer4drawEv", DrawGridLayerHook::drawH, DrawGridLayerHook::draw);
    HOOK("_ZN13DrawGridLayer15loadTimeMarkersESs", DrawGridLayerHook::loadTimeMarkersH, DrawGridLayerHook::loadTimeMarkers);
    HOOK("_ZN13DrawGridLayerD0Ev", DrawGridLayerHook::destructorH, DrawGridLayerHook::destructor);
}