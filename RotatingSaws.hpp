#pragma once
#include "pch.h"

namespace RotateSaws {
	bool objectIsSaw(gd::GameObject* obj);
	void beginRotations(gd::LevelEditorLayer* self);
	void stopRotations(gd::LevelEditorLayer* self);
	void pauseRotations(gd::LevelEditorLayer* self);
	void resumeRotations(gd::LevelEditorLayer* self);
	void beginRotateSaw(gd::GameObject* obj);
	void stopRotateSaw(gd::GameObject* obj);
	bool shouldRotateSaw();
	void enableRotations(bool);
}