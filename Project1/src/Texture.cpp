#include "Texture.h"
#include "ErrorHandler.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BytesPerPixel(0)
{
	//flips the image upside down because OpenGL's (0,0) is on the bottom left corner :clown:
	stbi_set_flip_vertically_on_load(1);

	//loads the texture data and store it on m_LocalBuffer (desired chanels is 4 becuase is RGBA)
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BytesPerPixel, 4);

	//generates the texture and binds (select) it
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//set up some settings for the texture (if is not defined, wont show on screen)

	//specify how the texture will be scalated if the texture is bigger or smaller than the window
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//clamps the texture so its not repeated on the window (on X/s and Y/t)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//give the OpenGL the data of the texture loaded by stbi_load
	//internalFormat is how we want OpenGL to read the data - 8 is the bits per chanel 
	//format is the format of the data we're proving to OpenGL
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	//if the buffer has content, we free it to free memory on the CPU
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	//the destructor will destroy the textures on m_RendererID
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	//OpenGL has 32 chanels for texture, you can specify the slot you want to store/retrieve from
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	//unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
