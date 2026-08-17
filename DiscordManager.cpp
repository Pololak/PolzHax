#include "DiscordManager.hpp"
#include "utils.hpp"

void DiscordManager::start() {
	if (m_initialized) return;
	m_initialized = true;

	rpcStartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	DiscordEventHandlers handlers;
	std::memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize("1538236384371015690", &handlers, 1, nullptr);

	PresenceSettings settings;
	this->updatePresence(settings);

	m_thread = std::thread([this]() {
		while (m_initialized) {
			Discord_RunCallbacks();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	});
}

void DiscordManager::stop() {
	if (!m_initialized) return;
	m_initialized = false;
	if (m_thread.joinable()) m_thread.join();
	Discord_ClearPresence();
}

void DiscordManager::updatePresence(PresenceSettings settings) {
	if (!m_initialized) return;

	DiscordRichPresence presence;
	std::memset(&presence, 0, sizeof(presence));

	presence.state = settings.state.empty() ? "Browsing menus" : settings.state.c_str();
	presence.details = settings.details.empty() ? std::string("Playing on " + getExeName() + " (1.9)").c_str() : settings.details.c_str();

	presence.largeImageKey = settings.largeImageKey.empty() ? "gd_logo" : settings.largeImageKey.c_str();
	presence.largeImageText = settings.largeImageText.empty() ? "Geometry Dash" : settings.largeImageText.c_str();
	presence.smallImageKey = settings.smallImageKey.empty() ? "ph_logo" : settings.smallImageKey.c_str();
	presence.smallImageText = settings.smallImageText.empty() ? "PolzHax" : settings.smallImageText.c_str();
	presence.startTimestamp = rpcStartTime;

	Discord_UpdatePresence(&presence);
}

PresenceSettings DiscordManager::settingsForLevel(gd::GJGameLevel* level) {
	PresenceSettings settings;

	if (level->m_autoLevel) {
		settings.smallImageKey = "auto";
		settings.smallImageText = "Auto";
	}
	else if (level->m_demon) {
		settings.smallImageKey = "demon";
		settings.smallImageText = "Demon";
	}
	else {
		switch (level->m_stars) {
		default:
		case 0:
			settings.smallImageKey = "na";
			settings.smallImageText = "NA";
			break;
		case 2:
			settings.smallImageKey = "easy";
			settings.smallImageText = "Easy";
			break;
		case 3:
			settings.smallImageKey = "normal";
			settings.smallImageText = "Normal";
			break;
		case 4:
		case 5:
			settings.smallImageKey = "hard";
			settings.smallImageText = "Hard";
			break;
		case 6:
		case 7:
			settings.smallImageKey = "harder";
			settings.smallImageText = "Harder";
			break;
		case 8:
		case 9:
			settings.smallImageKey = "insane";
			settings.smallImageText = "Insane";
			break;
		}
	}

	if (level->m_levelType == gd::GJLevelType::Saved) {
		settings.details = level->m_levelName + " by " + level->m_userName.c_str() + " ID: " + std::to_string(level->m_levelID);
	}
	else if (level->m_levelType == gd::GJLevelType::Editor) {
		settings.details = "Playing a created level";
		settings.smallImageKey = "hammer";
		settings.smallImageText = "Editor";
	}
	else {
		switch (level->m_difficulty) {
		case 1:
			settings.smallImageKey = "easy";
			settings.smallImageText = "Easy";
			break;
		case 2:
			settings.smallImageKey = "normal";
			settings.smallImageText = "Normal";
			break;
		case 3:
			settings.smallImageKey = "hard";
			settings.smallImageText = "Hard";
			break;
		case 4:
			settings.smallImageKey = "harder";
			settings.smallImageText = "Harder";
			break;
		case 5:
			settings.smallImageKey = "insane";
			settings.smallImageText = "Insane";
			break;
		case 6:
			settings.smallImageKey = "demon";
			settings.smallImageText = "Demon";
			break;
		}
		settings.details = level->m_levelName;
	}

	return settings;
}

PresenceSettings DiscordManager::settingsForEditor(gd::LevelEditorLayer* editorLayer) {
	PresenceSettings settings;

	settings.smallImageKey = "hammer";
	settings.smallImageText = "Editor";

	settings.details = "Working on a " + editorLayer->m_level->m_levelName;
	settings.state = std::to_string(editorLayer->m_objectCount) + " objects";

	if (editorLayer->m_playerState == 2) {
		settings.details = "Playtesting in editor";
	}

	return settings;
}