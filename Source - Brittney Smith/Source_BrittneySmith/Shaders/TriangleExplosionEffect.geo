#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GS_VERTEX
{
	in vec4 Position;
    in vec2 TexCoords;
	in vec3 Normal;
} gs_in[];	

out vec2 FragTexCoords;


uniform mat4 PVMMatrix;
uniform float time;


vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    FragTexCoords = gs_in[0].TexCoords;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    FragTexCoords = gs_in[1].TexCoords;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    FragTexCoords = gs_in[2].TexCoords;
    EmitVertex();

    EndPrimitive();
}