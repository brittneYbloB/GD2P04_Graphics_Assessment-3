#version 460 core

layout (points) in;									// Receive points in
layout (triangle_strip, max_vertices = 4) out;		// Send out triangle strip (make a quad)

// Uniforms
uniform mat4 PV;
uniform vec3 QuadAxisX;
uniform vec3 QuadAxisY;
// Outputs
out vec2 TexCoord;


void main()
{
	float Size = 0.3f;

	vec3 Point_BL = gl_in[0].gl_Position.xyz + vec3(-QuadAxisX + QuadAxisY) * Size;
	gl_Position = PV * vec4(Point_BL, 1.0f);
	TexCoord = vec2(0.0f, 0.0f);
	EmitVertex();

	vec3 Point_BR = gl_in[0].gl_Position.xyz + vec3(QuadAxisX + QuadAxisY) * Size;
	gl_Position = PV * vec4(Point_BR, 1.0f);
	TexCoord = vec2(1.0f, 0.0f);
	EmitVertex();

	vec3 Point_TL = gl_in[0].gl_Position.xyz + vec3(-QuadAxisX - QuadAxisY) * Size;
	gl_Position = PV * vec4(Point_TL, 1.0f);
	TexCoord = vec2(0.0f, 1.0f);
	EmitVertex();

	vec3 Point_TR = gl_in[0].gl_Position.xyz + vec3(QuadAxisX - QuadAxisY) * Size;
	gl_Position = PV * vec4(Point_TR, 1.0f);
	TexCoord = vec2(1.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}