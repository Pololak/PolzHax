//#include "ModMenuLayer.h"
//
//ModMenuLayer* ModMenuLayer::create() {
//	auto ret = new ModMenuLayer();
//	if (ret && ret->init()) {
//		ret->autorelease();
//	}
//	else {
//		delete ret;
//		ret = nullptr;
//	}
//	return ret;
//}
//
//bool ModMenuLayer::init() {
//	auto director = CCDirector::sharedDirector();
//	auto size = director->getWinSize();
//	auto label = CCLabelBMFont::create("PolzHax Alpha", "bigFont.fnt");
//
//	label->setPosition({ size.width / 2, size.height / 2 });
//	addChild(label);
//
//	auto bgSprite = CCSprite::create("GJ_gradientBG.png");
//	auto bgSize = bgSprite->getContentSize();
//
//	bgSprite->setScaleX(size.width / size.width);
//	bgSprite->setScaleY(size.height / size.height);
//
//	bgSprite->setAnchorPoint({ 0,0 });
//	bgSprite->setColor({ 100,100,100 });
//	bgSprite->setZOrder(-1);
//	addChild(bgSprite);
//
//	auto button = gd::CCMenuItemSpriteExtra::create(
//		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
//		nullptr,
//		this,
//		menu_selector(ModMenuLayer::backButtonCallback)
//	);
//
//	auto menu = CCMenu::create();
//	menu->addChild(button);
//	menu->setPosition({ 25, size.height - 25 });
//
//	addChild(menu);
//
//	setKeypadEnabled(true);
//
//	return true;
//}
//
//void ModMenuLayer::keyBackClicked() {
//	CCDirector::sharedDirector()->popScene();
//}
//
//void ModMenuLayer::backButtonCallback(CCObject* object) {
//	keyBackClicked();
//}
//
//void ModMenuLayer::switchCustomLayerButton(CCObject* object) {
//	auto layer = ModMenuLayer::create();
//	auto scene = CCScene::create();
//	scene->addChild(layer);
//	auto transition = CCTransitionFade::create(0.5f, scene);
//	CCDirector::sharedDirector()->pushScene(transition);
//}