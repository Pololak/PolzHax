#pragma once
#include "pch.h"
#include <vector>
#include <unordered_map>
#include <map>

namespace PolzBot {
	struct Event {
		bool down;
		bool p2;
		int frame;
		float xPosition;
		float yPosition;
		float rotation;
		double yVelocity;
	};

	inline size_t m_eventIndex;
	inline std::vector<std::string> replayNames;
	inline std::map<int, Event> m_replayEvents;
	inline std::vector<Event> m_replayEventsVec;

	void save();
	void load();

	void updateReplayList();
}