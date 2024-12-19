#pragma once
#include "Utils/GameObject.h"
#include <vector>

class ProjectileManager
{
public:
	ProjectileManager() = default;

	void AddPlayerProjectile(GameObject&& projectile);
	void ClearPlayerProjectiles();

	std::vector<GameObject> GetPlayerProjectiles();
	std::vector<GameObject>::iterator RemovePlayerProjectile(std::vector<GameObject>::iterator interator);

private:
	std::vector<GameObject> m_PlayerProjectiles;
};