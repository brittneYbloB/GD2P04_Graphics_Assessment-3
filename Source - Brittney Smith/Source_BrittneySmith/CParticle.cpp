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

#include "CParticle.h"

/***************************************************************************
* CParticle: Constructor
* @parameters: Origin and ID
* @return:
****************************************************************************/
CParticle::CParticle(glm::vec3 _origin, unsigned int _ID)
{
	this->m_origin = _origin;
	this->m_ID = _ID;

	// Set the default properties by calling the reset function
	ResetFire();
}

/***************************************************************************
* ~CParticle: Deconstructor
* @parameters: 
* @return:
****************************************************************************/
CParticle::~CParticle()
{
}

/***************************************************************************
* UpdateFountain: Fountain particle movement
* @parameters: Delta time
* @return:
****************************************************************************/
void CParticle::UpdateFountain(float _deltaTime)
{
	// Control the movement of the particle (will change dependent on the type of particle: fountain, snow, etc)

	// Update the velocity (V = -g * t)
	m_velocity.y += -1.0f * _deltaTime;
	m_position += m_velocity;

	// Decrease lifetime and reset if expired
	m_timeRemaining -= _deltaTime;
	if (m_timeRemaining <= 0.0f)
	{
		ResetFountain();	// Restart the particle
	}

}

/***************************************************************************
* UpdateFire: Fire particle movement
* @parameters: Delta time
* @return:
****************************************************************************/
void CParticle::UpdateFire(float _deltaTime)
{
	// Update the velocity (V = g * t); disabled gravity
	m_velocity.y += 0.001f * _deltaTime;


	m_position += m_velocity;

	// Decrease lifetime and reset if expired
	m_timeRemaining -= _deltaTime;
	if (m_timeRemaining <= 0.0f)
	{
		ResetFire();	// Restart the particle
	}
}

/***************************************************************************
* UpdateSnowfall: Snowfall particle movement
* @parameters: Delta time
* @return:
****************************************************************************/
void CParticle::UpdateSnowfall(float _deltaTime)
{
}

/***************************************************************************
* ResetFountain: Reset the particle when expired
* @parameters:
* @return:
****************************************************************************/
void CParticle::ResetFountain()
{
	m_position = m_origin;
	m_velocity = glm::vec3(
		0.25f * cos((float)m_ID * 0.0167f) + 0.25f * RandomFloat() - 0.125f,
		1.5f + 0.25f * RandomFloat() - 0.125f,
		0.25f * sin((float)m_ID * 0.0167f) + 0.25f * RandomFloat() - 0.125f);
	m_velocity *= 0.1f;
	m_timeRemaining = RandomFloat() + 0.125f;
}

/***************************************************************************
* ResetFire: Reset the particle when expired
* @parameters:
* @return:
****************************************************************************/
void CParticle::ResetFire()
{
	m_position = m_origin;
	m_velocity = glm::vec3(
		0.2f * sin((float)m_ID * 0.0167f) + 0.2f * RandomFloat() - 0.125f,
		1.5f + 0.25f * RandomFloat() - 0.125f,
		0.2f * cos((float)m_ID * 0.0167f) + 0.2f * RandomFloat() - 0.125f);
	m_velocity *= 0.1f;
	m_timeRemaining = RandomFloat() + 0.125f;
}

/***************************************************************************
* RandomFloat: Generate a random float
* @parameters:
* @return: Random float
****************************************************************************/
float CParticle::RandomFloat()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}