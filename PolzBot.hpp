#pragma once
#include "pch.h"
#include <vector>
#include <unordered_map>
#include <map>

namespace PolzBot {
	struct Event {
		bool down;
		bool p2;
		float xPosition;
		float yPosition;
		float rotation;
		double yVelocity;
	};

	inline std::vector<std::string> replayNames;
	inline std::map<int, Event> m_replayEvents;

	void save();
	void load();

	void updateReplayList();
}