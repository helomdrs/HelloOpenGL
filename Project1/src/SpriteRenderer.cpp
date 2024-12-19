#include "SpriteRenderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "vendor/glm/ext/matrix_transform.hpp"

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader)
	: m_Shader(shader)
{
	m_VertexArrayObject = std::make_unique<VertexArray>();

	float vertices[] =
	{
		0.f, 1.f,	0.f, 1.f,
		1.f, 1.f,	1.f, 1.f,
		1.f, 0.f,	1.f, 0.f,
		0.f, 0.f,	0.f, 0.f
	};

	m_VertexBufferObject = std::make_unique<VertexBuffer>(vertices, (sizeof(vertices)));

	VertexBufferLayout layout{};
	layout.Push<float>(2);
	layout.Push<float>(2);
	m_VertexArrayObject->AddBuffer(*m_VertexBufferObject, layout);

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_IndexBufferObject = std::make_unique<IndexBuffer>(indices, 6);
}

SpriteRenderer::~SpriteRenderer(){}

void SpriteRenderer::Draw(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const
{
	Draw(*m_Shader, texture, position, size, rotate, color);
}

void SpriteRenderer::Draw(Shader& shader, Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const
{
	shader.Bind();
	glm::mat4 model{ 1.f };
	model = glm::translate(model, glm::vec3(position, 0.f));

	//quad starts on top left, so its necessary to rotate on center
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));

	model = glm::scale(model, glm::vec3(size, 1.f));

	shader.SetUniformMat4f("u_Model", model);
	shader.SetUniform3f("u_SpriteColor", color);

	texture.Bind(0);
	m_VertexArrayObject->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IndexBufferObject->GetCount(), GL_UNSIGNED_INT, nullptr));
}
