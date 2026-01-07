#ifndef __LEVELSETTINGSOBJECT_H__
#define __LEVELSETTINGSOBJECT_H__

#include <gd.h>

namespace gd {

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

    class ColorAction;

    class LevelSettingsObject : public cocos2d::CCNode {
    public:
        ColorAction* m_backgroundColor; // 0xe8
        ColorAction* m_groundColor; // 0xec
        ColorAction* m_lineColor; // 0xf0
        ColorAction* m_objectColor; // 0xf4
        ColorAction* m_dlColor; // 0xf8
        ColorAction* m_customColor1; // 0xfc
        ColorAction* m_customColor2; // 0x100
        ColorAction* m_customColor3; // 0x104
        ColorAction* m_customColor4; // 0x108
        int m_startMode; // 0x10c
        int m_startSpeed; // 0x110
        bool m_startMini; // 0x114
        bool m_startDual; // 0x115
        bool m_twoPlayerMode; // 0x116
        PAD(1)
        float m_songOffset; // 0x118
        bool m_fadeIn; // 0x11c
        bool m_fadeOut; // 0x11d
        PAD(2)
        int m_BGIdx; // 0x120
        int m_GIdx; // 0x124
        bool m_startsWithStartPos; // 0x128
        bool m_isFlipped; // 0x129
        PAD(2)
        GJGameLevel* m_level; // 0x12c
        gd::string m_guidelineString; // 0x130
        bool m_songChanged; // 0x148

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

        ColorAction* getColorObjectBG() {
            return from<ColorAction*>(this, 0xe8);
        }

        /*auto getGJGameLevel() {
            return from<gd::GJGameLevel*>(this, 0x12C);
        }*/
    };
}

#endif
