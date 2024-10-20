#pragma once
#include "pch.h"
#include "utils.hpp"
#include "state.h"

struct Icons {
	static void patchCube(int amount) {
		auto cubeAmount = patchUtils::intToBytes(amount);
		patch(gd::base + 0x7fe60, { 0xbe, cubeAmount[0], cubeAmount[1], cubeAmount[2], cubeAmount[3] }); //GaragePage::init
		patch(gd::base + 0x80d3f, { 0xb8, cubeAmount[0], cubeAmount[1], cubeAmount[2], cubeAmount[3] }); //SimplePlayer::updatePlayerFrame

		patch(gd::base + 0x80841, { 0xba, cubeAmount[0], cubeAmount[1], cubeAmount[2], cubeAmount[3] }); //SimplePlayer::init

		patch(gd::base + 0xdfff6, { 0xba, cubeAmount[0], cubeAmount[1], cubeAmount[2], cubeAmount[3] }); //PlayerObject::updatePlayerFrame
		patch(gd::base + 0xd8cd9, { 0xb9, cubeAmount[0], cubeAmount[1], cubeAmount[2], cubeAmount[3] }); //PlayerObject::init
	}

	static void patchShip(int amount) {
		auto shipAmount = patchUtils::intToBytes(amount);
		patch(gd::base + 0x7fe93, { 0xbe, shipAmount[0], shipAmount[1], shipAmount[2], shipAmount[3] }); //GaragePage::init
		patch(gd::base + 0x80ce9, { 0xb8, shipAmount[0], shipAmount[1], shipAmount[2], shipAmount[3] }); //SimplePlayer::updatePlayerFrame

		patch(gd::base + 0xe0146, { 0xba, shipAmount[0], shipAmount[1], shipAmount[2], shipAmount[3] }); //PlayerObject::updatePlayerShipFrame
		patch(gd::base + 0xd8cef, { 0xb8, shipAmount[0], shipAmount[1], shipAmount[2], shipAmount[3] }); //PlayerObject::init
	}

	static void patchBall(int amount) {
		auto ballAmount = patchUtils::intToBytes(amount);
		patch(gd::base + 0x7fec6, { 0xbe, ballAmount[0], ballAmount[1], ballAmount[2], ballAmount[3] }); //GaragePage::init
		patch(gd::base + 0x80cfe, { 0xb8, ballAmount[0], ballAmount[1], ballAmount[2], ballAmount[3] }); //SimplePlayer::updatePlayerFrame

		patch(gd::base + 0xe0436, { 0xba, ballAmount[0], ballAmount[1], ballAmount[2], ballAmount[3] }); //PlayerObject::updatePlayerRollFrame
	}

	static void patchBird(int amount) {
		auto ufoAmount = patchUtils::intToBytes(amount);
		patch(gd::base + 0x7fef6, { 0xbe, ufoAmount[0], ufoAmount[1], ufoAmount[2], ufoAmount[3] }); //GaragePage::init
		patch(gd::base + 0x80d13, { 0xb8, ufoAmount[0], ufoAmount[1], ufoAmount[2], ufoAmount[3] }); //SimplePlayer::updatePlayerFrame

		patch(gd::base + 0xe0296, { 0xba, ufoAmount[0], ufoAmount[1], ufoAmount[2], ufoAmount[3] }); //PlayerObject::updatePlayerBirdFrame
	}

	static void patchDart(int amount, int dartID) {
		auto count = getCount("dart", "001");
		if (dartID > amount) {
			dartID = amount;
			setting().selected_dart = amount;
		}
		auto dartAmount = patchUtils::intToBytes(amount);
		//patch(gd::base + 0x7fef6, { 0xbe, dartAmount[0], dartAmount[1], dartAmount[2], dartAmount[3] }); //GaragePage::init
		patch(gd::base + 0x80d28, { 0xb8, dartAmount[0], dartAmount[1], dartAmount[2], dartAmount[3] }); //SimplePlayer::updatePlayerFrame

		auto patchDartID = patchUtils::intToBytes(dartID);
		patch(gd::base + 0xe058f, { 0x6a, patchDartID[0] }); //PlayerObject::updatePlayerDartFrame 1
		patch(gd::base + 0xe05a8, { 0x6a, patchDartID[0] }); //PlayerObject::updatePlayerDartFrame 2
		patch(gd::base + 0xe05ba, { 0x6a, patchDartID[0] }); //PlayerObject::updatePlayerDartFrame 3
	}

	static int getCount(std::string startLabel, std::string end) { // taken from https://github.com/Alphalaneous/ExtraIcons/blob/main/src/main.cpp

		int count = 1;
		while (true) {

			std::stringstream number;
			number << std::setw(2) << std::setfill('0') << count;

			if (CCSprite::createWithSpriteFrameName((startLabel + "_" + number.str() + "_" + end + ".png").c_str())) {
				count++;
			}
			else {
				break;
			}
		}
		std::cout << startLabel << ": " << count - 1 << "\n";
		return count - 1;
	}
};