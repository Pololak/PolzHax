#include "DebugModule.hpp"
#include "ColorChannelSprite.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <string>
#include <support/zip_support/ZipUtils.h>

#include "utils.hpp"

void renderDebugModule() {
	ImGui::SetNextWindowSize(ImVec2(300.f * setting().UISize, 300.f * setting().UISize));
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();

		ImGui::Text("Window Resolution: %.0fx%.0f", director->getOpenGLView()->getFrameSize().width, director->getOpenGLView()->getFrameSize().height);
		ImGui::Text("WindowPadding: %f/%f", ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y);
		ImGui::Text("FramePadding: %f/%f", ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y);
		ImGui::Text("ItemSpacing: %f/%f", ImGui::GetStyle().ItemSpacing.x, ImGui::GetStyle().ItemSpacing.y);
		ImGui::Text("CellPadding: %f/%f", ImGui::GetStyle().CellPadding.x, ImGui::GetStyle().CellPadding.y);

		float SHORT_ITEM_WIDTH = (200.f * setting().UISize) / 2.f - ImGui::GetStyle().WindowPadding.x * 1.25f;
		float LONG_ITEM_WIDTH = (200.f * setting().UISize) - ImGui::GetStyle().WindowPadding.x * 2.f;

		ImGui::Text("SHORT_ITEM_WIDTH: %f", SHORT_ITEM_WIDTH);
		ImGui::Text("LONG_ITEM_WIDTH: %f", LONG_ITEM_WIDTH);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameManager: 0x%p", gd::GameManager::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gamemanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::GameManager::sharedState())->getCString());
		}

		auto glm = gd::GameLevelManager::sharedState();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameLevelManager: 0x%p", glm);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gamelevelmanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", glm)->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("LocalLevelManager: 0x%p", gd::LocalLevelManager::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##locallevelmanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::LocalLevelManager::sharedState())->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("CCDirector: 0x%p", CCDirector::sharedDirector());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##ccdirector", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", CCDirector::sharedDirector())->getCString());
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("ObjectToolbox: 0x%p", gd::ObjectToolbox::sharedState());
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##objectToolbox", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gd::ObjectToolbox::sharedState())->getCString());
		}

		auto fme = gd::FMODAudioEngine::sharedEngine();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("FMODAudioEngine: 0x%p", fme);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##fmodaudioengine", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", fme)->getCString());
		}

		auto gjam = gd::GJAccountManager::sharedState();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("GJAccountManager: 0x%p", gjam);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gjAccountManager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gjam)->getCString());
		}

		if (ImGui::CollapsingHeader("Account info (password)")) {
			ImGui::Text("Username: %s", gjam->m_username.c_str());
			ImGui::Text("Password: %s", gjam->m_password.c_str());
			ImGui::Text("AccountID: %i", gjam->m_accountID);
		}

		if (ImGui::Button("Refresh Login")) {
			gjam->loginAccount(gjam->m_username, gjam->m_password);
		}

		ImGui::Text("Pulse 1: %f", fme->m_pulse1);
		ImGui::Text("Pulse 2: %f", fme->m_pulse2);
		ImGui::Text("Pulse 3: %f", fme->m_pulse3);
		ImGui::Text("Pulse Counter: %i", fme->m_pulseCounter);
		ImGui::Text("Metering: %d", fme->m_metering);
		ImGui::Checkbox("m_metering", &fme->m_metering);

		ImGui::Checkbox("No DrawNode Clear", &setting().m_clearHitboxes);

		auto pl = gd::GameManager::sharedState()->getPlayLayer();
		if (pl) {
			static bool cl = false;
			if (ImGui::Checkbox("Disable No Collision on Playback", &cl)) {
				if (cl) {
					sequence_patch(gd::base + 0xeb33a, { 0xe9, 0x72, 0xf3, 0xff, 0xff, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xeb33a, { 0x0f, 0x84, 0x71, 0xf3, 0xff, 0xff });
				}
			}

			ImGui::TextWrapped("m_replayString: %s", pl->m_replayString.c_str());
			
			//ImGui::TextWrapped("IDK: %s", cocos2d::ZipUtils::decompressString(pl->m_level->m_levelString.c_str(), false));
			//ImGui::TextWrapped("IDK: %s", cocos2d::ZipUtils::decompressString(pl->m_level->m_recordString, false));

			if (pl->m_player) {
				ImGui::Text("yVel: %f", pl->m_player->m_yVelocity);
				ImGui::Text("Speed: %f", pl->m_player->m_speed);
				ImGui::Text("Gravity: %f", pl->m_player->m_gravity);
			}

			static float customYVel = 0.f;
			ImGui::SetNextItemWidth(80.f);
			ImGui::DragFloat("yVel", &customYVel);
			ImGui::SameLine();
			if (ImGui::Button("Apply")) {
				pl->m_player->m_yVelocity = customYVel;
			}

			ImGui::Text("PlayLayer 0x174: %i", from<CCArray*>(pl, 0x174)->count());

			static int colorID = 0;
			ImGui::InputInt("ColorID", &colorID);
			if (ImGui::Button("Update Color")) {
				pl->updateColor({0, 0, 0}, 1.f, colorID);
			}

			ImGui::Text("m_fromColor: %i %i %i", pl->m_activeBGColorAction->m_fromColor.r, pl->m_activeBGColorAction->m_fromColor.g, pl->m_activeBGColorAction->m_fromColor.b);
			ImGui::Text("m_toColor: %i %i %i", pl->m_activeBGColorAction->m_toColor.r, pl->m_activeBGColorAction->m_toColor.g, pl->m_activeBGColorAction->m_toColor.b);
		}

		ImGui::Text("VSync: %i", CCApplication::sharedApplication()->getVerticalSyncEnabled());
		if (ImGui::Button("Toggle VSync")) {
			CCApplication::sharedApplication()->toggleVerticalSync(!CCApplication::sharedApplication()->getVerticalSyncEnabled());
		}
		if (ImGui::Button("Update VSync")) {
			CCApplication::sharedApplication()->updateVerticalSync();
		}
		if (ImGui::Button("Setup VSync")) {
			CCApplication::sharedApplication()->setupVerticalSync();
		}

		auto editorLayer = LevelEditorLayer::get();
		if (editorLayer) {
			static int colorID = 0;
			static int idk = 0;
			static int idk2 = 0;

			ImGui::InputInt("ColorID", &colorID);
			ImGui::InputInt("IDK", &idk);
			ImGui::InputInt("IDK2", &idk2);

			if (ImGui::Button("Open ColorSelectPopup")) {
				gd::ColorSelectPopup::create(editorLayer->m_uiLayer->m_selectedObject)->show();
			}

			ImGui::Text("Section: %i", editorLayer->sectionForPos(-(editorLayer->m_gameLayer->getPositionX()) / editorLayer->m_gameLayer->getScale() + CCDirector::sharedDirector()->getWinSize().width / 2.f));

			float screenBorderLeft = editorLayer->m_gameLayer->convertToNodeSpace({ director->getScreenLeft(), 0.f }).x;
			float screenBorderRight = editorLayer->m_gameLayer->convertToNodeSpace({ director->getScreenRight(), 0.f }).x;
			ImGui::Text("Screen Borders: %f/%f", screenBorderLeft, screenBorderRight);
		}

		ImGui::InputText("Favorites", &setting().m_favoritedLevelsIDs);

		auto strings = split(setting().m_favoritedLevelsIDs, ',');
		for (const auto& str : strings) {
			const int i = std::stoi(str);
			ImGui::Text("%i", i);
		}

		for (auto key : CCArrayExt<CCString*>(glm->m_savedLevelsDict->allKeys())) {
			auto level = static_cast<gd::GJGameLevel*>(glm->m_savedLevelsDict->objectForKey(key->getCString()));
			if (level) {
				if (!level->m_levelString.size()) continue;

				auto strings = split(setting().m_favoritedLevelsIDs, ',');
				for (const auto& str : strings) {
					const int i = std::stoi(str);
					if (level->m_levelID == i) {
						ImGui::Text("Name: %s ID: %i", level->m_levelName.c_str(), level->m_levelID);
					}
				}

				
			}
		}

		//auto mdm = gd::MusicDownloadManager::sharedState();

		//static int musicID = 0;
		//ImGui::SetNextItemWidth(120.f);
		//ImGui::InputInt("Song ID", &musicID);

		//if (mdm->isSongDownloaded(musicID)) {
		//	auto songObject = mdm->getSongInfoObject(musicID);
		//	if (songObject) {
		//		ImGui::Text("Song Name: %s", songObject->m_songName);
		//		ImGui::Text("Artist Name: %s", songObject->m_artistName);
		//		ImGui::Text("File size: %f", songObject->m_fileSize);
		//	}
		//}
	}
	ImGui::End();
}