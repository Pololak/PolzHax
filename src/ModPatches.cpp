#include "ModPatches.h"
#include "patching.h"
#include "Setting.h"

namespace ModPatches {
    // Bypass
    void onCharacterFilter() {
        if (setting().onCharacterFilter) {
            MemoryPatch::createWithHex(GDBASE, 0x1b8bc4 - ANDROID32_OFFSET, "21 e0").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1b8bc4 - ANDROID32_OFFSET, "21 d0").Modify();
        }
    }

    void onIcons() {
        if (setting().onIcons) {
            MemoryPatch::createWithHex(GDBASE, 0x1cdbae - ANDROID32_OFFSET, "01 20 1c bd").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1cdaae - ANDROID32_OFFSET, "01 20 1c bd").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1cdbae - ANDROID32_OFFSET, "01 24 03 29").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1cdaae - ANDROID32_OFFSET, "0a b9 04 29").Modify();
        }
    }

    void onMainLevels() {
        if (setting().onMainLevels) {
            MemoryPatch::createWithHex(GDBASE, 0x21df1c - ANDROID32_OFFSET, "00 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x21c0fe - ANDROID32_OFFSET, "66 e0").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x21df1c - ANDROID32_OFFSET, "0c dc").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x21c0fe - ANDROID32_OFFSET, "66 dd").Modify();
        }
    }

    void onSliderLimit() {
        if (setting().onSliderLimit) {
            MemoryPatch::createWithHex(GDBASE, 0x1b3594 - ANDROID32_OFFSET, "0a e0").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1b35bc - ANDROID32_OFFSET, "08 e0").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1b3594 - ANDROID32_OFFSET, "0a d5").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1b35bc - ANDROID32_OFFSET, "08 dd").Modify();
        }
    }

    void onTextLength() {
        if (setting().onTextLength) {
            MemoryPatch::createWithHex(GDBASE, 0x1b8bf0 - ANDROID32_OFFSET, "06 e0").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1b8bf0 - ANDROID32_OFFSET, "06 db").Modify();
        }
    }

    // Cosmetic
    void onNoDeathEffect() {
        if (setting().onNoDeathEffect) {
            MemoryPatch::createWithHex(GDBASE, 0x1de36a - ANDROID32_OFFSET, "00 bf 89 e0").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c601a - ANDROID32_OFFSET, "0c e0").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1de36a - ANDROID32_OFFSET, "00 f0 84 80").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c601a - ANDROID32_OFFSET, "63 b1").Modify();
        }
    }

    void loadPatches() {
        // Bypass
        onCharacterFilter();
        onIcons();
        onMainLevels();
        onSliderLimit();
        onTextLength();
        
        // Cosmetic
        onNoDeathEffect();
    }
}