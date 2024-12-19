#pragma once
#include <map>
#include <memory>
#include <string>

#include "Shader.h"
#include "Texture.h"

class ResourcesManager
{
public:
	static void ClearAll();

	static std::shared_ptr<Shader> LoadShader(const std::string& shaderPath, const std::string& name);
	static std::shared_ptr<Shader> GetShader(const std::string& name);

	static std::shared_ptr<Texture> LoadTexture(const std::string& texturePath, const std::string& name, bool toFlipVertically = false);
	static std::shared_ptr<Texture> GetTexture(const std::string& name);

private:
	ResourcesManager() = default;

	static std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
	static std::map<std::string, std::shared_ptr<Texture>> m_Textures;
};