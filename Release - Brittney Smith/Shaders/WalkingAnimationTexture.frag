#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;
uniform float nx_frames;
uniform float ny_frames;
uniform float uv_x;
uniform float uv_y;

// Output
out vec4 FinalColor;



void main()
{
    float x = 1.0f / nx_frames;
    float y = 1.0f / ny_frames;

    FinalColor = texture(ImageTexture, vec2(FragTexCoords.x * x, FragTexCoords.y * y) + vec2(x * uv_x, y * uv_y));
}