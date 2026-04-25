#include "ColorChannelSprite.hpp"

bool ColorChannelSprite::init(const char* label) {
	if (label) {
		CCLabelBMFont* colorLabel = CCLabelBMFont::create(label, "bigFont.fnt");
		colorLabel->setPosition(15.f, 16.f);
		colorLabel->setScale(.4f);
		this->addChild(colorLabel);
	}

	m_blendingDot = CCLabelBMFont::create("•", "bigFont.fnt");
	m_blendingDot->setScale(.5f);
	m_blendingDot->setPosition(5.5f, 26.5f);
	this->addChild(m_blendingDot);

	this->updateValues(ccWHITE);

	return this->initWithSpriteFrameName("playerSquare_001.png");
}

void ColorChannelSprite::updateBlending(bool blending) {
	if (m_blendingDot) {
		m_blendingDot->setVisible(blending);
	}
}

void ColorChannelSprite::updateValues(ccColor3B color, bool blending) {
	this->setColor(color);
	this->updateBlending(blending);
}