#include "Setting.h"
#include "utils.h"
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

#include <unordered_map>
#include <string>

// I hate this code

#define SETTING_NAME(var) (#var + 10)

SettingStruct& setting() {
    static SettingStruct ret;
    return ret;
}

void SettingStruct::load() {
    std::ifstream f;
    f.open("/storage/emulated/0/PolzHaxMobile/19/polzhax.txt");
    if (!f.is_open()) {
        f.close();
        return;
    }
    f.close();

    auto file = readFile("/storage/emulated/0/PolzHaxMobile/19/polzhax.txt");
    std::string data(file.begin(), file.end());

    tinyxml2::XMLDocument doc;
    auto parseRes = doc.Parse(data.c_str());

    if (parseRes != tinyxml2::XMLError::XML_SUCCESS) {
        return;
    }

    auto child = doc.FirstChildElement("setting");
    for (child = child->FirstChildElement(); child != nullptr; child = child->NextSiblingElement("name")) {
        if (strcmp(child->Value(), "name") == 0) {
            auto key = child->GetText();

            child = child->NextSiblingElement();
            if (child == nullptr) break;

            auto value = child->GetText();
            if (value == nullptr) continue;

            // Bypass
            if (strcmp(key, SETTING_NAME(setting().onCharacterFilter)) == 0) {
                setting().onCharacterFilter = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onIcons)) == 0) {
                setting().onIcons = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onMainLevels)) == 0) {
                setting().onMainLevels = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onSliderLimit)) == 0) {
                setting().onSliderLimit = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onTextLength)) == 0) {
                setting().onTextLength = std::atoi(value);
            }
            // Cosmetic
            else if (strcmp(key, SETTING_NAME(setting().onNoDeathEffect)) == 0) {
                setting().onNoDeathEffect = std::atoi(value);
            }
            // Creator
            else if (strcmp(key, SETTING_NAME(setting().onHitboxBugFix)) == 0) {
                setting().onHitboxBugFix = std::atoi(value);
            }
            // Level
            else if (strcmp(key, SETTING_NAME(setting().onNoclip)) == 0) {
                setting().onNoclip = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onPracticeMusic)) == 0) {
                setting().onPracticeMusic = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onSmartStartPos)) == 0) {
                setting().onSmartStartPos = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onStartPosSwitcher)) == 0) {
                setting().onStartPosSwitcher = std::atoi(value);
            }
            // Universal
            else if (strcmp(key, SETTING_NAME(setting().onNoTransition)) == 0) {
                setting().onNoTransition = std::atoi(value);
            }
            // Speedhack
            else if (strcmp(key, SETTING_NAME(setting().onSpeedhack)) == 0) {
                setting().onSpeedhack = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onSpeedhackMusic)) == 0) {
                setting().onSpeedhackMusic = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().speedhackValue)) == 0) {
                setting().speedhackValue = static_cast<float>(std::atof(value));
            }
            // Editor options
            else if (strcmp(key, SETTING_NAME(setting().onDurationLines)) == 0) {
                setting().onDurationLines = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onExperimentalLayering)) == 0) {
                setting().onExperimentalLayering = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onPreviewRotations)) == 0) {
                setting().onPreviewRotations = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onShowClicks)) == 0) {
                setting().onShowClicks = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onUnusedObjects)) == 0) {
                setting().onUnusedObjects = std::atoi(value);
            }
            else if (strcmp(key, SETTING_NAME(setting().onNewColorSelectMenu)) == 0) {
                setting().onNewColorSelectMenu = std::atoi(value);
            }
            // Other menu things
            else if (strcmp(key, SETTING_NAME(setting().m_liveColorEnabled)) == 0) {
                setting().m_liveColorEnabled = std::atoi(value);
            }
        }
    }

    LOGD("State loaded...");
}

void SettingStruct::save() {
    std::stringstream data;

    {
        data
            << "<setting>"
            // Bypass
            << "<name>" << SETTING_NAME(setting().onCharacterFilter) << "</name>"
            << "<val>" << setting().onCharacterFilter << "</val>"
            << "<name>" << SETTING_NAME(setting().onIcons) << "</name>"
            << "<val>" << setting().onIcons << "</val>"
            << "<name>" << SETTING_NAME(setting().onMainLevels) << "</name>"
            << "<val>" << setting().onMainLevels << "</val>"
            << "<name>" << SETTING_NAME(setting().onSliderLimit) << "</name>"
            << "<val>" << setting().onSliderLimit << "</val>"
            << "<name>" << SETTING_NAME(setting().onTextLength) << "</name>"
            << "<val>" << setting().onTextLength << "</val>"
            // Cosmetic
            << "<name>" << SETTING_NAME(setting().onNoDeathEffect) << "</name>"
            << "<val>" << setting().onNoDeathEffect << "</val>"
            // Creator
            << "<name>" << SETTING_NAME(setting().onHitboxBugFix) << "</name>"
            << "<val>" << setting().onHitboxBugFix << "</val>"
            // Level
            << "<name>" << SETTING_NAME(setting().onNoclip) << "</name>"
            << "<val>" << setting().onNoclip << "</val>"
            << "<name>" << SETTING_NAME(setting().onPracticeMusic) << "</name>"
            << "<val>" << setting().onPracticeMusic << "</val>"
            << "<name>" << SETTING_NAME(setting().onSmartStartPos) << "</name>"
            << "<val>" << setting().onSmartStartPos << "</val>"
            << "<name>" << SETTING_NAME(setting().onStartPosSwitcher) << "</name>"
            << "<val>" << setting().onStartPosSwitcher << "</val>"
            // Universal
            << "<name>" << SETTING_NAME(setting().onNoTransition) << "</name>"
            << "<val>" << setting().onNoTransition << "</val>"
            // Speedhack
            << "<name>" << SETTING_NAME(setting().onSpeedhack) << "</name>"
            << "<val>" << setting().onSpeedhack << "</val>"
            << "<name>" << SETTING_NAME(setting().onSpeedhackMusic) << "</name>"
            << "<val>" << setting().onSpeedhackMusic << "</val>"
            << "<name>" << SETTING_NAME(setting().speedhackValue) << "</name>"
            << "<val>" << setting().speedhackValue << "</val>"
            // Editor Options
            << "<name>" << SETTING_NAME(setting().onDurationLines) << "</name>"
            << "<val>" << setting().onDurationLines << "</val>"
            << "<name>" << SETTING_NAME(setting().onExperimentalLayering) << "</name>"
            << "<val>" << setting().onExperimentalLayering << "</val>"
            << "<name>" << SETTING_NAME(setting().onPreviewRotations) << "</name>"
            << "<val>" << setting().onPreviewRotations << "</val>"
            << "<name>" << SETTING_NAME(setting().onShowClicks) << "</name>"
            << "<val>" << setting().onShowClicks << "</val>"
            << "<name>" << SETTING_NAME(setting().onUnusedObjects) << "</name>"
            << "<val>" << setting().onUnusedObjects << "</val>"
            << "<name>" << SETTING_NAME(setting().onNewColorSelectMenu) << "</name>"
            << "<val>" << setting().onNewColorSelectMenu << "</val>"
            // Other menu things
            << "<name>" << SETTING_NAME(setting().m_liveColorEnabled) << "</name>"
            << "<val>" << setting().m_liveColorEnabled << "</val>"
            << "</setting>";
    }

    std::ofstream f;
    f.open("/storage/emulated/0/PolzHaxMobile/19/polzhax.txt", std::ios::out | std::ios::trunc | std::ios::binary);
    f.clear();
    f.write(data.str().c_str(), data.str().size());
    f.close();

    LOGD("State saved...");
}