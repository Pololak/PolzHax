#pragma once
#include <Geode/Geode.hpp>
#include "patching.hpp"

using namespace geode::prelude;

inline std::pair<std::string, std::string> split_once(const std::string& str, char split) {
	const auto n = str.find(split);
	return { str.substr(0, n), str.substr(n + 1) };
}

inline void safeModeON() {
	sequence_patch(geode::base::get() + 0xf0624, { 0xeb, 0x6c });
	sequence_patch(geode::base::get() + 0xe53b6, { 0xe9, 0x77, 0x01, 0x00, 0x00, 0x90 });
	sequence_patch(geode::base::get() + 0xe5419, { 0xe9, 0x14, 0x00, 0x00, 0x00, 0x90 });
}

inline void safeModeOFF() {
	sequence_patch(geode::base::get() + 0xf0624, { 0x75, 0x6c });
	sequence_patch(geode::base::get() + 0xe53b6, { 0x0f, 0x85, 0x76, 0x01, 0x00, 0x00 });
	sequence_patch(geode::base::get() + 0xe5419, { 0x0f, 0x85, 0x13, 0x01, 0x00, 0x00 });
}