#include "NoclipInfoLayer.hpp"
#include "PlayLayer.hpp"
#include "utils.hpp"

bool NoclipInfoLayer::init() {
	m_currentPage = 0;
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	m_runLabels = CCArray::create();
	m_runLabels->retain();

	auto* director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	director->getTouchDispatcher()->incrementForcePrio();
	this->registerWithTouchDispatcher();

	auto layer = CCLayer::create();
	auto menu = CCMenu::create();
	this->m_mainLayer = layer;
	this->m_buttonMenu = menu;

	layer->addChild(menu);
	this->addChild(layer);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(NoclipInfoLayer::onClose));
	onClose->setScale(.8f);
	onClose->setPosition(-85.f, 105.f);
	menu->addChild(onClose);

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ 180.f, 220.f });
	bg->setPosition({ winSize.width / 2.f, winSize.height / 2.f });
	layer->addChild(bg, -2);

	auto title = CCLabelBMFont::create("Noclip", "bigFont.fnt");
	title->setScale(.75f);
	title->setPosition({ winSize.width / 2.f, winSize.height / 2.f + 90.f });
	layer->addChild(title);

	m_pageLabel = CCLabelBMFont::create("Page: 1/1", "goldFont.fnt");
	m_pageLabel->setScale(.6f);
	m_pageLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 70.f);
	layer->addChild(m_pageLabel);
	this->updatePageLabel();

	auto bg2 = extension::CCScale9Sprite::create("square02_001.png");
	bg2->setContentSize({ 160.f, 160.f });
	bg2->setOpacity(100);
	bg2->setPosition({ winSize.width / 2.f, winSize.height / 2.f - 20.f });
	layer->addChild(bg2, -2);

	auto onPrevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onPrevSpr->setScale(.8f);
	auto onPrev = gd::CCMenuItemSpriteExtra::create(onPrevSpr, this, menu_selector(NoclipInfoLayer::onPrev));
	onPrev->setPosition(-110.f, -20.f);
	menu->addChild(onPrev);

	auto onNextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	onNextSpr->setFlipX(true);
	onNextSpr->setScale(.8f);
	auto onNext = gd::CCMenuItemSpriteExtra::create(onNextSpr, this, menu_selector(NoclipInfoLayer::onNext));
	onNext->setPosition(110.f, -20.f);
	menu->addChild(onNext);

	for (int i = 0; i < 10; ++i) {
		auto runLabel = CCLabelBMFont::create("AAA", "bigFont.fnt");
		runLabel->setScale(.45f);
		runLabel->setPosition(winSize.width / 2.f, winSize.height / 2.f + 50.f - (15.f * i));
		layer->addChild(runLabel);
		m_runLabels->addObject(runLabel);
	}

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	this->loadPage();

	return true;
}

void NoclipInfoLayer::keyBackClicked() {
	if (m_runLabels) {
		m_runLabels->release();
	}

	this->setTouchEnabled(false);
	this->setKeypadEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void NoclipInfoLayer::onClose(CCObject*) {
	this->keyBackClicked();
}

void NoclipInfoLayer::updatePageLabel() {
	auto noclipRunsCount = PlayLayer::getNoclipPercentage().size();
	if (noclipRunsCount) {
		m_pageLabel->setString(CCString::createWithFormat("Page: %i/%i", m_currentPage + 1, (noclipRunsCount % 10 == 0) ? noclipRunsCount / 10 : noclipRunsCount / 10 + 1)->getCString());
	}
}

void NoclipInfoLayer::loadPage() {
	auto noclipRuns = PlayLayer::getNoclipPercentage();

	if (m_runLabels && m_runLabels->count()) {
		for (auto label : CCArrayExt<CCLabelBMFont*>(m_runLabels)) {
			if (label) {
				label->setString("");
			}
		}
	}

	if (noclipRuns.empty()) return;

	if (m_currentPage < 0) m_currentPage = 0;

	int count = 0;

	std::cout << "Current Page: " << m_currentPage << std::endl;
	for (int i = m_currentPage * 10; i < noclipRuns.size(); ++i) {
		std::cout << noclipRuns[i].first << "% - " << noclipRuns[i].second << "%" << std::endl;

		if (m_runLabels && m_runLabels->count()) {
			auto label = static_cast<CCLabelBMFont*>(m_runLabels->objectAtIndex(count));
			if (label) {
				label->setString(CCString::createWithFormat("%.2f%% - %.2f%%", noclipRuns[i].first, noclipRuns[i].second)->getCString());
			}
		}

		++count;
		if (count >= 10) {
			break;
		}
	}
	if (m_currentPage > 0) {
		if (count == 0) {
			m_currentPage--;
			this->loadPage();
			return;
		}
	}

	this->updatePageLabel();
}

void NoclipInfoLayer::onPrev(CCObject*) {
	if (m_currentPage == 0) return;

	m_currentPage--;
	this->loadPage();
}

void NoclipInfoLayer::onNext(CCObject*) {
	if (m_currentPage == (PlayLayer::getNoclipPercentage().size() / 10)) return;

	m_currentPage++;
	this->loadPage();
}