#include "SpaceInvaders.h"
#include "ErrorHandler.h"

#include "vendor/im_gui/imgui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace TestFramework
{
	TestFramework::SpaceInvaders::SpaceInvaders()
		: m_DeltaTime(0.0f), m_LastFrame(0.0f)
	{
		//AudioManager::Init();
		m_GameManager->Init();
	}

	SpaceInvaders::~SpaceInvaders() 
	{
		m_GameManager->Close();
		m_GameManager.reset();
	}

	void TestFramework::SpaceInvaders::OnUpdate(float deltaTime)
	{
		const float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - deltaTime;
		m_LastFrame = currentFrame;

		TimeUtils::Time = currentFrame;
		TimeUtils::DeltaTime = m_DeltaTime;

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_GameManager->ProcessInput(m_DeltaTime, *m_InputManager);

		m_GameManager->Update(m_DeltaTime);
	}

	void TestFramework::SpaceInvaders::OnRenderer()
	{
		m_GameManager->Render();
	}

	void TestFramework::SpaceInvaders::OnImGuiRenderer()
	{
		//we can change some values here
	}

}