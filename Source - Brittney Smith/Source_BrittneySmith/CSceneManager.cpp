/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) [2022] Media Design School
File Name :
Description : GD2P04 : 3D Graphics Programming
Author : Brittney Smith
Mail : Brittney.Smith@mds.ac.nz
**************************************************************************/

#include "CSceneManager.h"

/***************************************************************************
* InitialSetup: Set-up the viewport, time manager, camera class,
*				programs, textures, VAOs, VBOs, EBOs, and enable culling
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::InitialSetup()
{
	srand((unsigned)time(NULL));
	
	// Perlin Noise
	int seed = (int)time(NULL);
	m_perlinNoise = new CPerlinNoise();
	m_perlinNoise->m_Seed = seed;
	m_perlinNoise->SaveToFile();

	// Enabling Depth Testing for 3D
	glEnable(GL_DEPTH_TEST);
	// Declaring a Depth Test function
	glDepthFunc(GL_LESS);	// LESS than the stored depth value

	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// (red, green, blue, alpha)

	// Setting up the Viewport
	glViewport(0, 0, 800, 800);

	// Setting up time-manager pointer as is a non-static class
	m_pTimeManager = new CTimeManager();

	// Setting up the camera
	m_pCamera = new CCamera();

	// Light manager
	m_pLightManager = new CLightManager();

	// Create skybox/cubemap
	m_pSkyBox = new CSkyBox(m_pCamera);

	// Input for callback functions
	m_pInputManager = new CInputManager(m_pCamera, m_pTimeManager, m_pLightManager, this);
	m_pInputManager->SetCallbacks(m_pWindow);

	// Enable blending to only render the actual character pixels & no BG
	// Part of the setup for the Per-Fragment Processing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create programs
	CreatePrograms();

	// Particle system
	m_particleSystem = new CParticleSystem_CPU(m_pCamera, m_programGeometryIndivBillboardingQuads, glm::vec3(0.0f, 0.0f, 0.0f));
	m_particleSystem_GPU = new CParticleSystem_GPU(m_pCamera, m_programParticleGPU, m_programComputeShader, glm::vec3(0.0f, 0.0f, 0.0f));
	//m_particleSystem_GPU = new CParticleSystem_GPU(m_pCamera, m_programGeometryShaderParticlesGPU, m_programComputeShader, glm::vec3(0.0f, 0.0f, 0.0f));

	// Load textures
	m_textureCourage = CTextureLoader::LoadTexture("Images/Courage.jpg", m_textureCourage);

	// Normal sphere object
	m_pSphere_1 = new CSphere(0.5f, 10, m_textureCourage);
	m_pSphere_2 = new CSphere(0.5f, 10, m_textureCourage);
	m_pSphere_1->m_objPosition = glm::vec3(0.0f, 0.1f, 1.0f);
	m_pSphere_2->m_objPosition = glm::vec3(0.0f, 0.6f, 1.0f);

	// Skulls
	//m_skullModel.Mesh_Model("Models/Skull/source/ceramicskull_obj/", "ceramicskull.obj");
	//m_skullModel.m_objPosition = { 0.0f, 0.0f, 1.0f };

	// Doggie
	//m_doggieModel.Mesh_Model("Models/Doggie/", "LucyFullColor.obj");
	//m_doggieModel.m_objPosition = { 0.5f, 0.5f, 1.0f };
	//m_doggieModel.m_objScale3D = { 0.00075f, 0.00075f, 0.00075f };


	// Terrain
	m_heightMap = new CHeightMap("Models/Terrain/Heightmap.raw");

	// Framebuffer Object
	m_framebufferObject = new CFramebuffer();

	// Shadow Mapping
	m_shadowRenderTarget = new CShadowMap();

	// Quad
	m_pQuad = new CQuad();

	// Geometry shader shape test
	m_pointsGeometryShader = new CPointsMesh();


	// Clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// GLOBAL Culling
	glEnable(GL_CULL_FACE);	// Enable face culling globally
	glCullFace(GL_BACK);	// Does not render back-facing polygons
	glFrontFace(GL_CCW);	// Sets the winding order for fron-facing polygons (set to counterclockwise)

	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Enable Multi Sampling
	glEnable(GL_MULTISAMPLE);


	// Hide the cursor and capture it
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/***************************************************************************
* Update: Update delta time and user input
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::DefaultUpdate()
{
	glfwPollEvents();

	// Text input update
	m_pInputManager->TextCallbackInput(m_pWindow);

	// Calculate delta time
	m_pTimeManager->CalculateDeltaTime();

	// Calculate camera projection matrix
	m_pCamera->CalculateProjectionMatrix();

	// Calculate camera view matrix
	m_pCamera->CalculateViewMatrix();

	// Update SkyBox
	m_pSkyBox->Update(m_pTimeManager->m_fDeltaTime);
}


/***************************************************************************
* Update_Scene1: Geometry Shader
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene1()
{
	//m_skullModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	//m_doggieModel.UpdateObject(0.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphere_1->UpdateObject(10.0f, glm::vec3(0, 1, 0), m_pTimeManager->m_fDeltaTime);
	m_pSphere_2->UpdateObject(10.0f, glm::vec3(1, 1, 1), m_pTimeManager->m_fDeltaTime);

	m_heightMap->UpdateObject();
}

/***************************************************************************
* Update_Scene2: Tessellation Shaders + Level of Detail (LOD)
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene2()
{
	// Particle system
	m_particleSystem->Update(m_pTimeManager->m_fDeltaTime, 1);	// fire
}

/***************************************************************************
* Update_Scene3: Framebuffers + Post-Processing
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene3()
{
	m_particleSystem_GPU->Update(m_pTimeManager->m_fDeltaTime);
}

/***************************************************************************
* Render_Scene4:
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Update_Scene4()
{
}



/***************************************************************************
* Render_Scene1: 
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene1()
{
	m_pLightManager->LightsPerspective(m_pLightManager);	// light PV matrix

	// Shadow render pass
	// Bind
	m_shadowRenderTarget->Bind();
	// Render all objects that can cast shadows
	m_pSphere_1->ShadowPassRender(m_programShadowPass, m_pCamera, m_pLightManager);
	m_pSphere_2->ShadowPassRender(m_programShadowPass, m_pCamera, m_pLightManager);
	m_heightMap->ShadowRender(m_programShadowPass, m_pCamera, m_pLightManager);
	// Unbind
	m_shadowRenderTarget->Unbind();

	// Render scene as normal
	m_pSkyBox->Render(m_programSkyBox, m_pCamera);
	m_pSphere_1->StandardRender(m_programStandardShadowPass, m_pCamera, m_pLightManager, m_shadowRenderTarget);
	m_pSphere_2->StandardRender(m_programStandardShadowPass, m_pCamera, m_pLightManager, m_shadowRenderTarget);
	m_heightMap->StandardRender(m_programShadowedHeightmap, m_pCamera, m_pLightManager, m_shadowRenderTarget);
}

/***************************************************************************
* Render_Scene2: 
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene2()
{
	// Skybox
	//m_pSkyBox->Render(m_programSkyBox, m_pCamera);

	// Particle system
	m_particleSystem->Render(m_pCamera);
}

/***************************************************************************
* Render_Scene3: 
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene3()
{
	// Skybox
	//m_pSkyBox->Render(m_programSkyBox, m_pCamera);

	m_particleSystem_GPU->Render();
}


/***************************************************************************
* Render_Scene4:
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::Render_Scene4()
{
}



/***************************************************************************
* CreatePrograms: Create/set programs
* @parameter:
* @return:
****************************************************************************/
void CSceneManager::CreatePrograms()
{
	// Create lighting programs
	m_programCombinedLights = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/CombinedLighting.frag");
	m_programShadowPass = CShaderLoader::CreateProgram("Shaders/ShadowPass.vs", "Shaders/ShadowPass.frag");
	m_programStandardShadowPass = CShaderLoader::CreateProgram("Shaders/StandardPassWithShadows.vs", "Shaders/StandardPassWithShadows.frag");
	m_programShadowedHeightmap = CShaderLoader::CreateProgram("Shaders/ShadowedHeightMap.vs", "Shaders/ShadowedHeightMap.frag");
	m_programLitHeightmap = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/LitHeightmap.frag");
	m_program3DTexture = CShaderLoader::CreateProgram("Shaders/3D_Normal.vs", "Shaders/Fog.frag");
	m_programSkyBox = CShaderLoader::CreateProgram("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	m_programSkyBoxFog = CShaderLoader::CreateProgram("Shaders/SkyBox.vs", "Shaders/SkyboxFog.frag");
	// Geometry shader program
	m_programGeometryShader = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertex.vert", "Shaders/Triangle.geo", "Shaders/StaticTexture.frag");
	m_programGeometryShaderShowingNormals = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertexShowingNormals.vert", "Shaders/TriangleShowingNormals.geo", "Shaders/FixedColor.frag");
	m_programGeometryShaderExplosion = CShaderLoader::CreateProgram_VGF("Shaders/GeoVertexShowingNormals.vert", "Shaders/TriangleExplosionEffect.geo", "Shaders/StaticTexture.frag");
	m_programGeometryFountainQuads = CShaderLoader::CreateProgram_VGF("Shaders/QuadParticle.vs", "Shaders/QuadParticle.geo", "Shaders/QuadParticle.frag");
	m_programGeometryBillboardingQuads = CShaderLoader::CreateProgram_VGF("Shaders/QuadParticle.vs", "Shaders/QuadParticleBillboarding.geo", "Shaders/QuadParticle.frag");
	m_programGeometryIndivBillboardingQuads = CShaderLoader::CreateProgram_VGF("Shaders/QuadParticle.vs", "Shaders/QuadParticleIndividualBillboarding.geo", "Shaders/QuadParticle.frag");
	// Tessallation programs
	m_programQuadPatch_TS = CShaderLoader::CreateProgram_VTF("Shaders/TessellationVS.vs", "Shaders/TCS_QuadPatch.tcs", "Shaders/TES_QuadPatch.tes", "Shaders/FixedColor.frag");
	m_programTrianglePatch_TS = CShaderLoader::CreateProgram_VTF("Shaders/TessellationVS.vs", "Shaders/TCS_TrianglePatch.tcs", "Shaders/TES_TrianglePatch.tes", "Shaders/StaticTexture.frag"); // FixedColor
	m_programTerrain_TS = CShaderLoader::CreateProgram_VTF("Shaders/3D_Normal.vs", "Shaders/TCS_TrianglePatch.tcs", "Shaders/TES_TrianglePatch.tes", "Shaders/LitHeightmap.frag");
	// Post processprograms
	m_programInvertedColor = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/InvertColor.frag");
	m_programGreyscale = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/Greyscale.frag");
	m_programRainingOnYourScreen = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/RainingOnYourScreen.frag");
	m_programTV_CRT = CShaderLoader::CreateProgram("Shaders/2DShape.vs", "Shaders/TV_CRT_Pixels.frag");
	// Particle system
	m_programParticleCPU = CShaderLoader::CreateProgram("Shaders/PointParticle.vs", "Shaders/PointParticle.frag");
	m_programParticleGPU = CShaderLoader::CreateProgram("Shaders/StandardRenderPipeline.vs", "Shaders/StandardRenderPipeline.frag");
	m_programComputeShader = CShaderLoader::CreateComputeShader("Shaders/ComputeShader.cmpts");
	m_programGeometryShaderParticlesGPU = CShaderLoader::CreateProgram_VGF("Shaders/StandardRenderPipeline.vs", "Shaders/QuadParticleBillboarding.geo", "Shaders/StandardRenderPipeline.frag");
}


