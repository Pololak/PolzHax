#ifndef __LEVELSETTINGSOBJECT_H__
#define __LEVELSETTINGSOBJECT_H__

#include <gd.h>

namespace gd {
    class ColorAction;

    enum Gamemode {
        kGamemodeCube = 0,
        kGamemodeShip = 1,
        kGamemodeBall = 2,
        kGamemodeUfo = 3,
        kGamemodeWave = 4
    };

    enum Speed {
        kSpeedNormal = 0,
        kSpeedSlow = 1,
        kSpeedFast = 2,
        kSpeedFaster = 3
    };

    class LevelSettingsObject : public cocos2d::CCNode {
    public:
        ColorAction* m_background_color;
        ColorAction* m_ground_color;
        ColorAction* m_line_color;
        ColorAction* m_object_color;
        ColorAction* m_3dl_color;
        ColorAction* m_color1;
        ColorAction* m_color2;
        ColorAction* m_color3;
        ColorAction* m_color4;

        auto getBGColor() {
            return from<int*>(this, 0xe8);
        }

        auto getGamemode() {
            return from<int>(this, 0x10C);
        }
        auto setGamemode(int gamemode) {
            from<int>(this, 0x10C) = gamemode;
        }

        auto getSpeed() {
            return from<int>(this, 0x110);
        }
        auto setSpeed(int speed) {
            from<int>(this, 0x110) = speed;
        }

        auto getMini() {
            return from<bool>(this, 0x114);
        }
        auto setMini(bool isMini) {
            from<bool>(this, 0x114) = isMini;
        }

        auto getDual() {
            return from<bool>(this, 0x115);
        }
        auto setDual(bool isDual) {
            from<bool>(this, 0x115) = isDual;
        }

        auto get2PMode() {
            return from<bool>(this, 0x116);
        }
        auto set2PMode(bool is2P) {
            from<bool>(this, 0x116) = is2P;
        }

        auto getSongOffset() {
            return from<float>(this, 0x118);
        }
        auto setSongOffset(float offset) {
            from<float>(this, 0x118) = offset;
        }

        auto getSongFadeIn() {
            return from<bool>(this, 0x11C);
        }
        auto setSongFadeIn(bool isFadeIn) {
            from<bool>(this, 0x11C) = isFadeIn;
        }

        auto getSongFadeOut() {
            return from<bool>(this, 0x11D);
        }
        auto setSongFadeOut(bool isFadeOut) {
            from<bool>(this, 0x11D) = isFadeOut;
        }

        auto getIsFlipped() {
            return from<bool>(this, 0x129);
        }
        auto getIsFlipped(bool isFlipped) {
            from<bool>(this, 0x129) = isFlipped;
        }

        /*auto getGJGameLevel() {
            return from<gd::GJGameLevel*>(this, 0x12C);
        }*/
    };
}

#endif
