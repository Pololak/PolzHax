#include "../libs/cocos2dx/include/cocos2d.h"
#include "../libs/cocos2dx/extensions/cocos-ext.h"
#include "../libs/gdh/include/gd.h"
#include "../libs/Dobby/include/dobby.h"
#include <../../../../android-ndk/android-ndk-r17c/sources/cxx-stl/gnu-libstdc++/4.9/include/experimental/string_view>
#include "hooking.h"
#include "logger.h"
#include "patching.h"
#include "jni.h"

#define GDBASE "libcocos2dcpp.so"
#define ANDROID32_OFFSET 0x10000

using namespace cocos2d;