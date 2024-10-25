#include "state.h"

SettingStruct& setting() {
	static SettingStruct inst;
	return inst;
}

LayersStruct& layers() {
	static LayersStruct inst;
	return inst;
}

FuckThis& fuckThis() {
	static FuckThis inst;
	return inst;
}