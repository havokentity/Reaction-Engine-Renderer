/*
	Description: OpenGL Multitexture procedure externs
*/

#ifndef RX_GL_MULTITEX_EXTE_H_
#define RX_GL_MULTITEX_EXTE_H_

#include <gl/glext.h> //gl extensions header
#include "Client.h"

namespace DifferentialArts
{
	extern RX_API PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB;

	extern RX_API PFNGLMULTITEXCOORD4FARBPROC			glMultiTexCoord4fARB;
	extern RX_API PFNGLMULTITEXCOORD4FVARBPROC			glMultiTexCoord4fvARB;

	extern RX_API PFNGLACTIVETEXTUREARBPROC				glActiveTextureARB;
	extern RX_API PFNGLCLIENTACTIVETEXTUREARBPROC	    glClientActiveTextureARB;
}

#endif