#pragma once
#include "pch.h"

namespace MenuLayer {
	inline bool(__thiscall* MenuLayer_init)(CCLayer* self);
	bool __fastcall MenuLayer_init_H(CCLayer* self, void*);

	inline bool(__thiscall* MenuLayer_bottomMenu)();
	void __fastcall MenuLayer_bottomMenu_H();

	class Callback {
	public:
		void FLAlertBtn(CCObject*);
		void FLAlertGeode(CCObject*);
		void zero(CCObject*);
		void one(CCObject*);
		void two(CCObject*);
	};
}

//class MenuLayer : public CCLayer, gd::FLAlertLayerProtocol {
//public:
//	static inline bool(__thiscall* init)(CCLayer* self);
//	static bool __fastcall hook(CCLayer* self);
//	void FLAlertBtn(CCObject*);
//	void onTrailerBtn(CCObject*);
//	//void EndLayer(CCObject*);
//	
//};

//namespace MenuLayer
//{
//	inline bool(__thiscall* init)(CCLayer* self);
//	bool __fastcall hook(CCLayer* self);
//}

//class MenuLayer : public CCLayer {
//protected:
//	virtual bool init();
//public:
//	static MenuLayer* create();
//	void customButtonCallback(CCObject*);
//	void callback(CCObject*);
//};

//class MenuLayer : public CCLayer, public gd::FLAlertLayerProtocol { // or cocos2d::CCLayer if you didn't add "using namespace cocos2d" in pch.h
//protected:
//	virtual bool init();
//	void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2) override;
//public:
//	static inline bool(__thiscall* init)(CCLayer* self); // PauseLayer::init (Func in GD)
//	static bool __fastcall hook(CCLayer* self); // What will run after PauseLayer::init
//	void customButtonCallback(CCObject*);
//	void callback(CCObject*);
//};