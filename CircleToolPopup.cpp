#include "CircleToolPopup.hpp"
#include "LevelEditorLayer.hpp"

static float m_angle;
static float m_step;

bool CircleToolPopup::init() {
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

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

	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ 300.f, 220.f });
	bg->setPosition(winSize / 2.f);
	layer->addChild(bg, -2);

	auto onCloseSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	onCloseSpr->setScale(.8f);
	auto onClose = gd::CCMenuItemSpriteExtra::create(onCloseSpr, this, menu_selector(CircleToolPopup::onClose));
	onClose->setPosition(-145.f, 105.f);
	menu->addChild(onClose);

	auto title = CCLabelBMFont::create("Circle Tool", "goldFont.fnt");
	title->setPosition(winSize.width / 2.f, winSize.height / 2.f + 95.f);
	title->setScale(.75f);
	layer->addChild(title);

	auto arcLabel = CCLabelBMFont::create("Arc", "goldFont.fnt");
	arcLabel->setPosition(winSize.width / 2.f - 60.f, winSize.height / 2.f + 64.f);
	arcLabel->setScale(.75f);
	layer->addChild(arcLabel);

	auto angleBg = extension::CCScale9Sprite::create("square02_small.png");
	angleBg->setOpacity(100);
	angleBg->setContentSize({ 60, 30.f });
	angleBg->setPosition(winSize.width / 2.f - 60.f, winSize.height / 2.f + 38.f);
	layer->addChild(angleBg);

	m_angleInput = gd::CCTextInputNode::create(60, 30, "Val", this, "bigFont.fnt");
	m_angleInput->setString(CCString::createWithFormat("%.0f", m_angle)->getCString());
	m_angleInput->m_delegate = this;
	m_angleInput->setPosition(winSize.width / 2.f - 60.f, winSize.height / 2.f + 38.f);
	m_angleInput->setAllowedChars("0123456789.");
	m_angleInput->setMaxLabelScale(.8f);
	m_angleInput->setLabelPlaceholderScale(.6f);
	m_angleInput->setLabelPlaceholderColor(ccGRAY);
	layer->addChild(m_angleInput);

	auto stepLabel = CCLabelBMFont::create("Step", "goldFont.fnt");
	stepLabel->setPosition(winSize.width / 2.f + 60.f, winSize.height / 2.f + 64.f);
	stepLabel->setScale(.75f);
	layer->addChild(stepLabel);

	auto stepBg = extension::CCScale9Sprite::create("square02_small.png");
	stepBg->setOpacity(100);
	stepBg->setContentSize({ 60, 30.f });
	stepBg->setPosition(winSize.width / 2.f + 60.f, winSize.height / 2.f + 38.f);
	layer->addChild(stepBg);

	m_stepInput = gd::CCTextInputNode::create(60, 30, "Val", this, "bigFont.fnt");
	m_stepInput->setString(CCString::createWithFormat("%.0f", m_step)->getCString());
	m_stepInput->m_delegate = this;
	m_stepInput->setPosition(winSize.width / 2.f + 60.f, winSize.height / 2.f + 38.f);
	m_stepInput->setAllowedChars("0123456789.");
	m_stepInput->setMaxLabelScale(.8f);
	m_stepInput->setLabelPlaceholderScale(.6f);
	m_stepInput->setLabelPlaceholderColor(ccGRAY);
	layer->addChild(m_stepInput);

	auto onApply = gd::CCMenuItemSpriteExtra::create(gd::ButtonSprite::create("Apply", 0, 0, .75f, false, "goldFont.fnt", "GJ_button_01.png", 30.f), this, menu_selector(CircleToolPopup::onApply));
	onApply->setPosition(0.f, -85.f);
	menu->addChild(onApply);

	m_label = CCLabelBMFont::create("", "chatFont.fnt");
	m_label->setAnchorPoint({0.f, .5f});
	m_label->setPosition(winSize.width / 2.f - 120.f, winSize.height / 2.f - 40.f);
	layer->addChild(m_label);
	this->updateLabels();

	auto originalAuthorLbl = CCLabelBMFont::create("Original by Mat", "goldFont.fnt");
	originalAuthorLbl->setScale(0.5f);
	auto originalAuthor = gd::CCMenuItemSpriteExtra::create(originalAuthorLbl, this, menu_selector(CircleToolPopup::onOriginalAuthor));
	originalAuthor->setPosition(ccp(-92, -96));
	menu->addChild(originalAuthor);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);

	return true;
}

void CircleToolPopup::keyBackClicked() {
	this->setKeypadEnabled(false);
	this->setTouchEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void CircleToolPopup::onClose(CCObject*) {
	this->keyBackClicked();
}

void CircleToolPopup::textChanged(gd::CCTextInputNode* input) {
	if (input == m_angleInput) {
		m_angle = std::atof(input->getString().c_str());
	}
	else if (input == m_stepInput) {
		m_step = std::atof(input->getString().c_str());
	}
	this->updateLabels();
	std::cout << "m_angle: " << m_angle << " \nm_step: " << m_step << std::endl;
}

void CircleToolPopup::updateLabels() {
	if (LevelEditorLayer::get()) {
		auto objs = LevelEditorLayer::get()->m_uiLayer->m_selectedObjects;
		const auto amt = static_cast<int>(std::ceilf(m_angle / m_step) - 1.f);
		const auto obj_count = amt * objs->count();
		m_label->setString(("Copies: " + std::to_string(amt) + "\nObjects: " + std::to_string(obj_count)).c_str());
	}
}

void CircleToolPopup::onApply(CCObject*) {
	if (LevelEditorLayer::get()) {
		auto editor = LevelEditorLayer::get()->m_uiLayer;
		auto objs = editor->getSelectedObjects();
		const auto amt = static_cast<int>(std::ceilf(m_angle / m_step) - 1.f);
		if (objs && objs->count()) {
			const auto obj_count = objs->count() * amt;
			if (obj_count > 5000) {
				gd::FLAlertLayer::create(this, "Warning", std::string("This will create <cy>" + std::to_string(obj_count) + "</c> objects, are you sure?").c_str(), "Cancel", "Ok")->show();
			}
			else {
				this->perform();
			}
		}
	}
}

void CircleToolPopup::perform() {
	auto editorLayer = LevelEditorLayer::get();
	if (editorLayer) {
		auto editorUI = editorLayer->m_uiLayer;
		auto objs = CCArray::create();

		for (float i = 1; i * m_step < m_angle; ++i) {
			editorUI->onDuplicate(nullptr);
			auto selected = editorUI->getSelectedObjects();
			editorUI->rotateObjects(selected, m_step, { 0.f, 0.f });

			const float angle = i * m_step;

			editorLayer->m_undoList->removeLastObject();
			objs->addObjectsFromArray(selected);
		}

		editorLayer->m_undoList->addObject(gd::UndoObject::createWithArray(objs, gd::UndoCommand::Paste));
		editorUI->selectObjects(objs);
		this->keyBackClicked();
	}
}