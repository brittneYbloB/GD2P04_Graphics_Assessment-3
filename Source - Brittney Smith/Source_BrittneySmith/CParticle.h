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

class CParticle
{
public:
	CParticle(glm::vec3 _origin, unsigned int _ID);
	~CParticle();

	void UpdateFountain(float _deltaTime);
	void UpdateFire(float _deltaTime);
	void UpdateSnowfall(float _deltaTime);
	void ResetFountain();
	void ResetFire();
	glm::vec3 GetPosition() const { return m_position; }

private:
	static float RandomFloat();

private:
	unsigned int m_ID;
	glm::vec3 m_origin;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_timeRemaining;
};

