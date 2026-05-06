#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

inline std::pair<std::string, std::string> split_once(const std::string& str, char split) {
	const auto n = str.find(split);
	return { str.substr(0, n), str.substr(n + 1) };
}