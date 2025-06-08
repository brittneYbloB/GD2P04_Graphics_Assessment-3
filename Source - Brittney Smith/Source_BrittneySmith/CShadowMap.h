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

class CShadowMap
{
public:
	CShadowMap();
	~CShadowMap() {};

	void Bind();
	void Unbind();

	const GLuint GetShadowMap() const { return m_shadowMapTexture; };

private:
	void CreateTexture(GLuint& _textureID,
		GLenum _attachment,
		GLuint _internalFormat = GL_RGB,
		GLuint _format = GL_RGB);

private:
	GLuint m_FBO;
	GLuint m_shadowMapTexture;
};

