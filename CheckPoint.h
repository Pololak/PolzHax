#pragma once

struct CheckPointStorage {
	double velocity;

	float rotation;
	float rotation_2;

	float size;
	float size_2;

	static CheckPointStorage from(void* location) {
		return (CheckPointStorage({
			*(double*)((size_t)location + 0x458), // 0x458

			*(float*)((size_t)location + 0x018), // 0x18
			*(float*)((size_t)location + 0x01C), // 0x1C

			*(float*)((size_t)location + 0x24), // size
			*(float*)((size_t)location + 0x260), // size
			}));
	}

	void restore(void* location) {
		*(double*)((size_t)location + 0x458) = velocity;

		*(float*)((size_t)location + 0x018) = rotation;
		*(float*)((size_t)location + 0x01C) = rotation_2;

		*(float*)((size_t)location + 0x24) = size;
		*(float*)((size_t)location + 0x260) = size_2;
	}
};

struct CheckPoint {
	CheckPointStorage p1;
	CheckPointStorage p2;

	static CheckPoint from(void* location) {
		void* Player1 = *(void**)((char*)location + 0x2a4);
		void* Player2 = *(void**)((char*)location + 0x2a8);

		return (
			CheckPoint({
				CheckPointStorage::from(Player1),
				CheckPointStorage::from(Player2)
				})
			);
	}

	void restore(void* location) {
		p1.restore(*(void**)((char*)location + 0x2a4));
		p2.restore(*(void**)((char*)location + 0x2a8));
	}
};