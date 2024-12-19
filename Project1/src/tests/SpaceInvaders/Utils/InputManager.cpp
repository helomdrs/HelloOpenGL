#include "InputManager.h"

void InputManager::SetKeyPressed(const int key, const bool isPressed)
{
	m_Keys[key] = isPressed;
}

bool InputManager::GetKeyStatus(const int key) const
{
	return m_Keys[key];
}