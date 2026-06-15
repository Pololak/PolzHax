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
    // freopen("/storage/emulated/0/PolzHaxMobile/19/polzhax.txt", "r", stdin);
    // int hc;
    // std::cin >> hc;
    // for (int i = 0; i < hc; ++i) {
    //     std::string name;
    //     std::cin >> name;
    //     if (name == "onCharacterFilter") std::cin >> setting().onCharacterFilter;
    //     if (name == "onIcons") std::cin >> setting().onIcons;
    //     if (name == "onMainLevels") std::cin >> setting().onMainLevels;
    //     if (name == "onSliderLimit") std::cin >> setting().onSliderLimit;
    //     if (name == "onTextLength") std::cin >> setting().onTextLength;
    // }

    // if (f.is_open()) {
    //     int count;
    //     f >> count;
    //     for (int i = 0; i < count; ++i) {
    //         std::string name;
    //         f >> name;
    //         if (name == SETTING_NAME(setting().onCharacterFilter))  f >> setting().onCharacterFilter;
    //         if (name == SETTING_NAME(setting().onIcons))            f >> setting().onIcons;
    //         if (name == SETTING_NAME(setting().onMainLevels))       f >> setting().onMainLevels;
    //         if (name == SETTING_NAME(setting().onSliderLimit))      f >> setting().onSliderLimit;
    //         if (name == SETTING_NAME(setting().onTextLength))       f >> setting().onTextLength;
    //     }
    // }
}

void SettingStruct::save() {
    // std::ofstream f;
    // f.open("/storage/emulated/0/PolzHaxMobile/19/polzhax.json", std::ofstream::binary);
    // if (f.is_open()) {
    //     f << SETTING_COUNT << "\n";
    //     f << SETTING_NAME(setting().onCharacterFilter) << " "    << setting().onCharacterFilter << "\n";
    //     f << SETTING_NAME(setting().onIcons) << " "              << setting().onIcons << "\n";
    //     f << SETTING_NAME(setting().onMainLevels) << " "         << setting().onMainLevels << "\n";
    //     f << SETTING_NAME(setting().onSliderLimit) << " "        << setting().onSliderLimit << "\n";
    //     f << SETTING_NAME(setting().onTextLength) << " "         << setting().onTextLength << "\n";
    //     f.close();
    // }

    // LOGD("SettingStruct size: %i", sizeof(SettingStruct));
    // for (int i = 0; i < sizeof(SettingStruct); ++i) {

    // }
}