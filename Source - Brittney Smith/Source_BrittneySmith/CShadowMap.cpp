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

#include "CShadowMap.h"
#include <iostream>

#define WINDOWWIDTH 800
#define WINDOWHEIGHT 800

/***************************************************************************
* CShadowMap: Constructor to create texture/shadowmap
* @parameters:
* @return:
****************************************************************************/
CShadowMap::CShadowMap()
{
	// 1) Create an FBO and Texture to store depth information (Shadow Map)
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	CreateTexture(m_shadowMapTexture, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);	// depth component only

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error: Framebuffer failed to initialize correctly." << std::endl;
	}
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/***************************************************************************
* Bind: Bind the shadow depth buffer
* @parameters:
* @return:
****************************************************************************/
void CShadowMap::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear stencil buffer too if used!
	glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);

	glEnable(GL_DEPTH_TEST);
}

/***************************************************************************
* Unbind: Unbind the framebuffer
* @parameters:
* @return:
****************************************************************************/
void CShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
}

/***************************************************************************
* CreateTexture: Create shadowmap texture
* @parameters: Texture ID, attachment type, internal format, format
* @return:
****************************************************************************/
void CShadowMap::CreateTexture(GLuint& _textureID, GLenum _attachment, GLuint _internalFormat, GLuint _format)
{
	// 2) Generate, bind, and create a texture/s
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, WINDOWWIDTH, WINDOWHEIGHT, 0, _format, GL_UNSIGNED_BYTE, NULL); // match screen size and render across screen (no alpha); null data is populated at runtime

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 3) Attach the texture/s to the framebuffer object
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,			// Target: Framebuffer type
		_attachment,			// Attachment type				// Can be altered to create multiple attachments for multiple textures (useful for splitting info, like color, normal or position)
		GL_TEXTURE_2D,			// Texture target type
		_textureID,				// Texture ID
		0);						// Mipmap level

}
