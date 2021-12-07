#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;
in vec4 Pos;
in float Time;

// texture samplers
uniform sampler2D texture1;

void main()
{
	//    FragColor = texture(texture1, TexCoord);
	FragColor = fract(sin(Color * Pos * sin(Time)));
	FragColor = smoothstep(0.f, 1.f, FragColor * sin(Time) + fract(Pos));
}