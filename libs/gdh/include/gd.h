#ifndef __GD_H__
#define __GD_H__

#include <cocos2d.h>
#include <cocos-ext.h>

#include "gdMacros.h"

template <class R, class T>
R& from(T base, intptr_t offset) {
	return *reinterpret_cast<R*>(reinterpret_cast<uintptr_t>(base) + offset);
}

template<typename T>
T clamp(T value, T lower, T upper) {
    if (value < lower) return lower;
    if (value > upper) return upper;
    return value;
}

#include "GDProtocols.h"
#include "enums.h"
#include "../delegates/delegates.h"

#include "../actions/CCCircleWave.h"

#include "../menu_nodes/CCMenuItemSpriteExtra.h"
#include "../menu_nodes/CCMenuItemToggler.h"
#include "../menu_nodes/Slider.h"

#include "../manager_nodes/GManager.h"
#include "../manager_nodes/GameManager.h"
#include "../manager_nodes/LocalLevelManager.h"
#include "../manager_nodes/GameLevelManager.h"

#include "../scroll_nodes/CCContentLayer.h"
#include "../scroll_nodes/CCIndexPath.h"
#include "../scroll_nodes/CCScrollLayerExt.h"
#include "../scroll_nodes/TableView.h"
#include "../scroll_nodes/BoomListView.h"
#include "../scroll_nodes/CustomListView.h"

#include "../sprite_nodes/CCSpritePlus.h"
#include "../sprite_nodes/GameObject.h"
#include "../sprite_nodes/HardStreak.h"
#include "../sprite_nodes/PlayerObject.h"
#include "../sprite_nodes/ButtonSprite.h"
#include "../sprite_nodes/TextArea.h"

#include "../other_nodes/StartPosObject.h"
#include "../other_nodes/UndoObject.h"
#include "../other_nodes/OBB2D.h"
#include "../other_nodes/SongInfoObject.h"
#include "../other_nodes/CheckpointObject.h"
#include "../other_nodes/ColorAction.h"
#include "../other_nodes/GhostTrailEffect.h"
#include "../other_nodes/SettingsColorObject.h"
#include "../other_nodes/GJSearchObject.h"

#include "../layers_scenes_transitions_nodes/AudioEffectsLayer.h"
#include "../layers_scenes_transitions_nodes/BoomScrollLayer.h"
#include "../layers_scenes_transitions_nodes/LoadingCircle.h"
#include "../layers_scenes_transitions_nodes/GJListLayer.h"
#include "../layers_scenes_transitions_nodes/GJGroundLayer.h"
#include "../layers_scenes_transitions_nodes/DrawGridLayer.h"
#include "../layers_scenes_transitions_nodes/LevelEditorLayer.h"
#include "../layers_scenes_transitions_nodes/FLAlertLayer.h"
#include "../layers_scenes_transitions_nodes/EditorUI.h"
#include "../layers_scenes_transitions_nodes/ColorSelectPopup.h"
#include "../layers_scenes_transitions_nodes/ScrollingLayer.h"
#include "../layers_scenes_transitions_nodes/MenuLayer.h"
#include "../layers_scenes_transitions_nodes/CCBlockLayer.h"
#include "../layers_scenes_transitions_nodes/EditorPauseLayer.h"
#include "../layers_scenes_transitions_nodes/PauseLayer.h"
#include "../layers_scenes_transitions_nodes/UILayer.h"
#include "../layers_scenes_transitions_nodes/PlayLayer.h"
#include "../layers_scenes_transitions_nodes/LoadingLayer.h"
#include "../layers_scenes_transitions_nodes/EditLevelLayer.h"
#include "../layers_scenes_transitions_nodes/CustomizeObjectLayer.h"
#include "../layers_scenes_transitions_nodes/SetGroupIDLayer.h"
#include "../layers_scenes_transitions_nodes/LevelSelectLayer.h"
#include "../layers_scenes_transitions_nodes/LevelInfoLayer.h"
#include "../layers_scenes_transitions_nodes/CustomSongWidget.h"
#include "../layers_scenes_transitions_nodes/SelectArtLayer.h"
#include "../layers_scenes_transitions_nodes/LevelSettingsLayer.h"
#include "../layers_scenes_transitions_nodes/LevelBrowserLayer.h"

#include "../audio_nodes/FMODAudioEngine.h"

#include "../level_nodes/GJGameLevel.h"
#include "../level_nodes/LevelSettingsObject.h"
#include "../level_nodes/EndPortalObject.h"

#include "../text_input_nodes/CCTextInputNode.h"

#include "GameToolbox.h"

#endif