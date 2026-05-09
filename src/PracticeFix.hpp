#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CheckpointStorage {
	float rot;
	float rot_2;
	double yVelocity;
	float slopeVel;

	static CheckpointStorage from(PlayerObject* player) {
		return (CheckpointStorage({
			*(float*)((size_t)player + 0x18), // rot
			*(float*)((size_t)player + 0x1C), // rot
			player->m_yVelocity,
			player->m_slopeYVel
			}));
	}

	void restore(PlayerObject* player) {
		player->m_yVelocity = yVelocity;
		player->m_slopeYVel = slopeVel;
		*(float*)((size_t)player + 0x18) = rot;
		*(float*)((size_t)player + 0x1c) = rot_2;
	}
};

struct Checkpoint {
	CheckpointStorage p1;
	CheckpointStorage p2;

	static Checkpoint from(PlayLayer* playLayer) {
		PlayerObject* player1 = playLayer->m_player;
		PlayerObject* player2 = playLayer->m_player2;

		return (
			Checkpoint({
				CheckpointStorage::from(player1),
				CheckpointStorage::from(player2)
				})
			);
	}

	void restore(PlayLayer* playLayer) {
		p1.restore(playLayer->m_player);
		p2.restore(playLayer->m_player2);
	}
};