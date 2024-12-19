#include "ResourcesManager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "vendor/stb_image/stb_image.h"

std::map<std::string, std::shared_ptr<Shader>> ResourcesManager::m_Shaders{};
std::map<std::string, std::shared_ptr<Texture>> ResourcesManager::m_Textures{};

void ResourcesManager::ClearAll()
{
	m_Shaders.clear();
	m_Textures.clear();
}

std::shared_ptr<Shader> ResourcesManager::LoadShader(const std::string& shaderPath, const std::string& name)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderPath);
	m_Shaders[name] = shader;

	return m_Shaders[name];
}

std::shared_ptr<Shader> ResourcesManager::GetShader(const std::string& name)
{
	return m_Shaders[name];
}

std::shared_ptr<Texture> ResourcesManager::LoadTexture(const std::string& texturePath, const std::string& name, bool toFlipVertically)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(texturePath, toFlipVertically);

	m_Textures[name] = texture;

	return m_Textures[name];
}

std::shared_ptr<Texture> ResourcesManager::GetTexture(const std::string& name)
{
	return m_Textures[name];
}