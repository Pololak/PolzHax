#pragma once
#include "pch.h"
#include "utils.hpp"
#include "state.h"

struct Icons {
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