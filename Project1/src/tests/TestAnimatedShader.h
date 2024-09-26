#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

#include<memory>

namespace TestFramework
{
	class TestAnimatedShader : public Test
	{
	public:
		TestAnimatedShader();
		~TestAnimatedShader();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		float m_RedValue;
		float m_IncrementRate;
	};
}