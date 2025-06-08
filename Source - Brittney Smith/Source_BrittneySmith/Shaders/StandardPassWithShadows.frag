#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;
in vec4 FragPos_LightSpace;

struct DirLight
{
	vec3 Direction;
	vec3 Color;
	float AmbientStrength;
	float SpecularStrength;
};


// Uniform Inputs
uniform sampler2D ImageTexture;
uniform sampler2D ShadowMap;
uniform float GlobalAmbience = 0.1f;

uniform DirLight dirLight;								// Directional lighting

uniform float AmbientStrength;							// Percentage value of ambient light (0 -> 1)
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);		// Generally white (all color channels)

uniform vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);		// Part of diffuse
uniform vec3 LightPos = vec3(-2.0f, 6.0f, 3.0f);		// Part of diffuse


// Output
out vec4 FinalColor;



vec3 CalculateLight_Directional()
{
	// Light direction
	vec3 Normal = normalize(FragNormal);
	//vec3 LightDir = normalize(FragPos - dirLight.Direction);	// fragpos should not be involved!
	vec3 LightDir = normalize(dirLight.Direction);	// fragpos should not be involved!

	// Ambient Component
	vec3 Ambient = dirLight.AmbientStrength * AmbientColor;

	// Diffuse Component
	float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);	
	vec3 Diffuse = DiffuseStrength * dirLight.Color;

	// Combine the lighting components
	vec3 Light = vec3(Ambient + Diffuse);

	return Light;
}



float ShadowCalculation()
{
	// Perspective divide to get NDC range -1 -> 1
	vec3 NDC_Space = FragPos_LightSpace.xyz / FragPos_LightSpace.w;

	// Convert to Texture Coordinate Space 0 - 1
	vec3 TexCoordSpace = 0.5f * NDC_Space + 0.5f;

	float ShadowBias = 0.001f;
	float CurrentDepth = TexCoordSpace.z - ShadowBias;	// Shadow acne solution --> Shadow bias

	float ShadowMapDepth = texture(ShadowMap, TexCoordSpace.xy).r;
	float Shadow = CurrentDepth > ShadowMapDepth ? 1.0f : 0.0f;
	
	//// PCF (higher quality shadows)
	//vec2 TexelSize = 1.0f / textureSize(ShadowMap, 0);
	//float Shadow = 0.0f;
	//int Count = 0;
	//for (int Row = -1; Row <= 1; Row++)
	//{
	//	for (int Col = -1; Col <= 1; Col++)
	//	{
	//		vec2 TextureCoordOffset = TexCoordSpace.xyz + vec2(Row, Col) * TexelSize;
	//		float PCF_Depth = texture(ShadowMap, TextureCoordOffset).x;
	//		Shadow += CurrentDepth > PCF_Depth ? 1.0f : 0.0f;
	//		Count++;
	//	}
	//}
	//Shadow /= float(Count);	// Average results

	return Shadow;
}



void main()
{
	// Calculate the effect of the Direction Light
	vec3 LightOutput = CalculateLight_Directional(); // DirLight

	float Shadow = ShadowCalculation();
	vec3 LightShadow = GlobalAmbience + ((1.0f - Shadow) * LightOutput);	// vec3

	// Calculate the final color
	FinalColor = vec4(LightShadow, 1.0f) * texture(ImageTexture, FragTexCoords);
}