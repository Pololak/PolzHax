#pragma once
#include "pch.h"

class ShowTrajectory {
public:
	static ShowTrajectory& get() {
		static ShowTrajectory inst;
		return inst;
	}

	gd::PlayerObject* m_player;
	gd::PlayerObject* m_player2;
	bool m_isSimulation = false;
	bool m_isSimulationDead = true;
	bool m_p1Down = false;
	bool m_p2Down = false;
	float m_frameDt = 0.f;

	void createPlayers();
	void iterateForPlayer(gd::PlayerObject* player, bool holding, bool isPlayerTwo);
	void simulationForPlayer(gd::PlayerObject* player, gd::PlayerObject* player2, gd::PlayerObject* playerBase);
	void resetCollisionLog(gd::PlayerObject*);
	void drawRectangleHitbox(CCDrawNode*, CCRect const&, ccColor4B, ccColor4B);
	void drawForPlayer(gd::PlayerObject*);
};