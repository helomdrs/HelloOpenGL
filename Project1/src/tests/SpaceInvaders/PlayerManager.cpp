#include "PlayerManager.h"

#include "../src/vendor/glm/ext/matrix_clip_space.hpp"
#include "../src/vendor/glm/ext/matrix_transform.hpp"

PlayerManager::PlayerManager(ProjectileManager& projectileManager)
	:m_ProjectileHandler(projectileManager)
{
	m_ProjectileSprite = ResourcesManager::LoadTexture("res/textures/SpaceInvaders/projectile.png", "Projectile");

	m_PlayerShader = ResourcesManager::GetShader("Dynamic");
}

PlayerManager::~PlayerManager(){}

void PlayerManager::CreatePlayer(float boundsWidth, float boundsHeight)
{
	m_PlayerSprite = ResourcesManager::LoadTexture("res/textures/SpaceInvaders/ship.png", "Player");

	const glm::vec2 playerInitialPosition = SetStartPosition(boundsWidth, boundsHeight);

	m_Player = std::make_shared<GameObject>(playerInitialPosition, PLAYER_SIZE, m_PlayerSprite, PLAYER_COLOR, glm::vec2(PLAYER_SPEED));
	m_PlayerPreviousPosition = playerInitialPosition;

	const std::shared_ptr<Texture> particleSprite = ResourcesManager::GetTexture("PlayerParticle");

	//particle emmitter
}

void PlayerManager::ProcessInput(float deltaTime, const InputManager& input, float boundsWidth)
{
	const float velocity = PLAYER_SPEED * deltaTime;

	if (input.GetKeyStatus(GLFW_KEY_A)) //arrumar esse problema do include
	{
		if (m_Player->Position.x >= 0 + PLAYER_OFFSET.x)
		{
			m_Player->Position.x -= velocity;
		}
	}

	else if (input.GetKeyStatus(GLFW_KEY_D))
	{
		if (m_Player->Position.x <= boundsWidth - (m_Player->Size.x + PLAYER_OFFSET.x))
		{
			m_Player->Position.x += velocity;
		}
	}

	if (input.GetKeyStatus(GLFW_KEY_SPACE))
	{
		if (CanShoot())
		{
			CreateProjectile();
			m_LastShotTime = TimeUtils::Time;
		}
	}
}

void PlayerManager::CreateProjectile()
{
	constexpr glm::vec2 projectileSize = glm::vec2(20.0f, 20.0f);
	constexpr glm::vec3 projectileColor = glm::vec3(0.5f, 1.0f, 1.0f);
	constexpr float projectileSpeed = 800.0f;
	//shot sound path

	m_ShotPosition = glm::vec2(
		m_Player->Position.x + (m_Player->Size.x / 2 - 10.0f),
		m_Player->Position.y - 30
	);

	GameObject projectile{
		m_ShotPosition,
		projectileSize,
		m_ProjectileSprite,
		projectileColor,
		glm::vec2(projectileSpeed)
	};

	//play 2d sound

	m_ProjectileHandler.AddPlayerProjectile(std::move(projectile));
}

void PlayerManager::Render(const SpriteRenderer& renderer)
{
	//particle emmiter render

	if (m_Player->Destroyed) { return; }

	m_PlayerShader->Bind();
	glm::mat4 previousModel = glm::mat4{ 1.f };
	previousModel = glm::translate(previousModel, glm::vec3{ m_PlayerPreviousPosition, 0.f });
	previousModel = glm::scale(previousModel, glm::vec3(m_Player->Size, 1.f));
	m_PlayerShader->SetUniformMat4f("u_PreviousModel", previousModel);
	m_PlayerPreviousPosition = m_Player->Position;

	renderer.Draw(*m_PlayerShader, *m_PlayerSprite, m_Player->Position, m_Player->Size, m_Player->Rotation, m_Player->Color);
}

void PlayerManager::Update(float deltaTime)
{
	//update particle emitter
}

void PlayerManager::HandlePlayerHit()
{
	m_PlayerCurrentLives -= 1;
	//emit particles

	if (m_PlayerCurrentLives <= 0)
	{
		//play explosion sound
		m_Player->Destroyed = true;
	}
}

void PlayerManager::DestroyPlayer() const
{
	if (m_Player->Destroyed) { return; }
	m_Player->Destroyed = true;
}

void PlayerManager::Restart(float boundsWidth, float boundsHeight)
{
	m_Player->Destroyed = false;
	m_Player->Position = SetStartPosition(boundsWidth, boundsHeight);
	m_PlayerPreviousPosition = m_Player->Position;
	m_PlayerCurrentLives = PLAYER_INITIAL_LIVES;
}

bool PlayerManager::CanShoot() const
{
	return TimeUtils::Time - m_LastShotTime >= SHOT_COOLDOWN;
}

glm::vec2 PlayerManager::SetStartPosition(float boundsWidth, float boundsHeight)
{
	const glm::vec2 playerInitialPosition = glm::vec2(
		(boundsWidth / 2.0f) - (PLAYER_SIZE.x / 2.0f),
		boundsHeight - PLAYER_SIZE.y - PLAYER_OFFSET.y
	);

	return playerInitialPosition;
}
