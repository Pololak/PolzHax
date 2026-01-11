#include "LevelSearchLayer.hpp"

gd::GJSearchObject* __fastcall LevelSearchLayer::getSearchObjectH(gd::LevelSearchLayer* self, void*, gd::SearchType searchType, std::string searchQuery) {
	auto searchObject = LevelSearchLayer::getSearchObject(self, searchType, searchQuery);

	searchObject->m_length.clear();



	searchObject->m_length += ",4";

	return searchObject;
}

void LevelSearchLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xa1b30), LevelSearchLayer::getSearchObjectH, reinterpret_cast<void**>(&LevelSearchLayer::getSearchObject));
}