#include "GameObject.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"
#include "PracticeFix.hpp"

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
	if (setting().onShowLayout) {
		switch (self->m_objectID) {
		case 29: case 30: case 104: case 105: case 744: case 221: case 717: case 718: case 743:
			return;
		}
	}
	GameObject::triggerObject(self);
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

	return GameObject::getObjectRect(self, rect, x, y);
}

void GameObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ee50), GameObject::customSetupH, reinterpret_cast<void**>(&GameObject::customSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x756b0), GameObject::getEditorColorH, reinterpret_cast<void**>(&GameObject::getEditorColor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6ece0), GameObject::shouldBlendColorH, reinterpret_cast<void**>(&GameObject::shouldBlendColor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x736e0), GameObject::playShineEffectH, reinterpret_cast<void**>(&GameObject::playShineEffect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6e230), GameObject::triggerObjectH, reinterpret_cast<void**>(&GameObject::triggerObject));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x75980), GameObject::activatedByPlayerH, reinterpret_cast<void**>(&GameObject::activatedByPlayer));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x722e0), GameObject::getObjectRectH, reinterpret_cast<void**>(&GameObject::getObjectRect));
}

void RingObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf6d40), RingObject::spawnCircleH, reinterpret_cast<void**>(&RingObject::spawnCircle));
}