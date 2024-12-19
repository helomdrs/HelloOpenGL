#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;  

out vec2 v_UV;
out vec4 v_ClipSpacePosition;
out vec4 v_PreviousClipSpacePosition;

uniform mat4 u_Model;
uniform mat4 u_PreviousModel;
uniform mat4 u_Projection;

void main()
{
    vec4 localPosition = vec4(a_Position, 0.0f, 1.f);
    v_ClipSpacePosition = u_Projection * u_Model * localPosition;

    gl_Position = v_ClipSpacePosition;

    v_UV = a_UV;
    v_PreviousClipSpacePosition = u_Projection * u_PreviousModel * localPosition;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 o_Color;
layout (location = 1) out vec4 o_Velocity;

in vec2 v_UV;
in vec4 v_ClipSpacePosition;
in vec4 v_PreviousClipSpacePosition;

uniform sampler2D u_Image;
uniform vec3 u_SpriteColor;

void main()
{          
    vec4 textureColor = texture(u_Image, v_UV);
    o_Color = vec4(u_SpriteColor, 1.f) * textureColor;

    vec3 pos = (v_ClipSpacePosition / v_ClipSpacePosition.w).xyz;
    vec3 previousPos = (v_PreviousClipSpacePosition / v_PreviousClipSpacePosition.w).xyz;

    o_Velocity = vec4((pos - previousPos).xy * textureColor.a, 0.f, 1.f);
}
