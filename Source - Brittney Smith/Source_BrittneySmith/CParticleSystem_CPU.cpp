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

#include "CParticleSystem_CPU.h"
#include "CParticle.h"
#include "CCamera.h"
#include "CTextureLoader.h"

/***************************************************************************
* CParticleSystem_CPU: Constructor; initialize variables and create particles
* @parameters: Camera, program, and origin
* @return:
****************************************************************************/
CParticleSystem_CPU::CParticleSystem_CPU(CCamera* _camera, GLuint _program, glm::vec3 _origin)
{
	m_program = _program;
	m_sceneCamera = _camera;

	//m_texture = CTextureLoader::LoadTexture("Images/snowflake.png", m_texture);
	m_texture = CTextureLoader::LoadTexture("Images/fire.png", m_texture);

	// Create particles
	m_particleCount = 1000;
	for (int i = 0; i < (int)m_particleCount; i++)
	{
		// Initialise position vector
		m_particlePositions.push_back(glm::vec3(0.0f));

		CParticle P = CParticle(_origin, i);
		m_particles.push_back(P);
	}

	// Creae the VAO and VBO as normal using position data only and GL_DYNAMIC_DRAW
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec3) * m_particlePositions.size(),
		NULL, GL_DYNAMIC_DRAW);

	// Vertex Attributes (position data only)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);		
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/***************************************************************************
* ~CParticleSystem_CPU: Deconstructor
* @parameters: 
* @return:
****************************************************************************/
CParticleSystem_CPU::~CParticleSystem_CPU()
{
}

/***************************************************************************
* Update: Update particles' movement
* @parameters: Delta time, int for movement type (can later replace with enum)
* @return:
****************************************************************************/
void CParticleSystem_CPU::Update(float _deltaTime, int _movementType)
{
	// Calculate Billboard values
	glm::vec3 CameraFront = m_sceneCamera->m_cameraFront;
	CameraFront = glm::normalize(CameraFront);

	m_quadAxisX = glm::cross(CameraFront, m_sceneCamera->m_cameraUpDir);
	m_quadAxisX = glm::normalize(m_quadAxisX);

	m_quadAxisY = glm::cross(CameraFront, m_quadAxisX);
	m_quadAxisY = glm::normalize(m_quadAxisY);


	// Update the particles and the position array
	for (unsigned int i = 0; i < m_particleCount; i++)
	{
		switch (_movementType)
		{
			// Fountain
			case 0:
			{
				m_particles[i].UpdateFountain(_deltaTime);
				break;
			}
			// Fire
			case 1:
			{
				m_particles[i].UpdateFire(_deltaTime);
				break;
			}
			// Snowfall
			case 2:
			{
				m_particles[i].UpdateSnowfall(_deltaTime);
				break;
			}
			default:
			{
				m_particles[i].UpdateFountain(_deltaTime);
				break;
			}
		}
		m_particlePositions[i] = m_particles[i].GetPosition();
	}

	// Reload the vertex array to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(glm::vec3) * m_particlePositions.size(),
		m_particlePositions.data());

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



/***************************************************************************
* Render: Render particles
* @parameters: Camera (for billboarding)
* @return:
****************************************************************************/
void CParticleSystem_CPU::Render(CCamera* _camera)
{
	// Load uniforms to shader
	glUseProgram(m_program);

	glm::mat4 cameraPV = _camera->m_projectionMat * _camera->m_viewMat;
	glUniformMatrix4fv(glGetUniformLocation(m_program, "PV"),
		1,
		GL_FALSE,
		glm::value_ptr(cameraPV));	// camera PV matrix

	// Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "ImageTexture"), 0);

	// Billboarding variables
	//glUniform3fv(glGetUniformLocation(m_program, "QuadAxisX"), 1, glm::value_ptr(m_quadAxisX));
	//glUniform3fv(glGetUniformLocation(m_program, "QuadAxisY"), 1, glm::value_ptr(m_quadAxisY));
	
	// Individual Billboarding -> camera values required
	glUniform3fv(glGetUniformLocation(m_program, "CameraPosition"), 1, glm::value_ptr(_camera->m_cameraPos));
	glUniform3fv(glGetUniformLocation(m_program, "CameraUpDir"), 1, glm::value_ptr(_camera->m_cameraUpDir));


	// Disable writing to the depth mask
	glDepthMask(GL_FALSE);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_particleCount);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	// Re-enable the depth mask
	glDepthMask(GL_TRUE);
}
