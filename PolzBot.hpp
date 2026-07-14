#pragma once
#include "pch.h"
#include <vector>
#include <unordered_map>
#include <map>

namespace PolzBot {
	inline std::vector<std::string> replayNames;
	inline std::map<int, std::pair<bool, bool>> m_replayEvents;

	void save();
	void load();

	void updateReplayList();
}