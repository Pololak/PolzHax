#include "CustomizeObjectLayer.hpp"
#include "EditorUI.hpp"

bool __fastcall CustomizeObjectLayer::initH(gd::CustomizeObjectLayer* self, void*, gd::GameObject* object, CCArray* objects) {
	if (!CustomizeObjectLayer::init(self, object, objects)) return false;

	auto on3DLSpr = gd::ButtonSprite::create("3D-Line", 48, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	self->m_buttonsArray->addObject(on3DLSpr);
	auto on3DL = gd::CCMenuItemSpriteExtra::create(on3DLSpr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	on3DL->setPosition(80.f, 5.f);
	self->m_buttonMenu->addChild(on3DL, 0, 8);

	auto onWhiteSpr = gd::ButtonSprite::create("White", 48, 0, .4f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
	self->m_buttonsArray->addObject(onWhiteSpr);
	auto onWhite = gd::CCMenuItemSpriteExtra::create(onWhiteSpr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	onWhite->setPosition(-80.f, 5.f);
	self->m_buttonMenu->addChild(onWhite, 0, 9);

	// decomp of highlighting buttons, works perfectly lol, taken from libcocos2dcpp.so 1.93
	int iVar1 = 0; // objectColor
	int iVar19 = 0; // compareColor?
	gd::ButtonSprite* pCVar5 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(0)); // P-Col 1
	gd::ButtonSprite* pCVar6 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(1)); // P-Col 2
	gd::ButtonSprite* pCVar8 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(2)); // Light BG
	gd::ButtonSprite* pCVar10 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(3)); // Col 1
	gd::ButtonSprite* pCVar12 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(4)); // Col 2
	gd::ButtonSprite* pCVar14 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(5)); // Col 3
	gd::ButtonSprite* pCVar16 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(6)); // Col 4
	gd::ButtonSprite* pCVar4 = static_cast<gd::ButtonSprite*>(self->m_buttonsArray->objectAtIndex(7)); // Default

	if (self->m_targetObject == nullptr) {
		iVar1 = -1;
		for (int i = 0; i < self->m_targetObjects->count(); i++) {
			auto object = reinterpret_cast<gd::GameObject*>(self->m_targetObjects->objectAtIndex(i));
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

void __fastcall CustomizeObjectLayer::destructorH(gd::CustomizeObjectLayer* self) {
	CustomizeObjectLayer::destructor(self);

	if (EditorUI::get()) {
		EditorUI::updateObjectInfoLabel(EditorUI::get());
	}
}

void CustomizeObjectLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2dc70), CustomizeObjectLayer::initH, reinterpret_cast<void**>(&CustomizeObjectLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x2daf0), CustomizeObjectLayer::destructorH, reinterpret_cast<void**>(&CustomizeObjectLayer::destructor));
}