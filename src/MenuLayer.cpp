#include "MenuLayer.hpp"
#include "Setting.hpp"

bool PolzMenuLayer::init() {
    if (!MenuLayer::init()) return false;

    if (setting().onAutoSave) {
        setting().save();
    }
    
    return true;
}