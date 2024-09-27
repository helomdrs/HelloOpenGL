#pragma once

#include <memory>

#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

namespace TestFramework
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_ProjectionMatrix, m_ViewMatrix;
		glm::vec3 m_TranslationMatrix;
	};
}