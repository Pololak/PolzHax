#pragma once

#include "pch.h"

struct CheckPointStorage {
    float rot;
    float rot_2;

    float size;
    float size_2;

    double velocity;

    static CheckPointStorage from(gd::PlayerObject* player) {
        return (CheckPointStorage({
            *(float*)((size_t)player + 0x18), // rot
            *(float*)((size_t)player + 0x1C), // rot

            *(float*)((size_t)player + 0x25C), // size
            *(float*)((size_t)player + 0x260), // size

            *(double*)((size_t)player + 0x458),

            }));
    }

    void restore(gd::PlayerObject* player) {
        *(double*)((size_t)player + 0x458) = velocity;

        *(float*)((size_t)player + 0x018) = rot;
        *(float*)((size_t)player + 0x01c) = rot_2;

        *(float*)((size_t)player + 0x25C) = size;
        *(float*)((size_t)player + 0x260) = size_2;
    }
};

struct CheckPoint {
    CheckPointStorage p1;
    CheckPointStorage p2;

    static CheckPoint from(gd::PlayLayer* playLayer) {
        gd::PlayerObject* player1 = playLayer->player1();
        gd::PlayerObject* player2 = playLayer->player2();

        return (
            CheckPoint({
                CheckPointStorage::from(player1),
                CheckPointStorage::from(player2)
                })
            );
    }

    void restore(gd::PlayLayer* playLayer) {
        p1.restore(playLayer->player1());
        p2.restore(playLayer->player2());
    }
};

struct ObjectBlendingStorage {
    bool col1_blend;
    bool col2_blend;
    bool col3_blend;
    bool col4_blend;
    bool dl_blend;

    static ObjectBlendingStorage from(gd::PlayLayer* playLayer) {
        return (ObjectBlendingStorage({
            *(bool*)((size_t)playLayer + 0x329),
            *(bool*)((size_t)playLayer + 0x32a),
            *(bool*)((size_t)playLayer + 0x32b),
            *(bool*)((size_t)playLayer + 0x32c),
            *(bool*)((size_t)playLayer + 0x32d),
            }));
    }

    void restore(gd::PlayLayer* playLayer) {
        *(bool*)((size_t)playLayer + 0x329) = col1_blend;
        *(bool*)((size_t)playLayer + 0x32a) = col2_blend;
        *(bool*)((size_t)playLayer + 0x32b) = col3_blend;
        *(bool*)((size_t)playLayer + 0x32c) = col4_blend;
        *(bool*)((size_t)playLayer + 0x32d) = dl_blend;
    }
};

struct ObjectBlending {
    ObjectBlendingStorage obj;

    static ObjectBlending from(gd::PlayLayer* playLayer) {
        return (
            ObjectBlending({
                ObjectBlendingStorage::from(playLayer)
                })
            );
    }

    void restore(gd::PlayLayer* playLayer) {
        obj.restore(playLayer);
    }
};