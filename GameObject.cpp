#include "GameObject.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"
#include "PracticeFix.hpp"
#include "utils.hpp"

void __fastcall GameObject::customSetupH(gd::GameObject* self) {
	GameObject::customSetup(self);

	if (setting().m_smallSawHitbox) {
		if (self->m_objectID == 183) {
			self->m_objectRadius = 15.f;
		}
	}
}

ccColor3B __fastcall GameObject::getEditorColorH(gd::GameObject* self) {
	int color = static_cast<int>(self->m_customColorMode);
	
	switch (color) {
	case 8:
		return ccc3(255, 255, 0);
		break;
	default:
		return GameObject::getEditorColor(self);
		break;
	}
}

bool __fastcall GameObject::shouldBlendColorH(gd::GameObject* self) {
	if (gd::GameManager::sharedState()->getPlayLayer()) {
		return GameObject::shouldBlendColor(self);
	}
	else {
		if (!LevelEditorLayer::get()) return false;

		switch (self->getColorMode()) {
		case gd::GJCustomColorMode::Col1:
		case gd::GJCustomColorMode::Col2:
		case gd::GJCustomColorMode::Col3:
		case gd::GJCustomColorMode::Col4:
		case gd::GJCustomColorMode::Col3DL:
			return LevelEditorLayer::isColorBlending(self->getColorMode());
			break;
		default: break;
		case static_cast<gd::GJCustomColorMode>(9): return false;
		}
		return false;
	}
}

void __fastcall RingObject::spawnCircleH(gd::RingObject* self) {
	if (!setting().onNoOrbRing) RingObject::spawnCircle(self);
}

void __fastcall GameObject::playShineEffectH(gd::GameObject* self) {
	if (!setting().onNoPortalShine) GameObject::playShineEffect(self);
}

void __fastcall GameObject::triggerObjectH(gd::GameObject* self) {
	if (setting().onShowLayout && isColorTrigger(self)) {
		return;
	}

	// this is awful

	//auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

	//ccColor3B origBGColor = ccWHITE;
	//ccColor3B origGColor = ccWHITE;
	//ccColor3B origLineColor = ccWHITE;
	//ccColor3B origObjColor = ccWHITE;
	//ccColor3B orig3DLColor = ccWHITE;
	//ccColor3B orig01Color = ccWHITE;
	//ccColor3B orig02Color = ccWHITE;
	//ccColor3B orig03Color = ccWHITE;
	//ccColor3B orig04Color = ccWHITE;

	//if (playLayer && playLayer->m_testMode && isColorTrigger(self)) {
	//	origBGColor = playLayer->m_backgroundSprite->getColor();
	//	origGColor = playLayer->m_gColorRef->getColor();
	//	origLineColor = playLayer->m_lineColorRef->getColor();
	//	origObjColor = playLayer->m_objColorRef->getColor();
	//	orig3DLColor = playLayer->m_3DLineColorRef->getColor();
	//	orig01Color = playLayer->m_custom01ColorRef->getColor();
	//	orig01Color = playLayer->m_custom02ColorRef->getColor();
	//	orig01Color = playLayer->m_custom03ColorRef->getColor();
	//	orig01Color = playLayer->m_custom04ColorRef->getColor();
	//}

	GameObject::triggerObject(self);

	//if (playLayer && playLayer->m_testMode && isColorTrigger(self)) {
	//	
	//}
}

//void __fastcall GameObject::activatedByPlayerH(gd::GameObject* self, void*, gd::GameObject* p0) {
//	GameObject::activatedByPlayer(self, p0);
//
//	std::cout << "GameObject::activatedByPlayer()" << std::endl;
//	std::cout << self << std::endl;
//	std::cout << std::boolalpha << self->m_hasBeenActivatedP1 << std::endl;
//	std::cout << p0 << std::endl;
//	std::cout << std::boolalpha << p0->m_hasBeenActivatedP1 << std::endl;
//}

CCRect* __fastcall GameObject::getObjectRectH(gd::GameObject* self, void*, CCRect* rect, float x, float y) {
	auto ret = GameObject::getObjectRect(self, rect, x, y);

	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	auto editorLayer = LevelEditorLayer::get();

	if (setting().onHitboxMultiplier && ((playLayer && ((playLayer->m_player != self) && (playLayer->m_player2 != self))) || (editorLayer && ((editorLayer->m_player != self) && (editorLayer->m_player2 != self))))) {
		switch (self->m_objectType) {
		case gd::GameObjectType::Hazard:
			if (self->m_objectRadius <= 0.f) {
				ret->origin.x += ret->size.width / 2.f;
				ret->origin.y += ret->size.height / 2.f;
				ret->size.width *= setting().hazardHitboxesMult;
				ret->size.height *= setting().hazardHitboxesMult;
				ret->origin.x += ret->size.width / 2.f;
				ret->origin.y += ret->size.height / 2.f;
			}
			break;
		default: break;
		}
	}

	return ret;
}

CCRect* __fastcall GameObject::getObjectRect2H(gd::GameObject* self, void*, CCRect* rect, float x, float y) {
	auto ret = GameObject::getObjectRect2(self, rect, x, y);

	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	auto editorLayer = LevelEditorLayer::get();

	if (setting().onHitboxMultiplier && ((playLayer && ((playLayer->m_player != self) && (playLayer->m_player2 != self))) || (editorLayer && ((editorLayer->m_player != self) && (editorLayer->m_player2 != self))))) {
		switch (self->m_objectType) {
		case gd::GameObjectType::Hazard:
			if (self->m_objectRadius <= 0.f) {
				ret->origin.x += ret->size.width / 2.f;
				ret->origin.y += ret->size.height / 2.f;
				ret->size.width *= setting().hazardHitboxesMult;
				ret->size.height *= setting().hazardHitboxesMult;
				ret->origin.x += ret->size.width / 2.f;
				ret->origin.y += ret->size.height / 2.f;
			}
			break;
		default: break;
		}
	}

	return ret;
}

float __fastcall GameObject::getRadiusH(gd::GameObject* self) {


	return GameObject::getRadius(self);
}

void GameObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ee50), GameObject::customSetupH, reinterpret_cast<void**>(&GameObject::customSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x756b0), GameObject::getEditorColorH, reinterpret_cast<void**>(&GameObject::getEditorColor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ece0), GameObject::shouldBlendColorH, reinterpret_cast<void**>(&GameObject::shouldBlendColor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x736e0), GameObject::playShineEffectH, reinterpret_cast<void**>(&GameObject::playShineEffect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6e230), GameObject::triggerObjectH, reinterpret_cast<void**>(&GameObject::triggerObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x75980), GameObject::activatedByPlayerH, reinterpret_cast<void**>(&GameObject::activatedByPlayer));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x722e0), GameObject::getObjectRectH, reinterpret_cast<void**>(&GameObject::getObjectRect));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x723b0), GameObject::getObjectRect2H, reinterpret_cast<void**>(&GameObject::getObjectRect2));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x52960), GameObject::getRadiusH, reinterpret_cast<void**>(&GameObject::getRadius));
}

void RingObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf6d40), RingObject::spawnCircleH, reinterpret_cast<void**>(&RingObject::spawnCircle));
}