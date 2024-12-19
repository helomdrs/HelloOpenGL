#include "UIManager.h"

#include <GLFW/glfw3.h>

UIManager::UIManager(unsigned int screenWidth, unsigned int screenHeight)
	:m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
	m_DefaultTextShader = ResourcesManager::LoadShader("res/shaders/SpaceInvaders/Text.shader", "DefaultText");
	m_BlinkTextShader = ResourcesManager::LoadShader("res/shaders/SpaceInvaders/TextBlink.shader", "BlinkText");
	
	//load background texture

	m_MainMenuTextRenderer = std::make_unique<TextRenderer>(m_ScreenWidth, m_ScreenHeight, "res/fonts/Space Night.ttf", 50, m_DefaultTextShader);
	m_TextRenderer = std::make_unique<TextRenderer>(m_ScreenWidth, m_ScreenHeight, "res/fonts/Space Night.ttf", 30, m_DefaultTextShader);
	m_TextRenderer->SetupShader(m_BlinkTextShader);

	m_BlinkTextShader->Bind();
	m_BlinkTextShader->SetUniform1f("u_Speed", 1.5f);

	//calculate size, position and color
	//create background game object
}

void UIManager::ProcessMainMenuInput(const InputManager& input)
{
	if (input.GetKeyStatus(GLFW_KEY_ENTER)) { isPlayRequested = true; }
}

void UIManager::RenderMainMenuScreen() const
{
	m_MainMenuTextRenderer->RenderText("SPACE", 275, 170, 1.f);
	m_MainMenuTextRenderer->RenderText("INVADERS", 200, 230, 1.f);

	m_BlinkTextShader->Bind();
	m_BlinkTextShader->SetUniform1f("u_Time", TimeUtils::Time);
	m_TextRenderer->ChangeShader(m_BlinkTextShader);
	m_TextRenderer->RenderText("Press ENTER", 330, 400, 0.5f);

	m_TextRenderer->ChangeShader(m_DefaultTextShader);
	m_TextRenderer->RenderText("Controls", 20, 520, 0.4f);
	m_TextRenderer->RenderText("A and D to move", 20, 540, 0.4f);
	m_TextRenderer->RenderText("SPACE to shoot", 20, 560, 0.4f);
}

void UIManager::RenderInGameScreen(unsigned int playerLives) const
{
	m_TextRenderer->RenderText("LIVES: " + std::to_string(playerLives), 20.f, 20.f, 1.f);
}

void UIManager::RenderGameWinScreen() const
{
	m_TextRenderer->ChangeShader(m_DefaultTextShader);
	m_TextRenderer->RenderText("GOOD ENDING", 270, 200, 1.f);
	m_TextRenderer->RenderText("For you, can't say the same for the aliens...", 210, 250, 0.4f);

	RenderRestartMessage();
}

void UIManager::RenderGameOverScreen() const
{
	m_TextRenderer->ChangeShader(m_DefaultTextShader);
	m_TextRenderer->RenderText("BAD ENDING", 270, 200, 1.f);
	m_TextRenderer->RenderText("There's nothing else you could do. The aliens are superior!", 150, 250, 0.4f);

	RenderRestartMessage();
}

void UIManager::ExitMainMenu() 
{
	isPlayRequested = false;

	m_MainMenuTextRenderer.reset();
	m_TextRenderer->ChangeShader(m_DefaultTextShader);
}

void UIManager::Restart() const
{
	m_TextRenderer->ChangeShader(m_DefaultTextShader);
}

void UIManager::RenderRestartMessage() const
{
	m_BlinkTextShader->Bind();
	m_BlinkTextShader->SetUniform1f("u_Time", TimeUtils::Time);
	m_TextRenderer->ChangeShader(m_BlinkTextShader);
	m_TextRenderer->RenderText("Press R to restart", 20.f, 20.f, 0.5f);
}
