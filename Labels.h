#include "pch.h"
#include <vector>

class Labels {
public:
	CCMenu* m_labelMenu = nullptr;
	std::vector<CCLabelBMFont*> m_labels = {};
public:
	void updateLabelPositions();
	inline void createFpsLabel();
	void updateLabels(gd::PlayLayer* playLayer);
	static Labels* create();
};