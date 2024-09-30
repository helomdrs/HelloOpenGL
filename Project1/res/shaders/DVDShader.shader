#shader vertex
#version 330 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoords;

out vec2 v_TextureCoords;

uniform vec2 u_Transform;

void main()
{
	gl_Position = vec4(position.xy + u_Transform, position.z, 1.0);
	v_TextureCoords = textureCoords;
}

//----------------

#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 v_TextureCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	color = texture(u_Texture, v_TextureCoords) * u_Color;
}