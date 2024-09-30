#pragma once

#include <memory>

#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

namespace TestFramework
{
	class TestDvdLogo : public Test
	{
	public:
		TestDvdLogo();
		~TestDvdLogo();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		float m_DvdWidth, m_DvdHeigth, m_Velocity;
		glm::vec2 m_LogoPosition, m_LogoVelocity;

		void UpdatePosition(glm::vec2& logoPosition, const glm::vec2& velocity, float deltaTime);
		bool TouchedBorder(const glm::vec2& logoPosition, glm::vec2& velocity) const;
		void UpdateColor();
	};
}