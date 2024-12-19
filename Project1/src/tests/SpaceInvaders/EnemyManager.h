#pragma once

#include <map>
#include <vector>

//include particle emitter
#include "Utils/GameObject.h"
#include "Utils/LevelConfigs.h"
#include "Utils/ResourcesManager.h"

class SpriteRenderer;

class EnemyManager
{
public:
	EnemyManager(unsigned int boundsWidth, unsigned int boundsHeight, const LevelConfigs& levelConfig);

	void Update(float deltaTime);
	void Render(const SpriteRenderer& renderer);
	void OnEnemyHit(GameObject& enemy);
	void StopAttack();
	void Restart();

	bool IsAllEnemiesDead() const;
	bool IsEnemyAtBottom() const;

	inline std::vector<GameObject>& GetEnemies() { return m_Enemies; }

private:

	//multipliers of difficulty level
	//total of particle emmiters

	const glm::vec2 MOVEMENT_VELOCITY{ 20.f, 20.f };
	const glm::vec2 MOVEMENT_DIRECTION{ 1.f, 0.f };

	glm::vec2 m_EnemySize{ 40.f, 40.f };
	glm::vec2 m_CurrentMovementDirection = MOVEMENT_DIRECTION;
	const glm::vec2 m_CurrentMovementVelocity = MOVEMENT_VELOCITY;

	std::vector<GameObject> m_Enemies{};

	LevelConfigs m_LevelConfig;

	unsigned int m_LevelWidth{ 0 };
	unsigned int m_LevelHeight{ 0 };
	//unsigned int current difficulty index
	int m_TotalEnemiesKilled{ 0 };
	int m_TotalEnemies{ 0 };
	bool isPassiveBehaviorEnabled{ false };
	bool isEnemyAtBottom{ false };

	glm::vec2 SetEnemySize(const LevelConfigs& configs) const;
	glm::vec2 SetEnemyStartPosition(const LevelConfigs& configs);

	void MoveEnemies(float deltaTime);
	void MoveEnemiesDownwards();
	void SpawnEnemies(const LevelConfigs& configs);
	//void increase difficulty

	//particle emmiter
};