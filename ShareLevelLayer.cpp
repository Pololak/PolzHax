#include "ShareLevelLayer.hpp"

bool __fastcall ShareLevelLayer::initH(gd::ShareLevelLayer* self, void*, gd::GJGameLevel* level) {
	if (!ShareLevelLayer::init(self, level)) return false;

	auto descriptionTextArea = self->m_mainLayer->getChildByType<gd::TextArea*>(1);
	std::cout << descriptionTextArea << std::endl;
	if (descriptionTextArea) {
		if (level->m_levelDesc.empty()) {
			descriptionTextArea->setString("(No description provided)");
		}
		else {
			descriptionTextArea->setString(level->m_levelDesc.c_str());
		}
	}

	return true;
}

void ShareLevelLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf9630), ShareLevelLayer::initH, reinterpret_cast<void**>(&ShareLevelLayer::init));
}