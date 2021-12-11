#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec4 Pos;
in float Time;

void main()
{
	FragColor = sin(Color * Pos * sin(Time));
	FragColor = smoothstep(0.f, 1.f, FragColor * sin(Time) + fract(Pos));
	FragColor = vec4(1.f);
}