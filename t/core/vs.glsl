#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec4 Color;
out vec4 Pos;
out float Time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	//gl_Position = model * view * proj * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Color = vec4(clamp(aPos * aNormal, 0.f, 1.f), aTexCoord.x);
	Time = time;
	Pos = proj * view * model * vec4(aPos, 1.0f);
}