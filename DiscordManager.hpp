#pragma once
#include "pch.h"
#include <thread>

#include <discord_rpc.h>

// https://gitlab.com/tobyadd/GDH/-/blob/main/src/hacks/core/discord_rpc.cpp

struct PresenceSettings {
    std::string state = "";
    std::string details = "";
    std::string largeImageKey;
    std::string largeImageText;
    std::string smallImageKey;
    std::string smallImageText;
};

class DiscordManager {
public:
    static DiscordManager& get() {
        static DiscordManager inst;
        return inst;
    }

    bool m_initialized = false;
    std::thread m_thread;
    int64_t rpcStartTime = 0;

    std::string m_state;
    std::string m_details;
    std::string m_smallKey;
    std::string m_smallText;

    DiscordManager() = default;
    DiscordManager(const DiscordManager&) = delete;
    DiscordManager& operator=(const DiscordManager&) = delete;

    PresenceSettings settingsForLevel(gd::GJGameLevel*);
    PresenceSettings settingsForEditor(gd::LevelEditorLayer*);

    void start();
    void stop();
    void updatePresence(PresenceSettings);
};