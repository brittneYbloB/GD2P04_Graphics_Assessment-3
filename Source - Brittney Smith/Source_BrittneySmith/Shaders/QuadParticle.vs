#version 460 core

layout (location = 0) in vec3 Position;

uniform mat4 PV;

void main()
{
	// Pass through unaltered position
	gl_Position = vec4(Position, 1.0f);
}