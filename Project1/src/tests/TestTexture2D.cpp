#include "TestTexture2D.h"
#include "ErrorHandler.h"

#include "vendor/im_gui/imgui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace TestFramework
{
	TestFramework::TestTexture2D::TestTexture2D()
		: m_TranslationMatrix(200, 200, 0), 
		  m_ProjectionMatrix(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		  m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//an array with the vertex coordinates of the triangle and the texture coordinates
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,	//0 - botom left
			 50.0f, -50.0f, 1.0f, 0.0f, //1
			 50.0f,  50.0f, 1.0f, 1.0f, //2 - upper right
			-50.0f,  50.0f, 0.0f, 1.0f	//3
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
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		//set an uniform on the shader for the texture on the same slot we binded it
		m_Texture = std::make_unique<Texture>("res/textures/tiredakali.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {}

	void TestFramework::TestTexture2D::OnUpdate(float deltaTime)
	{
		//use this if we want to animate something on the texture
	}

	void TestFramework::TestTexture2D::OnRenderer()
	{
		//make the background black
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		{
			//calculate the MVP matrix based on the model matrix for the translation
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_TranslationMatrix);
			glm::mat4 modelViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix * modelMatrix;

			//bind (select) the shader so we can send the uniforms to it
			m_Shader->Bind();

			//sends the projection matrix to the vertex shader so it can be used when applying the positions of the vertices
			m_Shader->SetUniformMat4f("u_MVPmatrix", modelViewProjMatrix);

			//Draw call
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestFramework::TestTexture2D::OnImGuiRenderer()
	{
		ImGui::SliderFloat3("Translation", &m_TranslationMatrix.x, 0.0f, 960.0f);
	}

}