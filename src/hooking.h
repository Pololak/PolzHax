#pragma once
#include "../libs/dlfcn-win32-1.4.2/src/dlfcn.h"
#include <cstdio>
#include <android/log.h>
#include <mutex>
#include <cstdint>
#include "../Dobby/include/dobby.h"

#define LOG_TAG "PolzHax"

namespace hooking {
    static void* getHandle() {
        static void* handle = nullptr;
        static std::once_flag once;
        std::call_once(once, []() {
            handle = dlopen("libcocos2dcpp.so", RTLD_NOW);
            if (!handle) {
                __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "dlopen(libcocos2dcpp.so) failed: %s - trying global handle", dlerror());
                handle = dlopen(NULL, RTLD_NOW);
                if (!handle) {
                    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "dlopen(NULL) failed: %s", dlerror());
                } else {
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "using global handle %p", handle);
                }
            } else {
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "dlopen(libcocos2dcpp.so) -> %p", handle);
            }
        });
        return handle;
    }

    inline void hook(const char* symbol, void* func, void** origFunc) {
     void* h = getHandle();
    if (!h) return;
    void* addr = dlsym(h, symbol);
    if (!addr) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "dlsym('%s') failed", symbol);
        return;
    }

	uintptr_t thumb_addr = reinterpret_cast<uintptr_t>(addr);
    if ((thumb_addr & 1) == 0) {
        thumb_addr |= 1; 
    }
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "hook: %s -> %u", symbol, thumb_addr);
    DobbyHook((void*)thumb_addr, func, origFunc);
}

};

#define HOOK(symbol, newfunc, trampoline) do { \
 \
    void* __newf = reinterpret_cast<void*>(reinterpret_cast<void(*)()>(newfunc)); \
    hooking::hook(symbol, __newf, reinterpret_cast<void**>(&(trampoline))); \
} while (0)


class HookManager {
public:
    static void *getPointerFromSymbol(void *handle, const char *symbol) {
        return reinterpret_cast<void *>(dlsym(handle, symbol));
    }
};