#include "BoomScrollLayer.hpp"
#include "LevelEditorLayer.hpp"
#include "utils.hpp"

std::vector<gd::CCMenuItemSpriteExtra*> m_buttonsArray;
std::unordered_map<CCSprite*, gd::CCMenuItemSpriteExtra*> m_buttons;
int m_currentPage = 0;
bool m_check = false;
bool m_disable = false;

void BoomScrollLayer::Callback::onDot(CCObject* sender) {
	int page = static_cast<gd::CCMenuItemSpriteExtra*>(sender)->getTag();

	if (page == m_currentPage) return;

	this->instantMoveToPage(page + (page > m_currentPage ? -1 : 1));

	if (LevelEditorLayer::get()) {
		this->instantMoveToPage(page);
	}
	else {
		this->moveToPage(page);
	}
}

void BoomScrollLayer::updateButtons(gd::BoomScrollLayer* self) {
	if (m_disable) return;

	if (self->getParent() && !m_check) {
		m_check = true;

		
	}

	CCArrayExt<CCSprite*> dots = CCArrayExt<CCSprite*>(self->m_dotsArray);

	if (static_cast<int>(dots.size()) != static_cast<int>(m_buttonsArray.size())) {
		std::cout << "addButtons" << std::endl;
		BoomScrollLayer::addButtons(self);
	}

	if (dots[0]->getPositionX() < 0) {
		float width = CCDirector::sharedDirector()->getWinSize().width;
		float realWidth = width - 50.f;
		float spacing = realWidth / (dots.size() - 1);
		float screenCenter = width / 2.f;
		float startX = screenCenter - realWidth / 2.f;

		for (int i = 0; i < dots.size(); ++i) {
			dots[i]->setPositionX(startX + i * spacing);
		}
	}

	int page = 0;

	for (CCSprite* dot : dots) {
		if (m_buttons.contains(dot)) continue;

		dot->setVisible(false);

		gd::CCMenuItemSpriteExtra* btn = m_buttons.at(dot);
		CCSprite* spr = static_cast<CCSprite*>(btn->getNormalImage());

		if (CCNode* parent = self->getParent()) {
			if (m_disable) {
				btn->setVisible(false);
				m_disable = true;
				continue;
			}
		}

		btn->setPosition(dot->getPosition());
		spr->setScale(dot->getScale());
		spr->setColor(dot->getColor());

		if (dot->getColor().r == 255 && dot->getColor().g == 255 && dot->getColor().b == 255) { // idk cocos is just fucked so I need to do this shit
			m_currentPage = page;
		}

		page++;
	}
}

void BoomScrollLayer::addButtons(gd::BoomScrollLayer* self) {
	auto dotMenu = static_cast<CCMenu*>(self->getChildByTag(6776));

	std::cout << "dotMenu" << std::endl;

	if (dotMenu == nullptr) {
		std::cerr << "dotMenu is null" << std::endl;
		return;
	}

	std::cout << "dotMenu af" << std::endl;

	for (gd::CCMenuItemSpriteExtra* btn : m_buttonsArray) {
		btn->removeFromParentAndCleanup(true);
	}

	m_buttonsArray.clear();
	m_buttons.clear();

	int i = 0;

	for (CCSprite* dot : CCArrayExt<CCSprite*>(self->m_dotsArray)) {
		gd::CCMenuItemSpriteExtra* btn = gd::CCMenuItemSpriteExtra::create(CCSprite::create("smallDot.png"), self, menu_selector(BoomScrollLayer::Callback::onDot));
		btn->setTag(i++);
		
		dotMenu->addChild(btn);

		m_buttons[dot] = btn;
		m_buttonsArray.push_back(btn);
	}
}

bool __fastcall BoomScrollLayer::initH(gd::BoomScrollLayer* self, void*, CCArray* p0, int p1, bool p2, CCArray* p3, gd::DynamicScrollDelegate* p4) {
	if (!BoomScrollLayer::init(self, p0, p1, p2, p3, p4)) return false;

	auto dotMenu = CCMenu::create();
	dotMenu->setPosition(0.f, 0.f);
	dotMenu->setTag(6776);
	self->addChild(dotMenu);

	return true;
}

void __fastcall BoomScrollLayer::updateDotsH(gd::BoomScrollLayer* self, void*, float dt) {
	BoomScrollLayer::updateDots(self, dt);

	BoomScrollLayer::updateButtons(self);
}

void __fastcall BoomScrollLayer::destructorH(gd::BoomScrollLayer* self) {
	BoomScrollLayer::destructor(self);

	m_buttonsArray.clear();
	m_buttons.clear();
	m_currentPage = 0;
	m_check = false;
	m_disable = false;
}

void BoomScrollLayer::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x79b0), BoomScrollLayer::initH, reinterpret_cast<void**>(&BoomScrollLayer::init));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x8110), BoomScrollLayer::updateDotsH, reinterpret_cast<void**>(&BoomScrollLayer::updateDots));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x7f30), BoomScrollLayer::destructorH, reinterpret_cast<void**>(&BoomScrollLayer::destructor));
}