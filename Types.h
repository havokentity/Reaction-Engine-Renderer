/*
	Engine types
*/


#ifndef RX_ENGINE_TYPES_H
#define RX_ENGINE_TYPES_H

#include "Client.h"

namespace DifferentialArts
{
	#ifdef RX_OPENGL_API
	
		#include <gl/gl.h>
		#include <gl/glu.h>

		//Reduce unsigned variables declaration length
		typedef	unsigned long	ulong;
		typedef unsigned short	ushort;
		typedef unsigned int	uint;
		typedef unsigned char	uchar;

		typedef unsigned int    uint32;
		typedef unsigned short  uint16;

		typedef uint			TextureID;
		typedef void*			RawTextureData;

		typedef const char*		ConstCharPtr;
		
		enum RX_API DataFormatType
		{
			 BYTE_RX	= GL_BYTE,
			 UBYTE_RX	= GL_UNSIGNED_BYTE,
			 SHORT_RX	= GL_SHORT,
			 USHORT_RX	= GL_UNSIGNED_SHORT,
			 INT_RX		= GL_INT,
			 FLOAT_RX	= GL_FLOAT,
			 _2BYTES_RX = GL_2_BYTES,
			 _3BYTES_RX = GL_3_BYTES,
			 _4BYTES_RX = GL_4_BYTES,
			 DOUBLE_RX	= GL_DOUBLE
		};

		enum RX_API TextureMagFilter
		{
			LINEAR	= GL_LINEAR,
			NEAREST = GL_NEAREST
		};

		enum RX_API TextureMinFilter
		{
			NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR,
		};

		enum RX_API TextureTypes
		{
			TEXTURE_1D = GL_TEXTURE_1D,
			TEXTURE_2D = GL_TEXTURE_2D,
			TEXTURE_3D = GL_TEXTURE_3D,
			TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP_ARB,
			TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE_NV
		};



	#endif
}

#endif