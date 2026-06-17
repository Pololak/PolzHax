// https://github.com/HJfod/BetterEdit/blob/v4/tools/RotateSaws/rotateSaws.cpp
#include "RotateSaws.h"
#include <array>
#include "utils.h"

std::map<GameObject*, float> g_startRotations;
std::array<int, 100> g_sawIDs = {
	88, 89, 98,
	397, 398, 399,
	675, 676, 677,
	186, 187, 188,
	740, 741, 742,
	678, 679, 680,
	183, 184, 185,
	85, 86, 87, 97,
	137, 138, 139,
	154, 155, 156,
	180, 181, 182,
	222, 223, 224,
	375, 376, 377, 378,
	394, 395, 396,
};

namespace RotateSaws {
	bool objectIsSaw(GameObject* obj) {
		for (auto id : g_sawIDs) {
			if (obj->m_objectID == id) {
				return true;
			}
		}
		return false;
	}

	void beginRotateSaw(GameObject* obj) {
		if (obj->m_myAction)
			return;

		CCAction* r;
		r = obj->createRotateAction(360.f);
		r->setTag(9957);
		g_startRotations[obj] = obj->getRotation();
		obj->m_myAction = r;
		obj->m_myAction->retain();
		obj->runAction(r);
	}

	void beginRotations(LevelEditorLayer* self) {
		CCARRAY_FOREACH_B_TYPE(self->m_levelSections, section, CCArray) {
			if (section) {
				CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (object) {
						if (objectIsSaw(object)) {
							beginRotateSaw(object);
						}
					}
				}
			}
		}
	}

	void stopRotateSaw(GameObject* obj) {
		obj->stopActionByTag(9957);
		if (g_startRotations.count(obj)) {
			obj->setRotation(g_startRotations[obj]);
			g_startRotations.erase(obj);
		}
		if (obj->m_myAction) {
			obj->m_myAction->release();
		}
		obj->m_myAction = nullptr;
	}

	void stopRotations(LevelEditorLayer* self) {
		CCARRAY_FOREACH_B_TYPE(self->m_levelSections, section, CCArray) {
			if (section) {
				CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (object) {
						if (objectIsSaw(object)) {
							stopRotateSaw(object);
						}
					}
				}
			}
		}
		
		g_startRotations.clear();
	}

	void resumeRotations(LevelEditorLayer* self) {
		CCARRAY_FOREACH_B_TYPE(self->m_levelSections, section, CCArray) {
			if (section) {
				CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (object) {
						if (objectIsSaw(object)) {
							if (object->m_myAction) {
								object->runAction(object->m_myAction);
							}
						}
					}
				}
			}
		}
	}

	void pauseRotations(LevelEditorLayer* self) {
		CCARRAY_FOREACH_B_TYPE(self->m_levelSections, section, CCArray) {
			if (section) {
				CCARRAY_FOREACH_B_TYPE(section, object, GameObject) {
					if (object) {
						if (objectIsSaw(object)) {
							object->stopActionByTag(9957);
						}
					}
				}
			}
		}
	}
}