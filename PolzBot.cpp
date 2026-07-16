#include "PolzBot.hpp"
#include <fstream>
#include "json.hpp"
#include "Setting.hpp"

using namespace nlohmann;

void PolzBot::save() {
	if (PolzBot::m_replayEvents.empty()) {
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
	
	j["_"] = "PolzHax 1.920 - v1.3.3 (Vanilla) Build 160726";
	j["fps"] = static_cast<int>(setting().fpsValue);
	j["events"] = json::array();
	for (const auto& pushFrame : PolzBot::m_replayEvents) {
		json push;
		push["frame"] = pushFrame.first;
		push["down"] =	pushFrame.second.down;
		push["p2"] =	pushFrame.second.p2;
		push["rot"] =	pushFrame.second.rotation;
		push["x"] =		pushFrame.second.xPosition;
		push["y"] =		pushFrame.second.yPosition;
		push["yVel"] =	pushFrame.second.yVelocity;
		j["events"].push_back(push);
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

	PolzBot::m_replayEvents.clear();
	for (int z = 0; z < j["events"].size(); z++) {
		auto event = j["events"][z];
		PolzBot::m_replayEvents[event["frame"]].down =		event["down"];
		PolzBot::m_replayEvents[event["frame"]].p2 =		event["p2"];
		PolzBot::m_replayEvents[event["frame"]].rotation =	event["rot"];
		PolzBot::m_replayEvents[event["frame"]].xPosition =	event["x"];
		PolzBot::m_replayEvents[event["frame"]].yPosition = event["y"];
		PolzBot::m_replayEvents[event["frame"]].yVelocity = event["yVel"];
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