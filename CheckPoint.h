#pragma once

#include "pch.h"

struct CheckPointStorage {

    float rotation;
    double velocity;

    static CheckPointStorage from(gd::PlayerObject* player) {
        return (CheckPointStorage({
            player->getRealRotation(),
            player->getYVelocity(),
            }));
    }

    void restore(gd::PlayerObject* player) {
        player->setRealRotation(rotation);
        player->setYVelocity(velocity);
        //*(float*)((size_t)location + 0x18) = rotation;
        //*(float*)((size_t)location + 0x1c) = rotation_2;

        //*(float*)((size_t)location + 0x25c) = size;
        //*(float*)((size_t)location + 0x260) = size_2;

        //*(double*)((size_t)location + 0x458) = velocity;

        //*(float*)((size_t)location + 0x470) = size_3;
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

struct ObjectBlending {
    ccBlendFunc col1;
    ccBlendFunc col2;
    ccBlendFunc col3;
    ccBlendFunc col4;
    ccBlendFunc dline;
};
//#pragma once
//
//struct CheckPointStorage {
//	double velocity;
//
//	float rotation;
//	float rotation_2;
//
//	float size;
//	float size_2;
//
//	static CheckPointStorage from(gd::PlayLayer* playLayer) {
//		return (CheckPointStorage({
//			*(double*)((size_t)playLayer + 0x45c), // 0x458
//
//			*(float*)((size_t)playLayer + 0x018), // 0x18
//			*(float*)((size_t)playLayer + 0x01C), // 0x1C
//
//			*(float*)((size_t)playLayer + 0x25c), // size
//			*(float*)((size_t)playLayer + 0x260), // size
//			}));
//	}
//
//	void restore(gd::PlayLayer* playLayer) {
//		*(double*)((size_t)playLayer + 0x45c) = velocity;
//
//		*(float*)((size_t)playLayer + 0x018) = rotation;
//		*(float*)((size_t)playLayer + 0x01C) = rotation_2;
//
//		*(float*)((size_t)playLayer + 0x25c) = size;
//		*(float*)((size_t)playLayer + 0x260) = size_2;
//	}
//};
//
//struct CheckPoint {
//	CheckPointStorage p1;
//	CheckPointStorage p2;
//
//	static CheckPoint from(void* playLayer) {
//		void* Player1 = *(void**)((char*)playLayer + 0x2a4);
//		void* Player2 = *(void**)((char*)playLayer + 0x2a8);
//
//		return (
//			CheckPoint({
//				CheckPointStorage::from(Player1),
//				CheckPointStorage::from(Player2)
//				})
//			);
//	}
//
//	void restore(gd::PlayLayer* playLayer) {
//		p1.restore(*(void**)((char*)playLayer + 0x2a4));
//		p2.restore(*(void**)((char*)playLayer + 0x2a8));
//	}
//};