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

    // Creator
    void onEditorExtension() {
        if (setting().onEditorExtension) {
            MemoryPatch::createWithHex(GDBASE, 0x1f3776 - ANDROID32_OFFSET, "08 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f3788 - ANDROID32_OFFSET, "08 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f37a2 - ANDROID32_OFFSET, "08 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f37b4 - ANDROID32_OFFSET, "08 bf").Modify();

            MemoryPatch::createWithHex(GDBASE, 0x1faeac - ANDROID32_OFFSET, "08 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faebe - ANDROID32_OFFSET, "08 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faed4 - ANDROID32_OFFSET, "18 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faeee - ANDROID32_OFFSET, "18 bf").Modify();

            MemoryPatch::createWithHex(GDBASE, 0x1eac20 - ANDROID32_OFFSET, "00 00 00 00").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eac24 - ANDROID32_OFFSET, "00 60 ea 4a").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eb054 - ANDROID32_OFFSET, "00 00 00 00").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eb058 - ANDROID32_OFFSET, "00 60 ea 4a").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1f3776 - ANDROID32_OFFSET, "d8 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f3788 - ANDROID32_OFFSET, "58 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f37a2 - ANDROID32_OFFSET, "d8 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1f37b4 - ANDROID32_OFFSET, "58 bf").Modify();

            MemoryPatch::createWithHex(GDBASE, 0x1faeac - ANDROID32_OFFSET, "48 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faebe - ANDROID32_OFFSET, "48 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faed4 - ANDROID32_OFFSET, "c8 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1faeee - ANDROID32_OFFSET, "c8 bf").Modify();

            MemoryPatch::createWithHex(GDBASE, 0x1eac20 - ANDROID32_OFFSET, "00 00 96 43").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eac24 - ANDROID32_OFFSET, "00 60 6a 47").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eb054 - ANDROID32_OFFSET, "00 00 96 43").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1eb058 - ANDROID32_OFFSET, "00 60 6a 47").Modify();
        }
    }

    void onObjectBypass() {
        if (setting().onObjectBypass) {
            MemoryPatch::createWithHex(GDBASE, 0x1fb0ec - ANDROID32_OFFSET, "04 e0").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1fbb9c - ANDROID32_OFFSET, "c3 e7").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x49ca15 - ANDROID32_OFFSET, "25 69 20 6f 62 6a 65 63 74 73 00 00 00 00").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1fb0ec - ANDROID32_OFFSET, "04 dd").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1fbb9c - ANDROID32_OFFSET, "c3 dd").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x49ca15 - ANDROID32_OFFSET, "25 69 2f 25 69 20 6f 62 6a 65 63 74 73 00").Modify();
        }
    }

    // Level
    void onNoclip() {
        if (setting().onNoclip) {
            MemoryPatch::createWithHex(GDBASE, 0x1c5f6c - ANDROID32_OFFSET, "00 bf 39 e1").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1c5f6c - ANDROID32_OFFSET, "40 f0 3a 81").Modify();
        }
    }

    void onPracticeMusic() {
        if (setting().onPracticeMusic) {
            MemoryPatch::createWithHex(GDBASE, 0x1c8eba - ANDROID32_OFFSET, "00 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c9050 - ANDROID32_OFFSET, "03 e0").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c60c6 - ANDROID32_OFFSET, "00 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c60f2 - ANDROID32_OFFSET, "00 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c906a - ANDROID32_OFFSET, "00 bf 00 bf").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c757e - ANDROID32_OFFSET, "04 e0").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c7666 - ANDROID32_OFFSET, "00 bf").Modify();
        }
        else {
            MemoryPatch::createWithHex(GDBASE, 0x1c8eba - ANDROID32_OFFSET, "79 d1").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c9050 - ANDROID32_OFFSET, "1d b1").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c60c6 - ANDROID32_OFFSET, "93 b9").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c60f2 - ANDROID32_OFFSET, "1d b9").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c906a - ANDROID32_OFFSET, "e3 f7 eb fe").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c757e - ANDROID32_OFFSET, "23 b1").Modify();
            MemoryPatch::createWithHex(GDBASE, 0x1c7666 - ANDROID32_OFFSET, "44 d1").Modify();
        }
    }

    void loadPatches() {
        // Bypass
        onCharacterFilter();
        onIcons();
        onMainLevels();
        onSliderLimit();
        onTextLength();

        // Creator
        onEditorExtension();
        onObjectBypass();
        
        // Cosmetic
        onNoDeathEffect();

        // Level
        onNoclip();
        onPracticeMusic();
    }
}