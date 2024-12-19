#include "GameManager.h"

#include <GLFW/glfw3.h>
#include "../src/vendor/glm/ext/matrix_clip_space.hpp"

void GameManager::Init()
{
	LevelConfigs levelConfig{};
	levelConfig.EnemyColumns = 11;
	levelConfig.EnemyRows = 5;
	levelConfig.Padding = 15.f;
	levelConfig.HorizontalMargin = 80.f;
	levelConfig.TopMargin = 50.f;
	levelConfig.MinBottomMargin = 150.f;
	levelConfig.GameOverBottomThreshold = BOUNDS_HEIGTH - 120.f;

	glm::mat4 ortho = glm::ortho(0.f, BOUNDS_WIDTH, BOUNDS_HEIGTH, 0.f, -1.f, 1.f);

	std::shared_ptr<Shader> spriteShader = ResourcesManager::LoadShader("res/shaders/SpaceInvaders/Sprite.shader", "Sprite");

	spriteShader->Bind();
	spriteShader->SetUniformMat4f("u_Projection", ortho);
	spriteShader->SetUniform1i("u_Image", 0);

	std::shared_ptr<Shader> dynamicSpriteShader = ResourcesManager::LoadShader("res/shaders/SpaceInvaders/SpriteDynamic.shader","Dynamic");

	dynamicSpriteShader->Bind();
	dynamicSpriteShader->SetUniformMat4f("u_Projection", ortho);
	dynamicSpriteShader->SetUniform1i("u_Image", 0);

	m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

	m_ProjectileManager = std::make_unique<ProjectileManager>();
	m_PlayerManager = std::make_unique<PlayerManager>(*m_ProjectileManager);
	m_PlayerManager->CreatePlayer(BOUNDS_WIDTH, BOUNDS_HEIGTH);
	m_EnemyManager = std::make_unique<EnemyManager>(BOUNDS_WIDTH, BOUNDS_HEIGTH, levelConfig);
	m_UIManager = std::make_unique<UIManager>(BOUNDS_WIDTH, BOUNDS_HEIGTH);

	//m_BackgroundManager = std::make_unique<BackgroundManager>(BOUNDS_WIDTH, BOUNDS_HEIGTH);
	//m_PostProcessingManager = std::make_unique<PostProcessingManager>();

	//frame buffer here

	//audio here
}

void GameManager::Update(float deltaTime)
{
	if (m_CurrentGameState == EGameState::Playing)
	{
		if (IsGameOver()) return;

		UpdatePlayerProjectiles(deltaTime);
		RemoveDestroyedProjectiles();

		m_PlayerManager->Update(deltaTime);
		m_EnemyManager->Update(deltaTime);
	}
	else if (m_CurrentGameState == EGameState::GameWin)
	{
		UpdatePlayerProjectiles(deltaTime);
		RemoveDestroyedProjectiles();
	}
	else if (m_CurrentGameState == EGameState::GameOver)
	{
		RemoveDestroyedProjectiles();
		m_EnemyManager->Update(deltaTime);
	}
	else if (m_CurrentGameState == EGameState::MainMenu)
	{
		if (m_UIManager->IsPlayRequested())
		{
			StartGame();
		}
	}
}

void GameManager::ProcessInput(float deltaTime, const InputManager& input)
{
	if (m_CurrentGameState == EGameState::GameWin || m_CurrentGameState == EGameState::GameOver)
	{
		if (input.GetKeyStatus(GLFW_KEY_R))
		{
			Restart();
		}
	}

	if (m_CurrentGameState == EGameState::Playing || m_CurrentGameState == EGameState::GameWin)
	{
		m_PlayerManager->ProcessInput(deltaTime, input, BOUNDS_WIDTH);
	}
	else if (m_CurrentGameState == EGameState::MainMenu)
	{
		m_UIManager->ProcessMainMenuInput(input);
	}
}

void GameManager::RenderProjectiles() const
{
	for (auto& projectile : m_ProjectileManager->GetPlayerProjectiles())
	{
		projectile.Draw(*m_SpriteRenderer);
	}
}

void GameManager::Render()
{
	//bind and clear frame buffer

	if (m_CurrentGameState == EGameState::MainMenu)
	{
		//unbind frame buffer
		//render post processing

		m_UIManager->RenderMainMenuScreen();

		return;
	}

	m_EnemyManager->Render(*m_SpriteRenderer);
	m_PlayerManager->Render(*m_SpriteRenderer);

	RenderProjectiles();

	//unbind frame buffer
	//render post processing

	if (m_CurrentGameState == EGameState::Playing)
	{
		m_UIManager->RenderInGameScreen(m_PlayerManager->GetPlayerCurrentLives());
	}
	else if (m_CurrentGameState == EGameState::GameOver)
	{
		m_UIManager->RenderGameOverScreen();
	}
}

void GameManager::UpdatePlayerProjectiles(float deltaTime)
{
	for (auto& projectile : m_ProjectileManager->GetPlayerProjectiles())
	{
		projectile.Position.y -= projectile.Velocity.y * deltaTime;

		if (projectile.Position.y <= 0)
		{
			projectile.Destroyed = true;
			continue;
		}

		CheckEnemyCollisions(projectile);
	}
}

void GameManager::CheckEnemyCollisions(GameObject& projectile) const
{
	for (auto& enemy : m_EnemyManager->GetEnemies())
	{
		if (enemy.Destroyed) { continue; }
		if (!CollisionDetector::IsColliding(projectile, enemy)) { continue; }

		projectile.Destroyed = true;
		m_EnemyManager->OnEnemyHit(enemy);

		break;
	}
}

void GameManager::RemoveDestroyedProjectiles()
{
	auto playerInterator = m_ProjectileManager->GetPlayerProjectiles().begin();
	while (playerInterator != m_ProjectileManager->GetPlayerProjectiles().end())
	{
		if (playerInterator->Destroyed)
		{
			playerInterator = m_ProjectileManager->RemovePlayerProjectile(playerInterator);
		}
		else
		{
			playerInterator++;
		}
	}
}

bool GameManager::IsGameOver()
{
	if (m_EnemyManager->IsAllEnemiesDead())
	{
		HandleGameWin();
		return true;
	}

	if (m_PlayerManager->GetPlayer().Destroyed || m_EnemyManager->IsEnemyAtBottom())
	{
		HandleGameOver();
		return true;
	}

	return false;
}

void GameManager::StartGame()
{
	m_CurrentGameState = EGameState::Playing;
	m_UIManager->ExitMainMenu();
}

void GameManager::HandleGameWin()
{
	m_CurrentGameState = EGameState::GameWin;
	
	//audio play level complete
}

void GameManager::HandleGameOver()
{
	m_CurrentGameState = EGameState::GameOver;
	m_ProjectileManager->ClearPlayerProjectiles();
	m_PlayerManager->DestroyPlayer();
	m_EnemyManager->StopAttack();

	//audio play game over
}

void GameManager::Close()
{
	ResourcesManager::ClearAll();
}

void GameManager::Restart()
{
	m_CurrentGameState = EGameState::Playing;
	m_ProjectileManager->ClearPlayerProjectiles();
	m_PlayerManager->Restart(BOUNDS_WIDTH, BOUNDS_HEIGTH);
	m_EnemyManager->Restart();
	m_UIManager->Restart();
}


