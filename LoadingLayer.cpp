#include "LoadingLayer.hpp"

const char* __fastcall LoadingLayer::getLoadingStringH(gd::LoadingLayer* self) {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	if (tm.tm_mday == 5 && tm.tm_mon == 6) {
		return "Happy birthday PolzHax!";
	}

	if (tm.tm_mday == 9 && tm.tm_mon == 10) {
		return "Happy birthday 1.9!";
	}

	if (tm.tm_mday == 14 && tm.tm_mon == 11) {
		return "Happy birthday Pololak!";
	}

	return LoadingLayer::getLoadingString(self);
}

void LoadingLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xabef0), LoadingLayer::getLoadingStringH, reinterpret_cast<void**>(&LoadingLayer::getLoadingString));
}