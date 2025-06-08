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

#include "CParticleSystem_GPU.h"

/***************************************************************************
* CParticleSystem_GPU: Constructor; initialize variables and create particles
* @parameters: Camera, render program, compute programe, and origin
* @return:
****************************************************************************/
CParticleSystem_GPU::CParticleSystem_GPU(CCamera* _camera, GLuint _progamRender, GLuint _programCompute, glm::vec3 _origin)
{
	// Set variables
	m_sceneCamera = _camera;
	m_programRender = _progamRender;
	m_programCompute = _programCompute;
	m_emitterOrigin = _origin;

	// Create 2 vectors (vec4) to correspond with the VBOs
	std::vector<glm::vec4> PositionLife;
	std::vector<glm::vec4> Velocity;
	// Resize to the dimension equal to the NUM_PARTICLES
	PositionLife.resize(NUM_PARTICLES);
	Velocity.resize(NUM_PARTICLES);

	// Set initial values for each particle. Position, life span, and velocity
	float LifeModifier = 10.0f;
	float Speed_XZ = 8.0f;
	float Speed_Y = 40.0f;
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		PositionLife[i] = glm::vec4(_origin.x, _origin.y, _origin.z,	// Initial position
			RandomFloat() * LifeModifier);								// Initial lifespan

		float Lifespan = RandomFloat() * LifeModifier;					// Reset lifespan
		Velocity[i] = glm::vec4(
			(Speed_XZ * cos((float)i * 0.0167f)) + (Speed_XZ * 2.0f) * RandomFloat() - Speed_XZ,
			(Speed_Y + (Speed_Y * 0.5f) * RandomFloat() - (Speed_Y * 0.25f)),
			(Speed_XZ * sin((float)i * 0.0167f)) + (Speed_XZ * 2.0f) * RandomFloat() - Speed_XZ,
			Lifespan);
	}

	/* Shader storage buffer object */
	// Store position information
	glGenBuffers(1, &m_VBO_PositionLife);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VBO_PositionLife);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		sizeof(glm::vec4) * PositionLife.size(),
		PositionLife.data(),
		GL_DYNAMIC_DRAW);

	// Store velocity information
	glGenBuffers(1, &m_VBO_Velocity);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VBO_Velocity);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		sizeof(glm::vec4) * Velocity.size(),
		Velocity.data(),
		GL_DYNAMIC_DRAW);

	// Store initial properties information (velocity)
	glGenBuffers(1, &m_VBO_InitialProperties);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VBO_InitialProperties);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		sizeof(glm::vec4) * Velocity.size(),
		Velocity.data(),
		GL_DYNAMIC_DRAW);


	/* Compute to standard render link */
	// VAO for the standard pipeline render
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// VBO for the standard pipeline render
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_PositionLife);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	// Unbinding
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

/***************************************************************************
* ~CParticleSystem_GPU: Deconstructor
* @parameters: 
* @return:
****************************************************************************/
CParticleSystem_GPU::~CParticleSystem_GPU()
{
}

/***************************************************************************
* Update: Update particles
* @parameters: Delta time
* @return:
****************************************************************************/
void CParticleSystem_GPU::Update(float _deltaTime)
{
	glm::vec3 Gravity = glm::vec3(0.0f, -9.8f, 0.0f) * _deltaTime;
	m_velocityLifeChange = glm::vec4(Gravity, _deltaTime);
}

/***************************************************************************
* Render: Render particles
* @parameters:
* @return:
****************************************************************************/
void CParticleSystem_GPU::Render()
{
	// First pass - Compute Pipeline
	glUseProgram(m_programCompute);

	// Uniforms
	glUniform3fv(glGetUniformLocation(m_programCompute, "EmitterOrigin"),
		1, glm::value_ptr(m_emitterOrigin));
	glUniform4fv(glGetUniformLocation(m_programCompute, "VelocityLifeChange"),
		1, glm::value_ptr(m_velocityLifeChange));

	// Bind the storage buffers for compute shader manipulations
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_VBO_PositionLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VBO_Velocity);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_VBO_InitialProperties);

	// Set the compute shader going
	glDispatchCompute(GROUP_COUNT_X,	// X
		1,								// Y
		1);								// Z

	// Wait for compute shader completion and sync all threads
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// Unbinding - Cleanup
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//glUseProgram(0);


	/*--------------------------------------------------------*/

	// Second pass - Standard render pipeline
	glUseProgram(m_programRender);

	// Uniforms
	glm::mat4 P = m_sceneCamera->m_projectionMat;
	glm::mat4 V = m_sceneCamera->m_viewMat;
	glm::mat4 PV = P * V;
	glUniformMatrix4fv(glGetUniformLocation(m_programRender, "PV"),
		1, GL_FALSE, glm::value_ptr(PV));

	// Disable writing to the depth mask
	glDepthMask(GL_FALSE);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	// Re-enable the depth mask
	glDepthMask(GL_TRUE);

	// Unbinding - Cleanup
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
}

float CParticleSystem_GPU::RandomFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
