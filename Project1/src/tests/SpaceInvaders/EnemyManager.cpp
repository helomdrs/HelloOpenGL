#include "EnemyManager.h"

EnemyManager::EnemyManager(unsigned int boundsWidth, unsigned int boundsHeight, const LevelConfigs& levelConfig)
	:m_LevelWidth(boundsWidth), m_LevelHeight(boundsHeight), m_LevelConfig(levelConfig)
{
	SpawnEnemies(levelConfig);

	//set projectile sprite and particle sprite

	m_TotalEnemies = m_Enemies.size();
	
	//make emitters
}

void EnemyManager::Update(float deltaTime)
{
	//update emitters

	MoveEnemies(deltaTime);

	//if can shoot shoot
}

void EnemyManager::Render(const SpriteRenderer& renderer)
{
	for (const GameObject& enemy : m_Enemies)
	{
		//render particle emitters

		if (enemy.Destroyed) continue;
		enemy.Draw(renderer);
	}
}

void EnemyManager::OnEnemyHit(GameObject& enemy)
{
	m_TotalEnemiesKilled++;
	enemy.Destroyed = true;

	//play audio
	//emit particles
	//increase dificulty
}

void EnemyManager::StopAttack()
{
	isPassiveBehaviorEnabled = true;
}

void EnemyManager::Restart()
{
	m_TotalEnemiesKilled = 0;
	m_CurrentMovementDirection = MOVEMENT_DIRECTION;
	isPassiveBehaviorEnabled = false;
	isEnemyAtBottom = false;

	glm::vec2 startPosition = SetEnemyStartPosition(m_LevelConfig);

	int i = 0;
	for (unsigned int y = 0; y < m_LevelConfig.EnemyRows; y++)
	{
		for (unsigned int x = 0; x < m_LevelConfig.EnemyColumns; x++)
		{
			glm::vec2 position = startPosition;
			position.x += (m_LevelConfig.Padding + m_EnemySize.x) * x;
			position.y += (m_LevelConfig.Padding + m_EnemySize.y) * y;

			m_Enemies[i].Position = position;
			m_Enemies[i].Destroyed = false;

			i++;
		}
	}
}

bool EnemyManager::IsAllEnemiesDead() const
{
	return m_TotalEnemiesKilled >= m_TotalEnemies;
}

bool EnemyManager::IsEnemyAtBottom() const
{
	return isEnemyAtBottom;
}

glm::vec2 EnemyManager::SetEnemySize(const LevelConfigs& configs) const
{
	const float maxSize = 40.f;

	float desiredWidth = (m_LevelWidth - configs.Padding * configs.EnemyColumns - configs.HorizontalMargin * 2) / configs.EnemyColumns;
	float desiredHeight = (m_LevelWidth - configs.Padding * configs.EnemyRows - configs.TopMargin - configs.MinBottomMargin) / configs.EnemyRows;
	float smallestSide = std::min(desiredHeight, desiredWidth);

	if (smallestSide > maxSize) { smallestSide = maxSize; }

	desiredWidth = smallestSide;
	desiredHeight = smallestSide;

	return glm::vec2{ desiredWidth, desiredHeight };
}

glm::vec2 EnemyManager::SetEnemyStartPosition(const LevelConfigs& configs)
{
	float rowWidth = m_EnemySize.x * configs.EnemyColumns + configs.Padding * configs.EnemyColumns;
	glm::vec2 startPosition = { m_LevelWidth / 2.f - rowWidth / 2.f, configs.TopMargin };

	return startPosition;
}

void EnemyManager::MoveEnemies(float deltaTime)
{
	bool hasHitWall = false;

	for (GameObject& enemy : m_Enemies)
	{
		if (enemy.Destroyed) { continue; }

		enemy.Position += m_CurrentMovementVelocity * m_CurrentMovementDirection * deltaTime;

		bool isCrossingLeftBound = m_CurrentMovementDirection.x < 0.f && enemy.Position.x <= 0.f;
		bool isCrossingRightBound = m_CurrentMovementDirection.x > 0.f && enemy.Position.x + m_EnemySize.x >= m_LevelWidth;

		if (isCrossingRightBound || isCrossingLeftBound) { hasHitWall = true; }
	}

	if (!hasHitWall) { return; }

	m_CurrentMovementDirection.x = -m_CurrentMovementDirection.x;

	if (!isPassiveBehaviorEnabled) { MoveEnemiesDownwards(); }
}

void EnemyManager::MoveEnemiesDownwards()
{
	for (GameObject& enemy : m_Enemies)
	{
		if (enemy.Destroyed) { continue; }

		enemy.Position.y += m_EnemySize.y / 2.f;

		if (enemy.Position.y >= m_LevelConfig.GameOverBottomThreshold)
		{
			isEnemyAtBottom = true;
		}
	}
}

void EnemyManager::SpawnEnemies(const LevelConfigs& configs)
{
	std::shared_ptr<Texture> enemySprite = ResourcesManager::LoadTexture("res/textures/SpaceInvaders/enemy.png", "Enemy", true);
	m_EnemySize = SetEnemySize(configs);
	glm::vec2 startPosition = SetEnemyStartPosition(configs);

	//color mapping

	for (unsigned int i = 0; i < configs.EnemyRows; i++)
	{
		for (unsigned int j = 0; j < configs.EnemyColumns; j++)
		{
			glm::vec2 position = startPosition;
			position.x += (configs.Padding + m_EnemySize.x) * i;
			position.y += (configs.Padding + m_EnemySize.y) * j;

			m_Enemies.emplace_back(position, m_EnemySize, enemySprite);
		}
	}
}
