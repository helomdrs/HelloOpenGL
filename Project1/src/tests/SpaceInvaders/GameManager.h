#pragma once
#include <memory>
#include <vector>
#include <iostream>

#include "SpriteRenderer.h"

#include "tests/SpaceInvaders/Utils/InputManager.h"
#include "tests/SpaceInvaders/Utils/EGameState.h"
#include "tests/SpaceInvaders/Utils/GameObject.h"
#include "tests/SpaceInvaders/Utils/ResourcesManager.h"
#include "tests/SpaceInvaders/Utils/LevelConfigs.h"
#include "tests/SpaceInvaders/Utils/CollisionDetector.h"

#include "tests/SpaceInvaders/PlayerManager.h"
#include "tests/SpaceInvaders/UIManager.h"
#include "tests/SpaceInvaders/ProjectileManager.h"
#include "tests/SpaceInvaders/EnemyManager.h"

class Texture;
class SpriteRenderer;
class Shader;
class UIManager;
class TextRenderer;
class PlayerManager;

//class PostProcessingManager;
//class FrameBufferManager;

class GameManager
{
public: 
	void Init();
	void Update(float deltaTime);
	void UpdatePlayerProjectiles(float deltaTime);
	void ProcessInput(float deltaTime, const InputManager& input);
	void RenderProjectiles() const;
	void Render();

	void HandleGameOver();
	void Close();
	bool IsGameOver();

	void CheckEnemyCollisions(GameObject& projectile) const;

private:
	const float BOUNDS_WIDTH = 800.0f;
	const float BOUNDS_HEIGTH = 600.0f;

	std::unique_ptr<PlayerManager> m_PlayerManager;
	std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
	std::unique_ptr<UIManager> m_UIManager;
	std::unique_ptr<ProjectileManager> m_ProjectileManager;
	std::unique_ptr<EnemyManager> m_EnemyManager;

	//std::unique_ptr<PostProcessingManager> m_PostProcessingManager;
	//std::unique_ptr<FrameBufferManager> m_FrameBuffer;

	EGameState m_CurrentGameState{ EGameState::MainMenu };

	void StartGame();
	void HandleGameWin();
	void Restart();
	void RemoveDestroyedProjectiles();
};