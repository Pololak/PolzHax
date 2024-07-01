#include "gmdshare.h"

// Fork from Mat's Nice Hacks

template <class S>
void dump_level(GJGameLevel* level, S& stream) {
	const auto song_key = level->m_songID ?
		format("<k>k45</k><i>{}</i>", level->m_songID) :
		level->m_audioTrack ?
		format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";
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
GJGameLevel* import_level(S& stream) {
	auto data = matplist::parse(stream);
	auto level = GameLevelManager::sharedState()->createNewLevel();
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
			level->m_songID = std::stoi(text);
		}
	}
	return level;
}

bool EditLevelLayer_init(EditLevelLayer* self, GJGameLevel* level) {
	if (!matdash::orig<&EditLevelLayer_init>(self, level)) return false;
	auto menu = CCMenu::create();
	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto* const level = reinterpret_cast<EditLevelLayer*>(self)->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 320.f, false, 0)->show();
		}
		};
	auto button = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("export", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
		nullptr, self, to_handler<SEL_MenuHandler, handler>
	);
	menu->setZOrder(1);
	menu->setPosition(35, 69);
	menu->addChild(button);
	self->addChild(menu);
	return true;
}

bool LevelBrowserLayer_init(LevelBrowserLayer* self, GJSearchObject* obj) {
	if (!matdash::orig<&LevelBrowserLayer_init>(self, obj)) return false;

	if (obj->m_type == SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(CCDirector::sharedDirector()->getWinSize().width - 30.f, 85);
		self->addChild(menu);
		constexpr auto handler = [](CCObject*, CCObject*) {
			nfdchar_t* path = nullptr;
			if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
				std::ifstream file(path);
				auto* const level = import_level(file);
				free(path);
				if (!level) {
					FLAlertLayer::create(nullptr, "Error", "Failed to import", "OK", nullptr, 320.f, false, 0)->show();
					return;
				}
				auto scene = EditLevelLayer::scene(level);
				CCDirector::sharedDirector()->pushScene(scene);
			}
			};

		auto button = CCMenuItemSpriteExtra::create(
			ButtonSprite::create("import", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
			nullptr, self, to_handler<SEL_MenuHandler, handler>
		);
		menu->setZOrder(1);
		menu->addChild(button);
	}

	return true;
}

void gmdshare::init() {
	matdash::add_hook<&EditLevelLayer_init>(base + 0x3b5a0);
	matdash::add_hook<&LevelBrowserLayer_init>(base + 0x89590);
};