#include "tests/TestAnimatedShader.h"

#include "vendor/im_gui/imgui.h"

namespace TestFramework
{
	TestAnimatedShader::TestAnimatedShader()
		: m_RedValue(0.0f), m_BlueValue(0.0f), m_IncrementRate(0.05f)
	{
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VertexArray = std::make_unique<VertexArray>();
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_Shader = std::make_unique<Shader>("res/shaders/BasicShader.shader");
	}

	TestAnimatedShader::~TestAnimatedShader()
	{
	}

	void TestAnimatedShader::OnUpdate(float deltaTime)
	{
		if (m_RedValue > 1.0f)
			m_IncrementRate = -0.05f;
		else if (m_RedValue < 0.0f)
			m_IncrementRate = 0.05f;

		m_RedValue += m_IncrementRate * deltaTime;
	}

	void TestAnimatedShader::OnRenderer()
	{
		Renderer renderer;

		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", m_RedValue, 0.5f, m_BlueValue, 1.0f);

		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestAnimatedShader::OnImGuiRenderer()
	{
		ImGui::SliderFloat("Blue value", &m_BlueValue, 0.0f, 1.0f);
	}

}

