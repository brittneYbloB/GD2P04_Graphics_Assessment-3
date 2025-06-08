#version 460 core

in float LifeTime;

// Output
out vec4 FinalColor;


void main()
{
	FinalColor = vec4(1.0f, 1.0f, 0.0f, LifeTime);
}