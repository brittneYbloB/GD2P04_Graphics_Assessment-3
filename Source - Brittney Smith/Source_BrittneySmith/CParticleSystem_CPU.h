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
#include <vector>

class CParticleSystem_CPU
{
public:
	CParticleSystem_CPU(class CCamera* _camera, GLuint _program, glm::vec3 _origin);
	~CParticleSystem_CPU();

	void Update(float _deltaTime, int _movementType);
	void Render(CCamera* _camera);

private:
	std::vector<class CParticle> m_particles;
	std::vector<glm::vec3> m_particlePositions;

	CCamera* m_sceneCamera;
	GLuint m_program;
	GLuint m_VAO;
	GLuint m_VBO;
	unsigned int m_particleCount;
	GLuint m_texture;

	glm::vec3 m_quadAxisX;
	glm::vec3 m_quadAxisY;
};

