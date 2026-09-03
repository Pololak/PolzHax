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

		auto gm = gd::GameManager::sharedState();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("GameManager: 0x%p", gm);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##gamemanager", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", gm)->getCString());
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

		auto egl = CCEGLView::sharedOpenGLView();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("CCEGLView: 0x%p", egl);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - 50.f);
		if (ImGui::Button("Copy##cceglview", ImVec2(50.f, 0.f))) {
			clipboard::write(CCString::createWithFormat("%p", egl)->getCString());
		}
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Main Window: 0x%p, W: %i, H: %i", CCEGLView::sharedOpenGLView()->m_pMainWindow, from<int>(egl->m_pMainWindow, 0x1b4), from<int>(egl->m_pMainWindow, 0x1b8));
		ImGui::DragFloat("Width", &egl->m_obWindowedSize.width);
		ImGui::DragFloat("Height", &egl->m_obWindowedSize.height);
		
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

		if (ImGui::Checkbox("Borderless Fullscreen", &setting().onBorderlessFullscreen)) {
			if (setting().onBorderlessFullscreen) {
				if (egl->m_bIsFullscreen) {
					gm->setGameVariable("0025", false);
					gm->reloadAll(true, false, true);
				}

				HWND windowHwnd = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(egl->m_pMainWindow) + 0x244));
				LONG windowStyle = GetWindowLong(windowHwnd, GWL_STYLE);
				SetWindowLong(windowHwnd, GWL_STYLE, windowStyle & ~(WS_CAPTION | WS_SIZEBOX | WS_SYSMENU));

				toggleFreeWindowResize(true);

				HMONITOR monitor = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
				MONITORINFO monitorInfo;
				monitorInfo.cbSize = sizeof(MONITORINFO);
				if (!GetMonitorInfo(monitor, &monitorInfo)) {
					return;
				}

				RECT monitorRect = monitorInfo.rcMonitor;

				auto width = monitorRect.right - monitorRect.left;
				auto height = monitorRect.bottom - monitorRect.top;

				SetWindowPos(windowHwnd, HWND_TOP, monitorRect.left, monitorRect.top, width, height, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				egl->resizeWindow(width, height);
			}
			else {
				HWND windowHwnd = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(egl->m_pMainWindow) + 0x244));
				SetWindowLong(windowHwnd, GWL_STYLE, 0x6cf0000);

				toggleFreeWindowResize(false);

				HMONITOR monitor = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
				MONITORINFO monitorInfo;
				monitorInfo.cbSize = sizeof(MONITORINFO);
				if (!GetMonitorInfo(monitor, &monitorInfo)) {
					return;
				}

				RECT monitorRect = monitorInfo.rcMonitor;

				auto width = egl->m_obWindowedSize.width;
				auto height = egl->m_obWindowedSize.height;

				SetWindowPos(windowHwnd, HWND_TOP, monitorRect.left, monitorRect.top, width, height, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				egl->resizeWindow(width, height);
			}
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

			ImGui::Checkbox("m_isCameraShaking", &pl->m_isCameraShaking);
			ImGui::DragFloat("m_currentShakeStrength", &pl->m_currentShakeStrength);
			ImGui::Checkbox("m_hintShown", &pl->m_hintShown);
			ImGui::DragFloat("m_cameraFlip", &pl->m_cameraFlip);

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
			
			auto bgRef = pl->m_backgroundSprite->getColor();
			float bgColor[4] = { bgRef.r / 255.f, bgRef.g / 255.f, bgRef.b / 255.f, 1.f };
			ImGui::ColorEdit4("BG", bgColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoPicker);
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