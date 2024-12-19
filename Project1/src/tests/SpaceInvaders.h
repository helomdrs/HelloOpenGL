#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "tests/SpaceInvaders/GameManager.h"

namespace TestFramework
{
	class SpaceInvaders : public Test
	{
	public:
		SpaceInvaders();
		~SpaceInvaders();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:

		float m_DeltaTime, m_LastFrame;

		std::unique_ptr<GameManager> m_GameManager = std::make_unique<GameManager>();
		std::shared_ptr<InputManager> m_InputManager = std::make_shared<InputManager>();
	};
}