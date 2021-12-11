#version 330 core
layout (location = 0) in vec3 aPos;

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
	//Color = vec4(clamp(aPos, 0.f, 1.f), sin(time));
	Color = vec4(1.f);
	Time = time;
	Pos = proj * view * model * vec4(aPos, 1.0f);
}