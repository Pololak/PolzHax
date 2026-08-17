#include "discord_register.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>

static void Discord_RegisterW(const wchar_t* applicationId, const wchar_t* command)
{
    wchar_t exeFilePath[MAX_PATH];
    DWORD exeLen = GetModuleFileNameW(nullptr, exeFilePath, MAX_PATH);
    wchar_t openCommand[1024];

    if (command && command[0]) {
        swprintf_s(openCommand, L"%s", command);
    }
    else {
        swprintf_s(openCommand, L"%s", exeFilePath);
    }

    wchar_t protocolName[64];
    swprintf_s(protocolName, L"discord-%s", applicationId);
    
    wchar_t protocolDescription[128];
    swprintf_s(protocolDescription, L"URL:Run game %s protocol", applicationId);
    
    wchar_t urlProtocol = 0;
    wchar_t keyName[256];
    swprintf_s(keyName, L"Software\\Classes\\%s", protocolName);
    
    HKEY key;
    auto status = RegCreateKeyExW(HKEY_CURRENT_USER, keyName, 0, nullptr, 0, KEY_WRITE, nullptr, &key, nullptr);
    if (status != ERROR_SUCCESS) {
        return;
    }

    DWORD len = (DWORD)lstrlenW(protocolDescription) + 1;
    RegSetKeyValueW(key, nullptr, nullptr, REG_SZ, protocolDescription, len * sizeof(wchar_t));
    RegSetKeyValueW(key, nullptr, L"URL Protocol", REG_SZ, &urlProtocol, sizeof(wchar_t));
    RegSetKeyValueW(key, L"DefaultIcon", nullptr, REG_SZ, exeFilePath, (exeLen + 1) * sizeof(wchar_t));

    len = (DWORD)lstrlenW(openCommand) + 1;
    RegSetKeyValueW(key, L"shell\\open\\command", nullptr, REG_SZ, openCommand, len * sizeof(wchar_t));
    
    RegCloseKey(key);
}

extern "C" DISCORD_EXPORT void Discord_Register(const char* applicationId, const char* command)
{
    wchar_t appId[32];
    MultiByteToWideChar(CP_UTF8, 0, applicationId, -1, appId, 32);

    wchar_t openCommand[1024];
    const wchar_t* wcommand = nullptr;
    if (command && command[0]) {
        MultiByteToWideChar(CP_UTF8, 0, command, -1, openCommand, 1024);
        wcommand = openCommand;
    }

    Discord_RegisterW(appId, wcommand);
}

extern "C" DISCORD_EXPORT void Discord_RegisterSteamGame(const char* applicationId, const char* steamId)
{
    wchar_t appId[32];
    MultiByteToWideChar(CP_UTF8, 0, applicationId, -1, appId, 32);

    wchar_t wSteamId[32];
    MultiByteToWideChar(CP_UTF8, 0, steamId, -1, wSteamId, 32);

    HKEY key;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", 0, KEY_READ, &key) != ERROR_SUCCESS) {
        return;
    }

    wchar_t steamPath[MAX_PATH];
    DWORD pathBytes = sizeof(steamPath);
    auto status = RegQueryValueExW(key, L"SteamExe", nullptr, nullptr, (BYTE*)steamPath, &pathBytes);
    RegCloseKey(key);
    if (status != ERROR_SUCCESS || pathBytes < 1) {
        return;
    }

    DWORD pathChars = pathBytes / sizeof(wchar_t);
    for (DWORD i = 0; i < pathChars; ++i) {
        if (steamPath[i] == L'/') {
            steamPath[i] = L'\\';
        }
    }

    wchar_t command[1024];
    swprintf_s(command, L"\"%s\" steam://rungameid/%s", steamPath, wSteamId);

    Discord_RegisterW(appId, command);
}