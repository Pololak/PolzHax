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
//	static CheckPoint from(gd::PlayLayer* playLayer) {
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