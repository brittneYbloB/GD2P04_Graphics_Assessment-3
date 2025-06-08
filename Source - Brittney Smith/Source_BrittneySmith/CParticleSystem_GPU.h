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

#pragma once
// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <random>
#include <vector>

#include "CCamera.h"

// Defines
#define GROUP_COUNT_X 20000
#define WORK_GROUP_SIZE_X 128
#define NUM_PARTICLES (WORK_GROUP_SIZE_X * GROUP_COUNT_X)

class CParticleSystem_GPU
{
public:
	CParticleSystem_GPU(CCamera* _camera, GLuint _progamRender, GLuint _programCompute, glm::vec3 _origin);
	~CParticleSystem_GPU();

	void Update(float _deltaTime);
	void Render();

private:
	static float RandomFloat();

	CCamera* m_sceneCamera;
	GLuint m_programRender;
	GLuint m_programCompute;
	GLuint m_VAO;
	GLuint m_VBO_PositionLife;
	GLuint m_VBO_Velocity;
	GLuint m_VBO_InitialProperties;

	glm::vec3 m_emitterOrigin;
	glm::vec4 m_velocityLifeChange;
};

