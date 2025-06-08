#version 460 core

// Vertex data interpretation
layout (location = 0) in vec3 Position;

// Inputs
uniform mat4 LightMatrix_PV;
uniform mat4 Model;


void main()
{
	// Calculate the vertex position
	gl_Position = (LightMatrix_PV * Model) * vec4(Position, 1.0f);
}