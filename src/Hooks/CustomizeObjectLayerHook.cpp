#include "CustomizeObjectLayerHook.h"
#include "EditorUIHook.h"

bool CustomizeObjectLayerHook::initH(CustomizeObjectLayer* self, GameObject* object, CCArray* objects) {
    if (!CustomizeObjectLayerHook::init(self, object, objects)) return false;

    auto on3DLSpr = ButtonSprite::create("3D-Line", 48, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	self->m_buttonsArray->addObject(on3DLSpr);
	auto on3DL = CCMenuItemSpriteExtra::create(on3DLSpr, self, menu_selector(CustomizeObjectLayer::onSelectColor));
	on3DL->setPosition(80.f, 5.f);
	self->m_buttonMenu->addChild(on3DL, 0, 8);

	auto onWhiteSpr = ButtonSprite::create("White", 48, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	self->m_buttonsArray->addObject(onWhiteSpr);
	auto onWhite = CCMenuItemSpriteExtra::create(onWhiteSpr, self, menu_selector(CustomizeObjectLayer::onSelectColor));
	onWhite->setPosition(-80.f, 5.f);
	self->m_buttonMenu->addChild(onWhite, 0, 9);

	// decomp of highlighting buttons, works perfectly lol, taken from libcocos2dcpp.so 1.93
	int iVar1 = 0; // objectColor
	int iVar19 = 0; // compareColor?
	ButtonSprite* pCVar5 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(0)); // P-Col 1
	ButtonSprite* pCVar6 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(1)); // P-Col 2
	ButtonSprite* pCVar8 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(2)); // Light BG
	ButtonSprite* pCVar10 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(3)); // Col 1
	ButtonSprite* pCVar12 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(4)); // Col 2
	ButtonSprite* pCVar14 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(5)); // Col 3
	ButtonSprite* pCVar16 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(6)); // Col 4
	ButtonSprite* pCVar4 = static_cast<ButtonSprite*>(self->m_buttonsArray->objectAtIndex(7)); // Default

	if (self->m_targetObject == nullptr) {
		iVar1 = -1;
		for (int i = 0; i < self->m_targetObjects->count(); i++) {
			auto object = reinterpret_cast<GameObject*>(self->m_targetObjects->objectAtIndex(i));
			if (iVar1 == -1) {
				iVar1 = static_cast<int>(object->m_customColorMode);
			}
			else {
				iVar19 = static_cast<int>(object->m_customColorMode);
				if (iVar1 != iVar19) {
					iVar1 = 0;
					goto LAB_0024e170;
				}
			}
		}
		if (iVar1 == -1) {
			iVar1 = 0;
		}
	}
	else {
		iVar1 = static_cast<int>(object->m_customColorMode);
	}
LAB_0024e170:
	switch (iVar1) {
	case 0:
		break;
	case 1:
		pCVar4 = pCVar5;
		break;
	case 2:
		pCVar4 = pCVar6;
		break;
	case 3:
		pCVar4 = pCVar10;
		break;
	case 4:
		pCVar4 = pCVar12;
		break;
	case 5:
		pCVar4 = pCVar8;
		break;
	case 6:
		pCVar4 = pCVar14;
		break;
	case 7:
		pCVar4 = pCVar16;
		break;
	case 8:
		pCVar4 = on3DLSpr;
		break;
	case 9:
		pCVar4 = onWhiteSpr;
		break;
	default:
		goto LAB_0024e1e8;
	}
	self->highlightSelected(pCVar4);
LAB_0024e1e8:
	return true;

	return true;
}

void CustomizeObjectLayerHook::onSelectColorH(CustomizeObjectLayer* self, CCObject* sender) {
	CustomizeObjectLayerHook::onSelectColor(self, sender);

	if (EditorUIHook::get()) {
		EditorUIHook::updateObjectInfoLabel(EditorUIHook::get());
	}
}

void CustomizeObjectLayerHook::mem_init() {
    HOOK("_ZN20CustomizeObjectLayer4initEP10GameObjectPN7cocos2d7CCArrayE", CustomizeObjectLayerHook::initH, CustomizeObjectLayerHook::init);
    HOOK("_ZN20CustomizeObjectLayer13onSelectColorEPN7cocos2d8CCObjectE", CustomizeObjectLayerHook::onSelectColorH, CustomizeObjectLayerHook::onSelectColor);
}