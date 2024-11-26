#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class CCSpritePlus;

	// jesus fucking christ
	enum GameObjectType {
		kGameObjectTypeSolid = 0,
		kGameObjectTypeHazard = 2,
		kGameObjectTypeInverseGravityPortal = 3,
		kGameObjectTypeNormalGravityPortal = 4,
		kGameObjectTypeShipPortal = 5,
		kGameObjectTypeCubePortal = 6,
		kGameObjectTypeDecoration = 7,
		kGameObjectTypeYellowJumpPad = 8,
		kGameObjectTypePinkJumpPad = 9,
		kGameObjectTypeGravityPad = 10,
		kGameObjectTypeYellowJumpRing = 11,
		kGameObjectTypePinkJumpRing = 12,
		kGameObjectTypeGravityRing = 13,
		kGameObjectTypeInverseMirrorPortal = 14,
		kGameObjectTypeNormalMirrorPortal = 15,
		kGameObjectTypeBallPortal = 16,
		kGameObjectTypeRegularSizePortal = 17,
		kGameObjectTypeMiniSizePortal = 18,
		kGameObjectTypeUfoPortal = 19,
		kGameObjectTypeModifier = 20,
		kGameObjectTypeSecretCoin = 22,
		kGameObjectTypeDualPortal = 23,
		kGameObjectTypeSoloPortal = 24,
		kGameObjectTypeSlope = 25,
		kGameObjectTypeWavePortal = 26,
		kGameObjectTypeRobotPortal = 27,
		kGameObjectTypeTeleportPortal = 28,
		kGameObjectTypeCollectible = 30,
		kGameObjectTypeUserCoin = 31,
		kGameObjectTypeDropRing = 32,
		kGameObjectTypeSpiderPortal = 33,
		kGameObjectTypeRedJumpPad = 34,
		kGameObjectTypeRedJumpRing = 35,
		kGameObjectTypeCustomRing = 36,
		kGameObjectTypeDashRing = 37,
		kGameObjectTypeGravityDashRing = 38,
		kGameObjectTypeCollisionObject = 39,
		kGameObjectTypeSpecial = 40,
	};

	/*class SettingsColorObject : public CCNode {
	public:
		ccColor3B m_color;
		bool m_blending;
		int m_custom;
	};*/

	/*enum GJCustomColorMode {
		Default = 0,
		PlayerCol1 = 1,
		PlayerCol2 = 2,
		LightBG = 5,
		Col1 = 3,
		Col2 = 4,
		Col3 = 6,
		Col4 = 7,
		DL = 8
	};*/

	enum class CustomColorMode {
		Default = 0,
		PlayerCol1 = 1,
		PlayerCol2 = 2,
		LightBG = 5,
		Col1 = 3,
		Col2 = 4,
		Col3 = 6,
		Col4 = 7,
		DL = 8,
		White = 9
	};

	class GJSpriteColor;
	class ColorActionSprite;
	class GJEffectManager;
	//enum class GJCustomColorMode;

	#pragma runtime_checks("s", off)
	class GameObject : public cocos2d::CCSprite {
	public:
		bool m_isFlippedX; // m_flipX
		bool m_isFlippedY; // m_flipY
		cocos2d::CCPoint m_boxOffset;
		OBB2D* m_objectOBB2D;
		bool m_oriented; // m_isOriented
		cocos2d::CCObject* m_glowSprite;
		bool m_isSpeedPortal;
		bool m_isRingObject;
		cocos2d::CCAction* m_myAction;
		bool m_isRotatingObject;
		bool m_objectPoweredOn;
		bool m_hideObject;
		float m_orientedBoxWidth;
		float m_orientedBoxHeight;
		bool m_active; // m_isActive
		bool m_hasGlow;
		bool m_isDestroyed;
		cocos2d::CCParticleSystemQuad* m_particleSystem;
		gd::string m_particleString;
		bool m_particleAdded;
		cocos2d::CCPoint m_portalPosition;
		cocos2d::CCRect m_objectTextureRect;
		bool m_textureRectDirty;
		float m_fadeInPosOffset;
		cocos2d::CCRect m_objectRect2;
		bool m_isObjectRectDirty; // m_objectRectDirty
		bool m_isOrientedRectDirty;
		bool m_hasBeenActivated;
		bool m_activated;
		bool m_canChangeCustomColor;
		bool m_hasCustomChildren;
		bool m_hasColor;
		cocos2d::CCSprite* m_colorSprite;
		bool m_ignoreScreenCheck;
		float m_objectRadius; // m_radius
		bool m_isRotatedSide; // m_isRotated
		float m_scaleModX;
		float m_scaleModY;
		int m_ID;
		GameObjectType m_objectType;
		int m_section; // m_sectionIdx
		bool m_shouldSpawn;
		bool m_touchTriggered;
		cocos2d::CCPoint m_startPosition;
		gd::string m_textureName; // m_frame
		bool m_isDisabled;
		bool m_useAudioScale;
		bool m_sleeping; // m_isSleeping
		float m_startRotation;
		float m_startScaleX;
		float m_startScaleY;
		bool m_startFlipX;
		bool m_startFlipY;
		bool m_shouldHide;
		float m_spawnXPosition; // m_spawnXPos
		bool m_invisible; // m_isInvisible
		float m_enterAngle;
		int m_activeEnterEffect; // m_enterEffect
		cocos2d::ccColor3B m_tintColor;
		float m_tintDuration;
		bool m_tintGround;
		int m_objectID; // m_objectKey
		bool m_isDontEnter; // m_dontTransform
		bool m_isDontFade; // m_dontFade
		bool m_dontFadeTinted;
		bool m_isTintObject;
		bool m_stateVar;
		int m_defaultZOrder; // m_objectZ
		cocos2d::CCNode* m_objectParent;
		bool m_customAudioScale;
		float m_minAudioScale;
		float m_maxAudioScale;
		int m_secretCoinID; // m_uniqueID
		bool m_invisibleMode;
		bool m_glowUseBGColor;
		bool m_useSpecialLight;
		bool m_glowUseReverseColor;
		bool m_isColorObject;
		float m_opacityMod;
		float m_glowOpacityMod;
		bool m_dontShow;
		bool m_upSlope;
		int m_slopeType;
		float m_slopeAngle;
		bool m_hazardousSlope; // m_damaging
		CustomColorMode m_customColorMode;
		CustomColorMode m_defaultColorMode;
		bool m_forceBottomLayer;
		bool m_editorSelected;
		bool m_copyPlayerColor1;
		bool m_copyPlayerColor2;
		bool m_customColorBlend;
		bool m_wasSelected;
		bool m_isSelected;
		int m_editorSelectIdx;
		cocos2d::CCPoint m_storedPosition;
		int m_editorLayer; // m_groupID

	public:
		//CCNode vtable
		virtual void setScaleX(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x72070
				)(this, scale);
		}
		virtual void setScaleY(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x720d0
				)(this, scale);
		}
		virtual void setScale(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x72130
				)(this, scale);
		}
		virtual void setPosition(const cocos2d::CCPoint& pos) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, const cocos2d::CCPoint&)>(
				base + 0x71e60
				)(this, pos);
		}
		virtual void setVisible(bool visible) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, bool)>(
				base + 0x72610
				)(this, visible);
		}
		virtual void setRotation(float rotation) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x71f50
				)(this, rotation);
		}
		virtual bool initWithTexture(cocos2d::CCTexture2D* texture) {
			return reinterpret_cast<bool(__thiscall*)(GameObject*, cocos2d::CCTexture2D*)>(
				base + 0xCFA80
				)(this, texture); //not correct
		}
		//virtual void setChildColor(const cocos2d::ccColor3B& color) {
		//	return reinterpret_cast<void(__thiscall*)(GameObject*, const cocos2d::ccColor3B&)>(
		//		base + 0xEE900
		//		)(this, color); //not correct
		//}

		//CCRGBAProtocol vtable
		virtual void setOpacity(GLubyte opacity) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, GLubyte)>(
				base + 0x72350
				)(this, opacity);
		}

		static GameObject* createWithFrame(const char* frame) {
			return reinterpret_cast<GameObject* (__fastcall*)(const char*)>(
				base + 0xCF8F0 //not correct
				)(frame);
		}
		static GameObject* objectFromString(std::string str, bool unknown) {
			auto pRet = reinterpret_cast<GameObject* (__fastcall*)(std::string, bool)>(
				base + 0xEBE50 //not correct
				)(str, unknown);
			__asm add esp, 0x18
			return pRet;
		}

		cocos2d::CCAction* myAction() {
			return from<cocos2d::CCAction*>(this, 0x160);
		}

		void deselectObject() {
			this->m_isSelected = false;

			return reinterpret_cast<void(__fastcall*)(GameObject*)>(
				base + 0xeee50 //maybe it is lol
			)(this);
		}

		std::string getSaveString() {
			std::string ret;

			reinterpret_cast<void(__thiscall*)(GameObject*, std::string*)>(
				base + 0xed0c0
			)(
				this, &ret
			);

			return ret;
		}

		void addToGroup(int id) {
			reinterpret_cast<void(__thiscall*)(
				GameObject*, int
			)>(
				base + 0xeb8d0
			)(
				this, id
			);
		}

		auto getColorMode() {
			auto active = from<CustomColorMode>(this, 0x308);
			auto default_color = from<CustomColorMode>(this, 0x30c);
			// TODO: gd checks some boolean
			if (active == CustomColorMode::Default)
				active = default_color;
			return active;
		}

		cocos2d::CCRepeatForever* createRotateAction(float f) {
			__asm movss xmm1, f

			auto pRet = reinterpret_cast<cocos2d::CCRepeatForever * (__thiscall*)(GameObject*)>(
				base + 0x72230
				)(this);

			return pRet;
		}

		/*bool isOnValidGroup() {
			if (from<int>(this, 0x160) == 842150450) return 0;
			else return 1;
		}*/

		int getGroup()
		{
			return from<int>(this, 0x324);
		}

		int getType() {
			return from<int>(this, 0x268);
			//return reinterpret_cast<int(__fastcall*)(GameObject*)>(base + 0x52240)(this);
		}
		void setType(int newType) {
			from<int>(this, 0x268) = newType;
		}

		float getObjectRadius() {
			return from<float>(this, 0x254);
		}

		bool getIsFlippedY() {
			return from<bool>(this, 0x1C4);
		}
		bool getIsFlippedX() {
			return from<bool>(this, 0x1C5);
		}

		bool getUseAudioScale() {
			return from<bool>(this, 0x295);
		}

		void updateOrientedBox() {
			return reinterpret_cast<void (__thiscall*)(GameObject*)>(base + 0x75a50)(this);
		}

		gd::OBB2D* getOrientedBox() {
			return reinterpret_cast<gd::OBB2D*(__thiscall*)(GameObject*)>(base + 0x759e0)(this);
		}

		auto& getObjectID() {
			return from<int>(this, 0x2c4);
		}

		int getObjectColor() {
			return from<int>(this, 0x308);
		}

		auto getObjectGroup() {
			return from<int>(this, 0x324);
		}

		auto& triggerColor() {
			return from<cocos2d::ccColor3B>(this, 0x2b8);
		}

		auto& triggerBlending() {
			return from<bool>(this, 0x314);
		}

		auto& triggerP1() {
			return from<bool>(this, 0x312);
		}

		auto& triggerP2() {
			return from<bool>(this, 0x313);
		}

		bool getIsTintObject() const {
			return from<bool>(this, 0x2cb);
		}

		auto& triggerDuration() {
			return from<float>(this, 0x2bc);
		}

		void setObjectColor(cocos2d::ccColor3B color) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, cocos2d::ccColor3B)>(base + 0x75560)(this, color);
		}

		auto getChildSprite() {
			return from<cocos2d::CCSprite*>(this, 0x24c);
		}

		bool getHasColor() const {
			return from<bool>(this, 0x24a);
		}

		void setChildColor(cocos2d::ccColor3B color) {
			if (getHasColor()) {
				getChildSprite()->setColor(color);
			}
		}

		auto getGlowSprite() {
			return from<cocos2d::CCSprite*>(this, 0x1d8);
		}

		void setCustomColor(cocos2d::ccColor3B color) {
			if (getHasColor()) setChildColor(color);
			else setObjectColor(color);
		}
		bool isSelected() {
			return from<bool>(this, 0x316);
		}
		
		auto getActiveColorMode() {
			return from<CustomColorMode>(this, 0x308);
		}

		bool shouldBlendColor() {
			return reinterpret_cast<bool(__thiscall*)(GameObject*)>(base + 0x6ece0)(this);
		}

		auto& touchTriggered() {
			return from<bool>(this, 0x271);
		}
		/*auto triggerColor() {
			return from<cocos2d::ccColor3B&>(this, 0x2b8);
		}

		auto triggerDuration() {
			return from<float>(this, 0x2bc);
		}
		
		auto triggerBlending() {
			return from<bool>(this, 0x314);
		}

		auto touchTriggered() {
			return from<bool>(this, 0x271);
		}

		bool getHasColor() const {
			return from<bool>(this, 0x24a);
		}

		void setObjectColor(cocos2d::ccColor3B& color) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, cocos2d::ccColor3B&)>(base + 0x75560)(this, color);
		}

		void setCustomColor(cocos2d::ccColor3B& color) {
			if (getHasColor()) setChildColor(color);
			else setObjectColor(color);
		}

		auto getChildSprite() {
			return from<CCSprite*>(this, 0x24c);
		}
		void setChildColor(cocos2d::ccColor3B& color) {
			if (getHasColor()) {
				getChildSprite()->setColor(color);
			}
		}

		bool isSelected() {
			return from<bool>(this, 0x316);
		}

		bool getIsTintObject() const {
			return from<bool>(this, 0x2cb);
		}*/

		bool isInvisible() {
			return from<bool>(this, 0x2e8);
		}

		int getActiveColor() {
			return from<int>(this, 0x308);
		}

		virtual cocos2d::CCRect* const& getObjectRect(cocos2d::CCRect* rect) {
			return reinterpret_cast<cocos2d::CCRect* (__thiscall*)(GameObject*, cocos2d::CCRect*)>(base + 0x71b90)(this, rect);
		}

		cocos2d::CCRect* getObjectRect(cocos2d::CCRect* rect, float fl1, float fl2) {
			return reinterpret_cast<cocos2d::CCRect * (__thiscall*)(GameObject*, cocos2d::CCRect*, float, float)>(base + 0x71bd0)(this, rect, fl1, fl2);
		}

		auto hasBeenActivated() {
			return from<bool>(this, 0x246);
		}
		auto hasBeenActivatedP2() {
			return from<bool>(this, 0x247);
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif