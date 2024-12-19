#pragma once

class InputManager
{
public:
	void SetKeyPressed(int key, bool isPressed);
	bool GetKeyStatus(int key) const;

private:
	bool m_Keys[1024];
};