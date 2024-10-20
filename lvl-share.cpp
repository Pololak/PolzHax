#include "lvl-share.hpp"
#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <gd.h>
#include "utils.hpp"
#include <fstream>
#include <matplist.hpp>
#include <nfd.h>
#include <cocos2d.h>

using namespace cocos2d;


template <class S>
void dump_level(gd::GJGameLevel* level, S& stream) {
	const auto song_key = level->songID() ?
		format("<k>k45</k><i>{}</i>", level->songID()) :
		level->m_audioTrack ?
		format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";
	// encode it twice because gdshare does too
	// why? i dont know fod is stupid
	const auto encoded_desc = base64_encode(base64_encode(level->m_levelDesc.sv()));
	format_to(stream, R"(<d>
	<k>kCEK</k><i>4</i>
	<k>k2</k><s>{}</s>
	<k>k3</k><s>{}</s>
	<k>k4</k><s>{}</s>
	{}
	<k>k13</k><t/>
	<k>k21</k><i>2</i>
	<k>k50</k><i>24</i>
</d>)", level->m_levelName.sv(), encoded_desc, level->m_levelString.sv(), song_key);
}

template <class S>
gd::GJGameLevel* import_level(S& stream) {
	auto data = matplist::parse(stream);
	auto level = gd::GameLevelManager::sharedState()->createNewLevel();
	for (const auto& [key, var] : data) {
		if (!std::holds_alternative<matplist::Value>(var)) continue;
		matplist::Value value = std::get<matplist::Value>(var);
		using namespace std::literals;
		if (key == "k2"sv) {
			level->m_levelName = value.value;
		}
		else if (key == "k3"sv) {
			level->m_levelDesc = base64_decode(base64_decode(value.value));
		}
		else if (key == "k4"sv) {
			level->m_levelString = value.value;
		}
		else if (key == "k8"sv) {
			const auto text = value.value;
			level->m_audioTrack = std::stoi(text);
		}
		else if (key == "k45"sv) {
			const auto text = value.value;
			level->songID() = std::stoi(text);
		}
	}
	return level;
}

bool EditLevelLayer_init(gd::EditLevelLayer* self, gd::GJGameLevel* level) {
	if (!matdash::orig<&EditLevelLayer_init>(self, level)) return false;


	auto director = CCDirector::sharedDirector();

	auto menu = CCMenu::create();

	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto* const level = reinterpret_cast<gd::EditLevelLayer*>(self)->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			gd::FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 320.f, false, 0)->show();
		}
		};

	auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!btn_spr->initWithFile("BE_Export_File.png")) {
		btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, to_handler<SEL_MenuHandler, handler>);

	menu->setZOrder(1);
	menu->setPosition({ director->getScreenRight() - 29.5f, director->getScreenBottom() + 109 });
	menu->addChild(button);
	self->addChild(menu);

	return true;
}

bool LevelBrowserLayer_init(gd::LevelBrowserLayer* self, gd::GJSearchObject* obj) {
	if (!matdash::orig<&LevelBrowserLayer_init>(self, obj)) return false;

	if (obj->m_type == gd::SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(CCDirector::sharedDirector()->getWinSize().width - 30.f, 90);
		self->addChild(menu);
		constexpr auto handler = [](CCObject*, CCObject*) {
			nfdchar_t* path = nullptr;
			if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
				std::ifstream file(path);
				auto* const level = import_level(file);
				free(path);
				if (!level) {
					gd::FLAlertLayer::create(nullptr, "Error", "Failed to import", "OK", nullptr, 320.f, false, 0)->show();
					return;
				}
				auto scene = gd::EditLevelLayer::scene(level);
				CCDirector::sharedDirector()->pushScene(scene);
			}
			};

		auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
		if (!btn_spr->initWithFile("BE_Import_File.png")) {
			btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
		}

		auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, to_handler<SEL_MenuHandler, handler>);

		// auto button = CCMenuItemSpriteExtra::create(
		// 	ButtonSprite::create("import", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
		// 	nullptr, self, to_handler<SEL_MenuHandler, handler>
		// );
		menu->setZOrder(1);
		menu->addChild(button);
	}

	return true;
}

bool LevelInfoLayer_init(gd::LevelInfoLayer* self, gd::GJGameLevel* level) {
	if (!matdash::orig<&LevelInfoLayer_init>(self, level)) return false;

	auto director = CCDirector::sharedDirector();

	auto menu = from<CCMenu*>(self, 0x138);

	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto* const level = reinterpret_cast<gd::LevelInfoLayer*>(self)->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			gd::FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 320.f, false, 0)->show();
		}
	};

	auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!btn_spr->initWithFile("BE_Export_File.png")) {
		btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, to_handler<SEL_MenuHandler, handler>);


	//menu->setZOrder(1);
	button->setPosition({ -254, 24 });
	menu->addChild(button);
	//self->addChild(menu);

	return true;
}

void lvl_share::init() {
	matdash::add_hook<&EditLevelLayer_init>(gd::base + 0x3b5a0);
	matdash::add_hook<&LevelBrowserLayer_init>(gd::base + 0x89590);
	matdash::add_hook<&LevelInfoLayer_init>(gd::base + 0x9bc10);
}