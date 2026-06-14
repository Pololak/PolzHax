cmake_minimum_required(VERSION 3.21)

add_library(cocos2dx INTERFACE)

target_include_directories(cocos2dx INTERFACE
    cocos2dx
    cocos2dx/include
    cocos2dx/kazmath
    cocos2dx/kazmath/include
    cocos2dx/platform/android
    cocos2dx/platform/third_party/android
    cocos2dx/extensions
)