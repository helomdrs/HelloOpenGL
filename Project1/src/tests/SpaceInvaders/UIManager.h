#pragma once
#include <memory>

#include "Utils/GameObject.h"
#include "Utils/InputManager.h"
#include "Utils/ResourcesManager.h"
#include "Utils/TimeUtils.h"

#include "TextRenderer.h"
#include "Texture.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class InputManager;
class SpriteRenderer;

class UIManager
{
public:
	UIManager(unsigned int screenWidth, unsigned int screenHeight);

	void ProcessMainMenuInput(const InputManager& input);
	void RenderMainMenuScreen() const;
	void RenderInGameScreen(unsigned int playerLives) const;
	void RenderGameWinScreen() const;
	void RenderGameOverScreen() const;
	void ExitMainMenu();
	void Restart() const;

	inline bool IsPlayRequested() const { return isPlayRequested; }

private:
	std::unique_ptr<TextRenderer> m_TextRenderer;
	std::unique_ptr<TextRenderer> m_MainMenuTextRenderer;
	std::shared_ptr<Shader> m_DefaultTextShader;
	std::shared_ptr<Shader> m_BlinkTextShader;

	/*std::shared_ptr<Texture> m_GameEndedBGSprite;
	GameObject m_GameEndedBG;*/

	unsigned int m_ScreenWidth = 0;
	unsigned int m_ScreenHeight = 0;
	bool isPlayRequested = false;

	void RenderRestartMessage() const;
};