#include "PolzBot.hpp"
#include <fstream>
#include "json.hpp"
#include "Setting.hpp"

using namespace nlohmann;

void PolzBot::save() {
	if (PolzBot::m_replayEventsVec.empty()) {
		gd::FLAlertLayer::create("Error", "Macro is empty.", "OK")->show();
		return;
	}

	if (setting().m_selectedMacro.empty()) {
		gd::FLAlertLayer::create("Error", "File name is empty.", "OK")->show();
		return;
	}

	json j;
	std::ofstream o;
	std::stringstream buf;
	o.open(CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/replays/" + setting().m_selectedMacro + ".pgdr");
	
	j["_"] = "PolzHax 1.920 - v1.3.3 (Vanilla) Build 090826";
	j["fps"] = static_cast<int>(setting().fpsValue);
	j["events"] = json::array();
	for (const auto& event : PolzBot::m_replayEventsVec) {
		json jEvent;

		jEvent["frame"] =	event.frame;
		jEvent["down"] =	event.down;
		jEvent["p2"] =		event.p2;
		jEvent["rot"] =		event.rotation;
		jEvent["x"] =		event.xPosition;
		jEvent["y"] =		event.yPosition;
		jEvent["yVel"] =	event.yVelocity;

		j["events"].push_back(jEvent);
	}

	buf << j.dump(1);

	o.write(buf.str().c_str(), buf.str().size());
	o.close();

	updateReplayList();
}

void PolzBot::load() {
	std::ifstream i;
	i.open(CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/replays/" + setting().m_selectedMacro + ".pgdr");

	if (!i.is_open()) {
		gd::FLAlertLayer::create("Error", "File not found.", "OK")->show();
		return;
	}

	if (i.peek() == std::ifstream::traits_type::eof()) {
		gd::FLAlertLayer::create("Error", "File is empty.", "OK")->show();
		return;
	}

	json j = json::parse(i);

	std::cout << "Loaded macro for " << j["_"] << std::endl;

	setting().fpsValue = j["fps"];
	setting().tpsValue = j["fps"];

	std::cout << j["events"].size() << std::endl;

	PolzBot::m_replayEventsVec.clear();
	for (int z = 0; z < j["events"].size(); z++) {
		auto jEvent = j["events"][z];
		PolzBot::Event event;
		
		event.down =		jEvent["down"];
		event.p2 =			jEvent["p2"];
		event.frame =		jEvent["frame"];
		event.rotation =	jEvent["rot"];
		event.xPosition =	jEvent["x"];
		event.yPosition =	jEvent["y"];
		event.yVelocity =	jEvent["yVel"];

		PolzBot::m_replayEventsVec.push_back(event);
	}

	i.close();
}

void PolzBot::updateReplayList() {
	replayNames.clear();
	auto replaysPath = CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/replays";
	for (const auto& file : std::filesystem::directory_iterator(replaysPath)) {
		if (file.path().extension() == ".pgdr") {
			auto replayName = file.path().stem().string();
			replayNames.push_back(replayName);
		}
	}
}