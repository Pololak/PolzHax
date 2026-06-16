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
            // Speedhack
            << "<name>" << SETTING_NAME(setting().onSpeedhack) << "</name>"
            << "<val>" << setting().onSpeedhack << "</val>"
            << "<name>" << SETTING_NAME(setting().onSpeedhackMusic) << "</name>"
            << "<val>" << setting().onSpeedhackMusic << "</val>"
            << "<name>" << SETTING_NAME(setting().speedhackValue) << "</name>"
            << "<val>" << setting().speedhackValue << "</val>"
            << "</setting>";
    }

    std::ofstream f;
    f.open("/storage/emulated/0/PolzHaxMobile/19/polzhax.txt", std::ios::out | std::ios::trunc | std::ios::binary);
    f.clear();
    f.write(data.str().c_str(), data.str().size());
    f.close();

    LOGD("State saved...");
}