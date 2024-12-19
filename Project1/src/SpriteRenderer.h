#pragma once
#include <memory>

#include "vendor/glm/vec2.hpp";
#include "vendor/glm/vec3.hpp";

class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Texture;
class Shader;

class SpriteRenderer
{
public:
	SpriteRenderer(const std::shared_ptr<Shader>& shader);
	~SpriteRenderer();

	void Draw(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2{ 10.f, 10.f }, float rotate = 0.f, glm::vec3 color = glm::vec3{ 1.f }) const;
	void Draw(Shader& shader, Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2{ 10.f, 10.f }, float rotate = 0.f, glm::vec3 color = glm::vec3{ 1.f }) const;

private:
	std::shared_ptr<Shader> m_Shader;
	std::unique_ptr<VertexArray> m_VertexArrayObject;
	std::unique_ptr<VertexBuffer> m_VertexBufferObject;
	std::unique_ptr<IndexBuffer> m_IndexBufferObject;
};