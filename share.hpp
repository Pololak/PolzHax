#pragma once
#include "pch.h"
#include <fstream>
#include "utils.hpp"
#include "matplist.hpp"

template <class S>
void dump_level(gd::GJGameLevel* level, S& stream) {
	const auto song_key = level->m_songID ?
		format("<k>k45</k><i>{}</i>", level->m_songID) :
		level->m_audioTrack ?
		format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";
	// encode it twice because gdshare does too
	// why? i dont know fod is stupid
	const auto encoded_desc = base64_encode(base64_encode(level->m_levelDesc.sv()));
	format_to(stream, R"(<d>
	<k>kCEK</k><i>4</i>
	<k>k2</k><s>{}</s>
	<k>k3</k><s>{}</s>
	<k>k4</k><s>{}</s>
	{}
	<k>k13</k><t/>
	<k>k21</k><i>2</i>
	<k>k50</k><i>24</i>
</d>)", level->m_levelName.sv(), encoded_desc, level->m_levelString.sv(), song_key);
}

template <class S>
gd::GJGameLevel* import_level(S& stream) {
	auto data = matplist::parse(stream);
	auto level = gd::GameLevelManager::sharedState()->createNewLevel();
	for (const auto& [key, var] : data) {
		if (!std::holds_alternative<matplist::Value>(var)) continue;
		matplist::Value value = std::get<matplist::Value>(var);
		using namespace std::literals;
		if (key == "k2"sv) {
			level->m_levelName = value.value;
		}
		else if (key == "k3"sv) {
			level->m_levelDesc = base64_decode(base64_decode(value.value));
		}
		else if (key == "k4"sv) {
			level->m_levelString = value.value;
		}
		else if (key == "k8"sv) {
			const auto text = value.value;
			level->m_audioTrack = std::stoi(text);
		}
		else if (key == "k45"sv) {
			const auto text = value.value;
			level->m_songID = std::stoi(text);
		}
	}
	return level;
}