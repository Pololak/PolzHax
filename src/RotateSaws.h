#pragma once
#include "include.h"

namespace RotateSaws {
	bool objectIsSaw(GameObject*);
	void beginRotations(LevelEditorLayer*);
	void stopRotations(LevelEditorLayer*);
	void pauseRotations(LevelEditorLayer*);
	void resumeRotations(LevelEditorLayer*);
	void beginRotateSaw(GameObject*);
	void stopRotateSaw(GameObject*);
}