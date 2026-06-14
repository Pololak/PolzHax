#include "../libs/cocos2dx/include/cocos2d.h"
#include "../libs/cocos2dx/extensions/cocos-ext.h"
#include "../libs/gdh/include/gd.h"
#include "../libs/Dobby/include/dobby.h"
#include "hooking.h"
#include "logger.h"
#include "patching.h"
#include "jni.h"

#define GDBASE "libcocos2dcpp.so"

using namespace cocos2d;