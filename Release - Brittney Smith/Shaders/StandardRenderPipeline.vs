#version 460 core

layout (location = 0) in vec4 Position;

uniform mat4 PV;
out float LifeTime;


void main()
{
	gl_Position = PV * vec4(Position.xyz, 1.0f);
	LifeTime = Position.w;
}