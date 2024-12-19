#pragma once
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <ft2build.h>

#include FT_FREETYPE_H

#include "vendor/glm/vec2.hpp"
#include "vendor/glm/vec3.hpp"
#include "vendor/glm/ext/matrix_clip_space.hpp"

#include "tests/SpaceInvaders/Utils/ResourcesManager.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

class Texture;
class Shader;

struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer
{
public:
	TextRenderer(unsigned int width, unsigned int height, const std::string& fontPath, unsigned int fontSize, const std::shared_ptr<Shader>& shader);
	~TextRenderer();
	
	void RenderText(const std::string& text, float posX, float posY, float scale, const glm::vec3& color = { 1.f, 1.f, 1.f });
	void SetupShader(const std::shared_ptr<Shader>& shader) const;
	void ChangeShader(const std::shared_ptr<Shader>& shader);

private:
	std::map<char, Character> m_Characters;
	std::shared_ptr<Shader> m_Shader;
	std::unique_ptr<VertexArray> m_VertexArrayObject;
	std::unique_ptr<VertexBuffer> m_VertexBufferObject;
	std::unique_ptr<IndexBuffer> m_IndexBufferObject;

	unsigned int m_ScreenWidth{ 0 };
	unsigned int m_ScreenHeight{ 0 };

	void LoadFont(const std::string& fontPath, unsigned int fontSize);
};