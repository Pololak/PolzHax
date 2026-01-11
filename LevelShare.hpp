#pragma once
#include "pch.h"
#include "utils.hpp"
#include "matplist.hpp"

template <class S>
void dumpLevel(gd::GJGameLevel* level, S& stream) {
	const auto song_key = level->m_songID ?
		format("<k>k45</k><i>{}</i>", level->m_songID) :
		level->m_audioTrack ?
		format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";

	const auto encoded_desc = base64::encode(base64::encode(level->m_levelDesc));

	std::stringstream data;

	{
		data
			<< "<d>"
			<< "<k>kCEK</k>"
			<< "<i>4</i>"
			<< "<k>k2</k>"
			<< "<s>" << level->m_levelName << "</s>"
			<< "<k>k3</k>"
			<< "<s>" << encoded_desc
			<< "</s>"
			<< "<k>k4</k>"
			<< "<s>" << level->m_levelString
			<< "</s>"
			<< song_key
			<< "<k>k13</k>"
			<< "<t/>"
			<< "<k>k21</k>"
			<< "<i>2</i>"
			<< "<k>k50</k>"
			<< "<i>24</i>"
			<< "</d>";
	}

	stream << data.str();
}

template <class S>
gd::GJGameLevel* importLevel(S& stream) {
	auto data = matplist::parse(stream);
	auto level = gd::GameLevelManager::sharedState()->createNewLevel();
	for (const auto& [key, var] : data) {
		if (!std::holds_alternative<matplist::Value>(var)) continue;
		matplist::Value value = std::get<matplist::Value>(var);
		using namespace std::literals;
		if (key == "k2"sv) {
			from<gd::string>(level, 0xf0) = value.value;
		}
		else if (key == "k3"sv) {
			from<gd::string>(level, 0x108) = base64::decode(base64::decode(value.value));
		}
		else if (key == "k4"sv) {
			from<gd::string>(level, 0x120) = value.value;
		}
		else if (key == "k8"sv) {
			level->m_audioTrack = std::stoi(value.value);
		}
		else if (key == "k45"sv) {
			level->m_songID = std::stoi(value.value);
		}
	}
	return level;
}