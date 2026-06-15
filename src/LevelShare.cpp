#include "LevelShare.h"
#include <fstream>
#include "utils.h"

std::vector<char> readFile(const char* filename) {
    std::basic_ifstream<char> file(filename, std::ios::binary);

    return std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

bool LevelShare::exportLevel(GJGameLevel* level, std::string name, bool replace) {
    const std::string songKey = level->m_songID ?
        ("<k>k45</k><i>" + std::string(CCString::createWithFormat("%i", level->m_songID)->getCString()) + "</i>") :
        level->m_audioTrack ?
        ("<k>k8</k><i>" + std::string(CCString::createWithFormat("%i", level->m_audioTrack)->getCString()) + "</i>") : "";

    const std::string encodedDescription = base64::encode(base64::encode(level->m_levelDesc.c_str()));

    std::stringstream data;

    {
        data
            << "<d>"
            << "<k>kCEK</k>"
            << "<i>4</i>"
            << "<k>k2</k>"
            << "<s>" << level->m_levelName << "</s>"
            << "<k>k3</k>"
            << "<s>" << encodedDescription
            << "</s>"
            << "<k>k4</k>"
            << "<s>" << level->m_levelString
            << "</s>"
            << songKey
            << "<k>k13</k>"
            << "<t/>"
            << "<k>k21</k>"
            << "<i>2</i>"
            << "<k>k50</k>"
            << "<i>24</i>"
            << "</d>";
    }
    
    std::string path = "/storage/emulated/0/PolzHaxMobile/19/levels/" + (name.empty() ? level->m_levelName : name) + ".gmd";

    if (!replace) {
        std::ifstream o;
        o.open(path, std::ios::in | std::ios::binary);
        if (o.is_open()) {
            o.close();
            return false;
        }
        o.close();
    }

    std::ofstream f;
    f.open(path.c_str(), std::ios::out | std::ios::binary);
    f << data.str();
    f.close();

    FLAlertLayer::create("Success", "The level has been exported.", "OK")->show();

    return true;
}

bool LevelShare::importLevel(std::string path) {
    if (path.find(".gmd") == std::string::npos) {
        FLAlertLayer::create("Error", "File not found.", "OK")->show();
        return false;
    }

    std::ifstream fcheck;
    fcheck.open(path.c_str());
    if (!fcheck.is_open()) {
        FLAlertLayer::create("Error", "Failed to open file.", "OK")->show();
        fcheck.close();
        return false;
    }
    fcheck.close();

    auto file = readFile(path.c_str());
    std::string data(file.begin(), file.end());

    LOGD("Data: %s", data.c_str());
    LOGD("Path: %s", path.c_str());
    LOGD("File size: %i", data.size());

    tinyxml2::XMLDocument doc;

    auto parseRes = doc.Parse(data.c_str());

    if (parseRes != tinyxml2::XMLError::XML_SUCCESS) {
        FLAlertLayer::create("Error", "Failed to parse.", "OK")->show();
        return false;
    }

    GJGameLevel* importedLevel = GameLevelManager::sharedState()->createNewLevel();
    importedLevel->m_levelType = GJLevelType::Editor;
    GameLevelManager::sharedState()->m_createdNewLevel = true;

    auto child = doc.FirstChildElement("d");
    for (child = child->FirstChildElement(); child != nullptr; child = child->NextSiblingElement("k")) {
        if (strcmp(child->Value(), "k") == 0) {
            auto key = child->GetText();

            child = child->NextSiblingElement();
            if (child == nullptr) break;

            auto value = child->GetText();
            if (value == nullptr) continue;

            if (strcmp(key, "kCEK") == 0) {
                auto obj_type = std::atoi(value);
                if (obj_type != 4) {
                    FLAlertLayer::create("Error", "A valid file was not provided for import.", "OK")->show();
                    return false;
                }
            }
            else if (strcmp(key, "k2") == 0) {
                LOGD("k2: %s", value);
                importedLevel->setLevelName(value);
            }
            else if (strcmp(key, "k3") == 0) {
                LOGD("k3: %s", value);
                auto desc = base64::decode(base64::decode(value));
                LOGD("Decoded desc: %s", desc.c_str());
                importedLevel->setLevelDesc(desc);
            }
            else if (strcmp(key, "k4") == 0) {
                LOGD("k4: %s", value);
                importedLevel->setLevelString(value);
            }
            else if (strcmp(key, "k8") == 0) {
                LOGD("k8: %s", value);
                importedLevel->m_audioTrack = std::atoi(value);
            }
            else if (strcmp(key, "k45") == 0) {
                LOGD("k45: %s", value);
                importedLevel->m_songID = std::atoi(value);
            }
        }
    }

    if (importedLevel) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, EditLevelLayer::scene(importedLevel)));
    }

    return true;
}