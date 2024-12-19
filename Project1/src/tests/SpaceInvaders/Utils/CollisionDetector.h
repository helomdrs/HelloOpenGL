#pragma once
#include "GameObject.h"

class CollisionDetector
{
public:
	static bool IsColliding(const GameObject& objA, const GameObject& objB)
	{
		const float objAHorizontalBounds = objA.Position.x + objA.Size.x;
		const float objBHorizontalBounds = objB.Position.x + objB.Size.x;
		const bool isCollidingHorizontally = objAHorizontalBounds >= objB.Position.x && objBHorizontalBounds >= objA.Position.x;

		const float objAVerticalBounds = objA.Position.y + objA.Size.y;
		const float objBVerticalBounds = objB.Position.y + objB.Size.y;
		const bool isCollidingVertically = objAVerticalBounds >= objB.Position.y && objBVerticalBounds >= objA.Position.y;

		return isCollidingHorizontally && isCollidingVertically;
	}
};