#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <gd.h>
#include "cocos2d.h"

namespace gd {

	class CCSpritePlus;

	enum GameObjectType {
		kGameObjectTypeSolid = 0,
		kGameObjectTypeHazard = 2,
		kGameObjectTypeInverseGravityPortal = 3,
		kGameObjectTypeNormalGravityPortal = 4,
		kGameObjectTypeShipPortal = 5,
		kGameObjectTypeCubePortal = 6,
		kGameObjectTypeDecoration = 7,
		kGameObjectTypePulseObjects = 8, // Used for old pulse objects (wtf)
		kGameObjectTypeYellowJumpPad = 9,
		kGameObjectTypePinkJumpPad = 10,
		kGameObjectTypeGravityPad = 11,
		kGameObjectTypeYellowJumpRing = 12,
		kGameObjectTypePinkJumpRing = 13,
		kGameObjectTypeGravityRing = 14,
		kGameObjectTypeInverseMirrorPortal = 15,
		kGameObjectTypeNormalMirrorPortal = 16,
		kGameObjectTypeBallPortal = 17,
		kGameObjectTypeRegularSizePortal = 18,
		kGameObjectTypeMiniSizePortal = 19,
		kGameObjectTypeUfoPortal = 20,
		kGameObjectTypeModifier = 21,
		kGameObjectTypeBreakable = 22,
		kGameObjectTypeSecretCoin = 23,
		kGameObjectTypeDualPortal = 24,
		kGameObjectTypeSoloPortal = 25,
		kGameObjectTypeSlope = 26,
		kGameObjectTypeWavePortal = 27,
	};

	/*class ColorAction : public CCNode {
	public:
		ccColor3B m_color;
		bool m_blending;
		int m_custom;
	};*/

	/*enum GJGJCustomColorMode {
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
	class GJCustomColorModeSprite;
	class GJEffectManager;

	class GameObject : public CCSpritePlus {
	public:
		bool m_isFlippedX; // 0x1c4
		bool m_isFlippedY; // 0x1c5
		PAD(2)
		cocos2d::CCPoint m_boxOffset; // 0x1c8
		OBB2D* m_objectOBB2D; // 0x1d0
		bool m_isOriented; // 0x1d4
		PAD(3)
		cocos2d::CCSprite* m_glowSprite; // 0x1d8
		bool m_interactable; // 0x1dc
		bool m_isRingObject; // 0x1dd
		PAD(2)
		cocos2d::CCAction* m_myAction; // 0x1e0
		bool m_isRotatingObject; // 0x1e4
		bool m_objectPoweredOn; // 0x1e5
		bool m_hideObject; // 0x1e6
		PAD(1)
		float m_orientedBoxWidth; // 0x1e8
		float m_orientedBoxHeight; // 0x1ec
		bool m_isActive; // 0x1f0
		bool m_hasGlow; // 0x1f1
		bool m_isDestroyed; // 0x1f2
		PAD(1)
		cocos2d::CCParticleSystemQuad* m_particleSystem; // 0x1f4
		gd::string m_particleString; // 0x1f8
		bool m_particleAdded; // 0x210
		PAD(3)
		cocos2d::CCPoint m_portalPosition; // 0x214
		cocos2d::CCRect m_objectTextureRect; // 0x21c
		bool m_textureRectDirty; // 0x22c
		PAD(3)
		float m_fadeInPosOffset; // 0x230
		cocos2d::CCRect m_objectRect2; // 0x234
		bool m_isObjectRectDirty; // 0x244
		bool m_isOrientedObjectRectDirty; // 0x245
		bool m_hasBeenActivatedP1; // 0x246
		bool m_hasBeenActivatedP2; // 0x247
		bool m_canChangeCustomColor; // 0x248
		bool m_hasCustomChildren; // 0x249
		bool m_hasColor; // 0x24a
		PAD(1)
		cocos2d::CCSprite* m_colorSprite; // 0x24c
		bool m_ignoreScreenCheck; // 0x250
		PAD(3)
		float m_objectRadius; // 0x254
		bool m_isRotated; // 0x258
		PAD(3)
		float m_scaleModX; // 0x25c
		float m_scaleModY; // 0x260
		int m_ID; // 0x264
		GameObjectType m_objectType; // 0x268
		int m_sectionIdx; // 0x26c
		bool m_shouldSpawn; // 0x270
		bool m_touchTriggered; // 0x271
		PAD(2)
		cocos2d::CCPoint m_startPosition; // 0x274
		std::string m_textureName; // 0x27c // m_frame
		bool m_isDisabled; // 0x294
		bool m_useAudioScale; // 0x295
		bool m_isSleeping; // 0x296
		PAD(1)
		float m_startRotation; // 0x298
		float m_startScaleX; // 0x29c
		float m_startScaleY; // 0x2a0
		bool m_startFlipX; // 0x2a4
		bool m_startFlipY; // 0x2a5
		bool m_shouldHide; // 0x2a6
		PAD(1)
		float m_spawnXPos; // 0x2a8
		bool m_isInvisible; // 0x2ac
		PAD(3)
		float m_enterAngle; // 0x2b0
		int m_activeEnterEffect; // 0x2b4
		cocos2d::ccColor3B m_triggerColor; // 0x2b8
		float m_triggerDuration; // 0x2bc
		bool m_triggerTintGround; // 0x2c0
		PAD(3)
		int m_objectID; // 0x2c4
		bool m_dontEnter; // 0x2c8
		bool m_dontFade; // 0x2c9
		bool m_dontFadeTinted; // 0x2ca
		bool m_isTintObject; // 0x2cb
		bool m_stateVar; // 0x2cc
		PAD(3)
		int m_objectZ; // 0x2d0 // Z Order
		cocos2d::CCNode* m_objectParent; // 0x2d4
		bool m_customAudioScale; // 0x2d8
		PAD(3)
		float m_minAudioScale; // 0x2dc
		float m_maxAudioScale; // 0x2e0
		int m_secretCoinID; // 0x2e4 // m_uniqueID
		bool m_invisibleMode; // 0x2e8
		bool m_glowUseBGColor; // 0x2e9
		bool m_useSpecialLight; // 0x2ea
		bool m_glowUseReverseColor; // 0x2eb
		bool m_isColorObject; // 0x2ec
		PAD(3)
		float m_opacityMod; // 0x2f0 // object opacity
		float m_glowOpacityMod; // 0x2f4 // object glow opacity
		bool m_dontShow; // 0x2f8
		bool m_upSlope;  // 0x2f9
		PAD(2)
		int m_slopeType; // 0x2fc
		float m_slopeAngle; // 0x300
		bool m_hazardousSlope; // 0x304
		PAD(3)
		CustomColorMode m_customColorMode; // 0x308 // object color
		CustomColorMode m_defaultColorMode; // 0x30c
		bool m_forceBottomLayer; // 0x310
		bool m_shouldPreview; // 0x311 // m_editorSelected (why it named like this in .so)
		bool m_copyPlayerColor1; // 0x312
		bool m_copyPlayerColor2; // 0x313
		bool m_triggerBlending; // 0x314
		bool m_wasSelected; // 0x315
		bool m_isSelected; // 0x316 // in editor
		PAD(1)
		int m_editorSelectIdx; // 0x318
		cocos2d::CCPoint m_storedPosition; // 0x31c
		int m_editorGroup; // 0x324

		//CCRGBAProtocol vtable
		virtual void setOpacity(GLubyte opacity) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, GLubyte)>(
				base + 0x72a60
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

		bool canRotateFree() {
			return (
				m_objectType != GameObjectType::kGameObjectTypeSolid &&
				m_objectType != GameObjectType::kGameObjectTypeBreakable &&
				m_objectType != GameObjectType::kGameObjectTypeSlope
				);
		}
	};
}

#endif