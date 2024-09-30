#include "TestDvdLogo.h"
#include "ErrorHandler.h"

#include "vendor/im_gui/imgui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include <random>

namespace TestFramework
{
	TestFramework::TestDvdLogo::TestDvdLogo()
		: 	m_Velocity(0.1f), m_DvdHeigth(0.2f), m_DvdWidth(0.4f),
			m_LogoPosition(glm::vec2(0.0f, 0.0f)), m_LogoVelocity(glm::vec2(m_Velocity, m_Velocity))
	{
		//an array with the vertex coordinates of the triangle and the texture coordinates
		float positions[] = {
			-m_DvdWidth / 2,	-m_DvdHeigth / 2,	0.0f, 0.0f,		//0 - bottom left
			 m_DvdWidth / 2,	-m_DvdHeigth / 2,	1.0f, 0.0f,		//1 - bottom right
			 m_DvdWidth / 2,	 m_DvdHeigth / 2,	1.0f, 1.0f,		//2 - upper right
			-m_DvdWidth / 2,	 m_DvdHeigth / 2,	0.0f, 1.0f		//3 - upper left
		};

		//use an array of indices so more than one triangle can share the same vertexes
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//enable the blend and define how openGL will blend alpha pixels
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//creates an unique pointer to the heap of an vertex array and buffer objects
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		//the buffer stores 4 floats: 2 for vertices and 2 for texture coordinates
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		//add the buffer created on the layout to the vertex array
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		//creates an unique pointer to the heap of an index buffer object
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//creates an unique pointer to the heap of an shader object
		m_Shader = std::make_unique<Shader>("res/shaders/DVDShader.shader");
		m_Shader->Bind();

		//set an uniform on the shader for the texture on the same slot we binded it
		m_Texture = std::make_unique<Texture>("res/textures/dvd.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestDvdLogo::~TestDvdLogo() {}

	void TestFramework::TestDvdLogo::OnUpdate(float deltaTime)
	{
		Renderer renderer;
		
		UpdatePosition(m_LogoPosition, m_LogoVelocity, deltaTime);
		
		bool hasCollided = TouchedBorder(m_LogoPosition, m_LogoVelocity);
		if (hasCollided)
			UpdateColor();

		//Draw call
		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestFramework::TestDvdLogo::OnRenderer()
	{
		//make the background black
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();
		m_Shader->Bind();

		//Draw call
		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestDvdLogo::OnImGuiRenderer()
	{
		//TODO : create faster and slower button here
		if (ImGui::Button("Faster"))
		{
			m_Velocity += 0.1;
			m_LogoVelocity = glm::vec2(m_Velocity, m_Velocity);
		}

		if (ImGui::Button("Slower"))
		{
			m_Velocity -= 0.1;
			m_LogoVelocity = glm::vec2(m_Velocity, m_Velocity);
		}
	}

	void TestDvdLogo::UpdatePosition(glm::vec2& logoPosition, const glm::vec2& velocity, float deltaTime)
	{
		logoPosition.x += velocity.x * deltaTime;
		logoPosition.y += velocity.y * deltaTime;
		m_Shader->SetUniform2f("u_Transform", logoPosition.x, logoPosition.y);
	}

	bool TestDvdLogo::TouchedBorder(const glm::vec2& logoPosition, glm::vec2& velocity) const
	{
		float widthBound = 1 - m_DvdWidth / 2;
		float heightBound = 1 - m_DvdHeigth / 2;

		bool touchedHorizontalBorders = logoPosition.x >= widthBound || logoPosition.x <= -widthBound;
		bool touchedVerticalBorders = logoPosition.y >= heightBound || logoPosition.y <= -heightBound;

		if (touchedHorizontalBorders || touchedVerticalBorders)
		{
			if (touchedHorizontalBorders) velocity.x *= -1;
			if (touchedVerticalBorders) velocity.y *= -1;
			return true;
		}

		return false;
	}

	void TestDvdLogo::UpdateColor()
	{
		float r = ((float)rand() / RAND_MAX) * (1 - 0) + 0;
		float g = ((float)rand() / RAND_MAX) * (1 - 0) + 0;
		float b = ((float)rand() / RAND_MAX) * (1 - 0) + 0;
		m_Shader->SetUniform4f("u_Color", r, g, b, 1.0f);
	}

}