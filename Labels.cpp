#include "Labels.h"
#include "state.h"

void Labels::updateLabelPositions() {
	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	int i = 0;

	for (auto label : m_labels) {
		if (label->isVisible()) {
			label->setOpacity(setting().labelsOpacity);
			label->setPositionY(0 - (i * (17 * (setting().labelsScale * 2))));
			label->setScale(setting().labelsScale);
			i++;
		}
	}
}