#include "ProjectileManager.h"

void ProjectileManager::AddPlayerProjectile(GameObject&& projectile)
{
	m_PlayerProjectiles.emplace_back(std::move(projectile));
}

std::vector<GameObject>::iterator ProjectileManager::RemovePlayerProjectile(std::vector<GameObject>::iterator interator)
{
	return m_PlayerProjectiles.erase(interator);
}

void ProjectileManager::ClearPlayerProjectiles()
{
	m_PlayerProjectiles.clear();
}

std::vector<GameObject> ProjectileManager::GetPlayerProjectiles()
{
	return m_PlayerProjectiles;
}
