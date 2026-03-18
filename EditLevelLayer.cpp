#include "EditLevelLayer.hpp"
#include "Setting.hpp"
#include "LevelShare.hpp"
#include "nfd.h"
#include <fstream>

bool m_noRotationPass;
CCMenu* m_levelActionsMenu;

void EditLevelLayer::Callback::onExportLevel(CCObject*) {
	if (!this->m_level->m_levelString.size()) {
		gd::FLAlertLayer::create("Error", "Level string is empty!", "OK")->show();
		return;
	}

	nfdchar_t* path = nullptr;
	if (NFD_SaveDialog("gmd", CCFileUtils::sharedFileUtils()->getWritablePath2().c_str(), &path) == NFD_OKAY) {
		std::ofstream file(path);
		dumpLevel(this->m_level, file);
		free(path);
		gd::FLAlertLayer::create("Success", "The level has been saved.", "OK")->show();
	}
}

void EditLevelLayer::Callback::onMoveToTop(CCObject*) {
	auto flalert = gd::FLAlertLayer::create(this, "Move To Top", "Move this level to the top of the created levels list?", "NO", "YES");
	flalert->setTag(0x1b);
	flalert->show();
}

bool __fastcall EditLevelLayer::initH(gd::EditLevelLayer* self, void*, gd::GJGameLevel* level) {
	m_levelActionsMenu = nullptr;
	if (!EditLevelLayer::init(self, level)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	if (m_levelActionsMenu) {
		auto onCopy = static_cast<gd::CCMenuItemSpriteExtra*>(m_levelActionsMenu->getChildren()->objectAtIndex(2));

		auto onMoveToTopSpr = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
		if (!onMoveToTopSpr->initWithFile("GJ_orderUpBtn_001.png")) {
			onMoveToTopSpr->createWithSpriteFrameName("edit_upBtn_001.png");
		}
		onMoveToTopSpr->setScale(.925f);
		auto onMoveToTop = gd::CCMenuItemSpriteExtra::create(onMoveToTopSpr, self, menu_selector(EditLevelLayer::Callback::onMoveToTop));
		onMoveToTop->setPosition(onCopy->getPositionX(), onCopy->getPositionY() - 60.f);

		m_levelActionsMenu->addChild(onMoveToTop);
	}

	auto shareMenu = CCMenu::create();
	shareMenu->setPosition(director->getScreenRight(), director->getScreenBottom());
	self->addChild(shareMenu, 1);

	auto onExportLevelSpr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!onExportLevelSpr->initWithFile("BE_Export_File.png")) {
		onExportLevelSpr->createWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto onExportLevel = gd::CCMenuItemSpriteExtra::create(onExportLevelSpr, self, menu_selector(EditLevelLayer::Callback::onExportLevel));
	onExportLevel->setPosition(-30.f, 30.f);
	shareMenu->addChild(onExportLevel);


	return true;
}

void __fastcall EditLevelLayer::FLAlert_ClickedH(gd::EditLevelLayer* _self, void*, gd::FLAlertLayer* layer, bool btn2) {
	auto self = reinterpret_cast<gd::EditLevelLayer*>(reinterpret_cast<uintptr_t>(_self) - 0x11c);
	if ((layer->getTag() == 0x1a) && btn2) {
		m_noRotationPass = true;
		self->onEdit(nullptr);
		m_noRotationPass = false;
	}
	if ((layer->getTag() == 0x1b) && btn2) {
		gd::LocalLevelManager::sharedState()->m_localLevels->removeObject(self->m_level, true);
		gd::LocalLevelManager::sharedState()->m_localLevels->insertObject(self->m_level, 0);
		gd::LocalLevelManager::sharedState()->updateLevelOrder();
	}
	EditLevelLayer::FLAlert_Clicked(_self, layer, btn2);
}

void __fastcall EditLevelLayer::onEditH(gd::EditLevelLayer* self, void*, CCObject* sender) {
	if (setting().onNoRotation && !m_noRotationPass) {
		auto flalert = gd::FLAlertLayer::create(self, "No Rotation", "You have <cr>No Rotation</c> enabled, this will reset all rotated objects in your level if you save it. Do you still want to continue?", "NO", "YES");
		flalert->setTag(0x1a);
		flalert->show();

	}
	else return EditLevelLayer::onEdit(self, sender);
}

void __fastcall EditLevelLayer::onCloneH(gd::EditLevelLayer* self) {
	EditLevelLayer::onClone(self);

	auto clonedLevel = static_cast<gd::GJGameLevel*>(gd::LocalLevelManager::sharedState()->m_localLevels->objectAtIndex(0));
	clonedLevel->m_songID = self->m_level->m_songID;
}

void __fastcall EditLevelLayer::levelActionsMenuH() {
	__asm {
		mov m_levelActionsMenu, eax
	}
	EditLevelLayer::levelActionsMenu();
}

void __fastcall EditLevelLayer::destructorH(gd::EditLevelLayer* self) {
	EditLevelLayer::destructor(self);
	m_levelActionsMenu = nullptr;
}

void EditLevelLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3b5a0), EditLevelLayer::initH, reinterpret_cast<void**>(&EditLevelLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3d440), EditLevelLayer::onEditH, reinterpret_cast<void**>(&EditLevelLayer::onEdit));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3d910), EditLevelLayer::FLAlert_ClickedH, reinterpret_cast<void**>(&EditLevelLayer::FLAlert_Clicked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3da30), EditLevelLayer::onCloneH, reinterpret_cast<void**>(&EditLevelLayer::onClone));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3b3d0), EditLevelLayer::destructorH, reinterpret_cast<void**>(&EditLevelLayer::destructor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3c065), EditLevelLayer::levelActionsMenuH, reinterpret_cast<void**>(&EditLevelLayer::levelActionsMenu));
}