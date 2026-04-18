#pragma once
#include "pch.h"

namespace RotateSaws {
	bool objectIsSaw(gd::GameObject*);
	void beginRotations(gd::LevelEditorLayer*);
	void stopRotations(gd::LevelEditorLayer*);
	void pauseRotations(gd::LevelEditorLayer*);
	void resumeRotations(gd::LevelEditorLayer*);
	void beginRotateSaw(gd::GameObject*);
	void stopRotateSaw(gd::GameObject*);
}