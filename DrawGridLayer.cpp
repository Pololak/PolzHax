#include "DrawGridLayer.hpp"
#include "Setting.hpp"

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

void __fastcall DrawGridLayer::drawH(gd::DrawGridLayer* self) {
	//if (setting().onDurationLines) {
	//	if (self->m_effectObjects->count() != 0) {
	//		for (int i = 0; i < self->m_effectObjects->count(); i++) {
	//			auto effectObject = reinterpret_cast<gd::GameObject*>(self->m_effectObjects->objectAtIndex(i));
	//			if (effectObject) {
	//				auto triggerDuration = self->xPosForTime(self->timeForXPos(effectObject->getPositionX()) + effectObject->m_triggerDuration);

	//				glLineWidth(2);
	//				ccDrawColor4B(100, 100, 100, 75);
	//				switch (effectObject->m_objectID) {
	//					case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743: {
	//						if (effectObject->m_triggerDuration > 0) {
	//							ccDrawLine(effectObject->getPosition(), { triggerDuration, effectObject->getPositionY() });
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	DrawGridLayer::draw(self);

	if (setting().onDurationLines) {
		if (self->m_effectObjects->count() != 0) {
			for (int i = 0; i < self->m_effectObjects->count(); i++) {
				auto effectObject = reinterpret_cast<gd::GameObject*>(self->m_effectObjects->objectAtIndex(i));
				if (effectObject) {
					auto triggerDuration = self->xPosForTime(self->timeForXPos(effectObject->getPositionX()) + effectObject->m_triggerDuration);

					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glLineWidth(2);
					ccDrawColor4B(100, 100, 100, 75);
					switch (effectObject->m_objectID) {
					case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743: {
						if (effectObject->m_triggerDuration > 0) {
							ccDrawLine(effectObject->getPosition(), { triggerDuration, effectObject->getPositionY() });
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

	auto startSpeed = self->m_levelEditorLayer->m_levelSettings->m_startSpeed;

	switch (startSpeed) {
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

void DrawGridLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93710), DrawGridLayer::drawH, reinterpret_cast<void**>(&DrawGridLayer::draw));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x93030), DrawGridLayer::loadTimeMarkersH, reinterpret_cast<void**>(&DrawGridLayer::loadTimeMarkers));
    //loadDrawGridLayerMidHook();
}