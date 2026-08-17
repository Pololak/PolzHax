#include "DebugModule.hpp"
#include "ColorChannelSprite.hpp"
#include "LevelEditorLayer.hpp"
#include "Setting.hpp"
#include "PlayLayer.hpp"
#include "PolzBot.hpp"

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <string>
#include <vector>
#include <support/zip_support/ZipUtils.h>
#include "json.hpp"
#include <fstream>

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

		auto gsm = gd::GameSoundManager::sharedState();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameSoundManager: 0x%p", gsm);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##GameSoundManager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gsm)->getCString());
		}

		ImGui::Text("Active BG Music: %s", gsm->m_activeBGMusic.c_str());
		ImGui::Checkbox("Preloaded", &gsm->m_preloaded);

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

		ImGui::Text("filePath: %s", fme->m_filePath.c_str());

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

		ImGui::Checkbox("No DrawNode Clear", &setting().m_clearHitboxes);

		ImGui::Checkbox("Fix Slabs Y Offset", &setting().m_fixSlabOffset);

		ImGui::Checkbox("Small Saw Hitbox", &setting().m_smallSawHitbox);
		
		if (ImGui::Button("Reload Sounds")) {
			gd::GameSoundManager::sharedState()->preload();
		}

		if (ImGui::Button("Fade in Music")) {
			gd::GameManager::sharedState()->fadeInMusic();
		}

		if (ImGui::Button("Play effect")) {
			gd::GameSoundManager::playSound("playSound_01.ogg");
		}

		auto mdm = gd::MusicDownloadManager::sharedState();

		static int songID = 0;
		ImGui::InputInt("Song ID", &songID);
		if (mdm->isSongDownloaded(songID)) {
			auto songObject = mdm->getSongInfoObject(songID);
			ImGui::Text("Name: %s", songObject->m_songName.c_str());
			ImGui::Text("Artist: %s", songObject->m_artistName.c_str());
			ImGui::Text("File Size: %.02fMB", songObject->m_fileSize);
		}

		auto pl = gd::GameManager::sharedState()->getPlayLayer();
		if (pl) {
			if (ImGui::CollapsingHeader("Replay Events")) {
				auto& events = PolzBot::m_replayEventsVec;
				for (int i = 0; i < events.size(); i++) {
					if (ImGui::CollapsingHeader(std::string("Event " + std::to_string(i) + " [Frame " + std::to_string(events[i].frame) + "]").c_str())) {
						ImGui::Text("Down: %s", events[i].down ? "true" : "false");
						ImGui::Text("P2: %s", events[i].p2 ? "true" : "false");
						ImGui::Text("Rot: %f", events[i].rotation);
						ImGui::Text("XPos: %f", events[i].xPosition);
						ImGui::Text("YPos: %f", events[i].yPosition);
						ImGui::Text("YVel: %f", events[i].yVelocity);
					}
				}
			}

			if (ImGui::Button("pushButton true")) {
				pl->pushButton(1, true);
			}
			if (ImGui::Button("releaseButton true")) {
				pl->releaseButton(1, true);
			}
			if (ImGui::Button("pushButton false")) {
				pl->pushButton(1, false);
			}
			if (ImGui::Button("releaseButton false")) {
				pl->releaseButton(1, false);
			}

			static bool cl = false;
			if (ImGui::Checkbox("Disable No Collision on Playback", &cl)) {
				if (cl) {
					sequence_patch(gd::base + 0xeb33a, { 0xe9, 0x72, 0xf3, 0xff, 0xff, 0x90 });
				}
				else {
					sequence_patch(gd::base + 0xeb33a, { 0x0f, 0x84, 0x71, 0xf3, 0xff, 0xff });
				}
			}

			ImGui::Text("Level time: %.12f", pl->m_levelTime);

			if (ImGui::CollapsingHeader("Noclip Percentage")) {
				auto noclipRuns = PlayLayer::getNoclipPercentage();
				for (auto run : noclipRuns) {
					ImGui::Text("%.2f%% - %.2f%%", run.first, run.second);
				}
			}

			ImGui::Text("Accuracy: %.2f%%", PlayLayer::getNoclipAccuracy());

			if (ImGui::CollapsingHeader("BG Color Action")) {
				ImGui::Text("Duration: %.2f", pl->m_activeBGColorAction->m_duration);
				ImGui::Text("TimeStamp: %.4f", pl->m_activeBGColorAction->m_timeStamp);
				ImGui::Text("Blend: %s", pl->m_activeBGColorAction->m_blend ? "true" : "false");
			}
		}

		auto editorLayer = LevelEditorLayer::get();
		if (editorLayer) {
			auto editorUI = editorLayer->m_uiLayer;

			ImGui::DragFloat("Grid Size", &setting().m_customEditorGridSize);
			ImGui::DragFloat("EditorUI::m_gridSize", &editorUI->m_gridSize);

			ImGui::Text("Section: %i", editorLayer->sectionForPos(-(editorLayer->m_gameLayer->getPositionX()) / editorLayer->m_gameLayer->getScale() + CCDirector::sharedDirector()->getWinSize().width / 2.f));

			float screenBorderLeft = editorLayer->m_gameLayer->convertToNodeSpace({ director->getScreenLeft(), 0.f }).x;
			float screenBorderRight = editorLayer->m_gameLayer->convertToNodeSpace({ director->getScreenRight(), 0.f }).x;
			ImGui::Text("Screen Borders: %f/%f", screenBorderLeft, screenBorderRight);
		}
	}
	ImGui::End();
}