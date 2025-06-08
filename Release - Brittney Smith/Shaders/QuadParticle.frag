#version 460 core

// Inputs
in vec2 TexCoord;
uniform sampler2D ImageTexture;

// Output
out vec4 FinalColor;


void main()
{
	vec4 TexColor = texture(ImageTexture, TexCoord);
	FinalColor = TexColor * vec4(0.898f, 0.9255f, 0.9216f, 1.0f);
}