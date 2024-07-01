#pragma once
#include "pch.h"

struct CheckPointStorage {
    double velocity;

    float rotation;
    float rotation_2;

    float size;
    float size_2;

    static CheckPointStorage from(void* location) {
        return (CheckPointStorage({
            *(double*)((size_t)location + 0x628), // vel_y 

            *(float*)((size_t)location + 0x020), // rot
            *(float*)((size_t)location + 0x024), // rot

            *(float*)((size_t)location + 0x35C), // size
            *(float*)((size_t)location + 0x644), // size
            }));
    }

    void restore(void* location) {
        *(double*)((size_t)location + 0x628) = velocity;

        *(float*)((size_t)location + 0x020) = rotation;
        *(float*)((size_t)location + 0x024) = rotation_2;

        *(float*)((size_t)location + 0x35C) = size;
        *(float*)((size_t)location + 0x644) = size_2;
    }
};

struct CheckPoint {
    CheckPointStorage p1;
    CheckPointStorage p2;

    static CheckPoint from(void* location) {
        void* Player1 = *(void**)((char*)location + 0x224);
        void* Player2 = *(void**)((char*)location + 0x228);

        return (
            CheckPoint({
                CheckPointStorage::from(Player1),
                CheckPointStorage::from(Player2)
                })
            );
    }

    void restore(void* location) {
        p1.restore(*(void**)((char*)location + 0x224));
        p2.restore(*(void**)((char*)location + 0x228));
    }
};