/*
	Description: OpenGL Multitexture procedure externs
*/

#ifndef RX_GL_MULTITEX_ASSIGNERS_H_
#define RX_GL_MULTITEX_ASSIGNERS_H_

#include <gl/glext.h> //gl extensions header
#include "Client.h"

namespace DifferentialArts
{
	RX_API PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	 = NULL;

	RX_API PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB	 = NULL;
	RX_API PFNGLMULTITEXCOORD4FVARBPROC		glMultiTexCoord4fvARB	 = NULL;

	RX_API PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;
	RX_API PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB = NULL;
}

#endif