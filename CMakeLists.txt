cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_STL "gnustl_static")
set(ANDROID_ABI armeabi-v7a)

project(polzhax)

add_library(polzhax SHARED
    ./src/KittyMemory/KittyMemory.cpp
    ./src/KittyMemory/MemoryPatch.cpp
    ./src/KittyMemory/KittyUtils.cpp
    ./src/KittyMemory/MemoryBackup.cpp
    ./src/main.cpp
    ./src/Hooks/CustomizeObjectLayerHook.cpp
    ./src/Hooks/EditorPauseLayerHook.cpp
    ./src/Hooks/EditorUIHook.cpp
    ./src/Hooks/GameObjectHook.cpp
    ./src/Hooks/LevelEditorLayerHook.cpp
)

target_include_directories(polzhax PUBLIC 
    libs/cocos2dx
    libs/cocos2dx/include
    libs/cocos2dx/kazmath/include
    libs/cocos2dx/platform/android
    libs/cocos2dx/extensions
    libs/gdh
    libs/gdh/include
    libs/fmod
    libs/Dobby
    libs/Dobby/include
    libs/KittyMemory
)

set(DCMAKE_BUILD_TYPE Debug)
set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_definitions(-DLOGGING_DISABLE -DNDEBUG)
add_subdirectory(libs/Dobby)

target_link_libraries(polzhax PRIVATE dobby)
target_link_libraries(polzhax PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libcocos2dcpp.so
    ${CMAKE_CURRENT_SOURCE_DIR}/libfmod.so
)
target_compile_options(polzhax PUBLIC -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.)