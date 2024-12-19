#pragma once

#include <iostream>

#include "../src/vendor/glm/glm.hpp"

#include "Utils/GameObject.h"
#include "Utils/InputManager.h"
#include "Utils/TimeUtils.h"
#include "Utils/ResourcesManager.h"

#include "ProjectileManager.h"
#include "SpriteRenderer.h"

#include <GLFW/glfw3.h>
//include particle emmiter

class PlayerManager
{
public:
	PlayerManager(ProjectileManager& projectileManager);
	~PlayerManager();

	void CreatePlayer(float boundsWidth, float boundsHeight);
	void CreateProjectile();
	void ProcessInput(float deltaTime, const InputManager& input, float boundsWidth);
	void Render(const SpriteRenderer& renderer);
	void Update(float deltaTime);
	void HandlePlayerHit();
	void DestroyPlayer() const;
	void Restart(float boundsWidth, float boundsHeight);

	inline GameObject& GetPlayer() const { return *m_Player; }
	inline int GetPlayerCurrentLives() const { return m_PlayerCurrentLives; }

private:
	bool CanShoot() const;
	glm::vec2 SetStartPosition(float boundsWidth, float boundsHeight);

	const glm::vec2 PLAYER_SIZE = glm::vec2(60.0f, 60.0f);
	const glm::vec2 PLAYER_OFFSET = glm::vec2(20.0f, 20.0f);
	const glm::vec3 PLAYER_COLOR = glm::vec3(0.5f, 1.0f, 0.5f);

	glm::vec2 m_ShotPosition{ 0.f };
	glm::vec2 m_PlayerPreviousPosition{ 0.f };

	const float PLAYER_SPEED = 500.0f;
	const float SHOT_COOLDOWN = 0.5f;
	const int PLAYER_INITIAL_LIVES = 3;

	float m_LastShotTime = 0.0f;
	int m_PlayerCurrentLives = PLAYER_INITIAL_LIVES;

	std::shared_ptr<GameObject> m_Player;
	std::shared_ptr<Texture> m_PlayerSprite;
	std::shared_ptr<Texture> m_ProjectileSprite;
	std::shared_ptr<Shader> m_PlayerShader;

	ProjectileManager& m_ProjectileHandler;
	//particle emitter
};