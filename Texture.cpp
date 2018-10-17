/*

	THIS IS A NON OOP TEXTURE HANDLER and OOP texture handler

	Refer- Texture.h

	JPEG CODE thanks to jpeg group and gametutorials.com
*/

#include "Texture.h"

namespace DifferentialArts
{
	GLuint g_Textures[MAX_TEX] = {0};

	RX_API bool LoadTexture(const char* fileName, GLuint texture, bool Mipmaps, GLuint &width, GLuint &height,
							bool sClampToEdge, bool tClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 2D texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, (GLuint*)&texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
							  height = ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
							  GL_UNSIGNED_BYTE, ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
						 height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT),
						 GL_UNSIGNED_BYTE, ilGetData());		
		}  

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("2D Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadCrossCubeMapTexture(const char* fileName, GLuint texArray[], GLuint texID, bool mipmaps,
										GLuint &width, GLuint &height, GLuint HDRIFormat)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load CUBE MAP texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texArray[texID]);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int _width = width = ilGetInteger(IL_IMAGE_WIDTH);
		int _height = height = ilGetInteger(IL_IMAGE_HEIGHT);
		int _depth = 0;
		int _elementSize = ilGetInteger(IL_IMAGE_BPP);

		//myLog->LogError(LString("w: %d h: %d", _width, _height));

		if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
			return false;

		GLubyte *data = ilGetData();

		int fWidth = _width / 3;
		int fHeight = _height / 4;

		GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
		GLubyte *ptr;

		/* POSITIVE_X */
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[(((fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}	

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		}  

		/* NEGATIVE_X */
		//texID = LEFT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Y */
		//texID = FRONT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Y */
		//texID = BACK;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Z */
		//texID = BOTTOM;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Z */
		//texID = TOP;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			for (int i=0; i<fWidth; i++) {
				memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
				ptr += _elementSize;
			}
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Cube Map Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadCrossCubeMapTexture(const char* fileName, GLuint texture, bool mipmaps,
										GLuint &width, GLuint &height, GLuint HDRIFormat)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load CUBE MAP texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		int _width = width = ilGetInteger(IL_IMAGE_WIDTH);
		int _height = height = ilGetInteger(IL_IMAGE_HEIGHT);
		int _depth = 0;
		int _elementSize = ilGetInteger(IL_IMAGE_BPP);

		//myLog->LogError(LString("w: %d h: %d", _width, _height));

		if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
			return false;

		GLubyte *data = ilGetData();

		int fWidth = _width / 3;
		int fHeight = _height / 4;

		GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
		GLubyte *ptr;

		/* POSITIVE_X */
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[(((fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, (GLuint*)&texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		}  

		/* NEGATIVE_X */
		//texID = LEFT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Y */
		//texID = FRONT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Y */
		//texID = BACK;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Z */
		//texID = BOTTOM;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Z */
		//texID = TOP;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			for (int i=0; i<fWidth; i++) {
				memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
				ptr += _elementSize;
			}
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Cube Map Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadTextureFromLump(GLvoid* lump, GLuint texture, bool Mipmaps, GLuint size_l,
									GLuint &width, GLuint &height,
									bool sClampToEdge, bool tClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 2D texture from lump..."));

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadL(IL_TYPE_UNKNOWN, lump, size_l))
		{
			myGlobalGame->mlog.LogError(LString("Invalid Lump Data\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, (GLuint*)&texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
							  height = ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
							  GL_UNSIGNED_BYTE, ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
						 height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT),
						 GL_UNSIGNED_BYTE, ilGetData());		
		}  

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("2D Texture was loaded successfully from lump!"));

		return true;
	}

	RX_API bool LoadCrossCubeMapTextureFromLump(GLvoid* lump, GLuint texture, bool mipmaps, GLuint size_l,
												GLuint &width, GLuint &height, GLuint HDRIFormat)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load Cubemap texture from lump..."));

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadL(IL_TYPE_UNKNOWN, lump, size_l))
		{
			myGlobalGame->mlog.LogError(LString("Invalid Lump Data\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, (GLuint*)&texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int _width = width = ilGetInteger(IL_IMAGE_WIDTH);
		int _height = height = ilGetInteger(IL_IMAGE_HEIGHT);
		int _depth = 0;
		int _elementSize = ilGetInteger(IL_IMAGE_BPP);

		//myLog->LogError(LString("w: %d h: %d", _width, _height));

		if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
			return false;

		GLubyte *data = ilGetData();

		int fWidth = _width / 3;
		int fHeight = _height / 4;

		GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
		GLubyte *ptr;

		/* POSITIVE_X */
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[(((fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		}  

		/* NEGATIVE_X */
		//texID = LEFT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Y */
		//texID = FRONT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Y */
		//texID = BACK;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Z */
		//texID = BOTTOM;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Z */
		//texID = TOP;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			for (int i=0; i<fWidth; i++) {
				memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
				ptr += _elementSize;
			}
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Cube Map Texture was loaded successfully from lump!"));

		return true;
	}

	RX_API bool LoadCrossCubeMapTextureFromLump(GLvoid* lump, GLuint texArray[], GLuint texID,
												bool mipmaps, GLuint size_l,
												GLuint &width, GLuint &height, GLuint HDRIFormat)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load Cubemap texture from lump..."));

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadL(IL_TYPE_UNKNOWN, lump, size_l))
		{
			myGlobalGame->mlog.LogError(LString("Invalid Lump Data\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texArray[texID]);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int _width = width = ilGetInteger(IL_IMAGE_WIDTH);
		int _height = height = ilGetInteger(IL_IMAGE_HEIGHT);
		int _depth = 0;
		int _elementSize = ilGetInteger(IL_IMAGE_BPP);

		//myLog->LogError(LString("w: %d h: %d", _width, _height));

		if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
			return false;

		GLubyte *data = ilGetData();

		int fWidth = _width / 3;
		int fHeight = _height / 4;

		GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
		GLubyte *ptr;

		/* POSITIVE_X */
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[(((fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		}  

		/* NEGATIVE_X */
		//texID = LEFT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Y */
		//texID = FRONT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Y */
		//texID = BACK;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Z */
		//texID = BOTTOM;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Z */
		//texID = TOP;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			for (int i=0; i<fWidth; i++) {
				memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
				ptr += _elementSize;
			}
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Cube Map Texture was loaded successfully from lump!"));

		return true;
	}

	RX_API bool LoadTexture(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps,
							GLuint &width, GLuint &height,
							bool sClampToEdge, bool tClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 2D texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}	

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_2D, texArray[texID]);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
				width = ilGetInteger(IL_IMAGE_WIDTH),
				height = ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), 
				ilGetInteger(IL_IMAGE_TYPE), ilGetData());	

			/*
			int b = ilGetInteger(IL_IMAGE_BPP);
			int a = ilGetInteger(IL_IMAGE_FORMAT);
			int c = 5; 
			*/

		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
				width = ilGetInteger(IL_IMAGE_WIDTH),
				height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE),
				ilGetData());				
		}    	

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("2D Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadTextureFromLump(GLvoid* lump, GLuint texArray[], GLuint texID, bool Mipmaps, GLuint size_l,
									GLuint &width, GLuint &height, bool sClampToEdge, bool tClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 2D texture from lump..."));

		ILuint myImage;
		myImage = GenerateSingleImage();
		ilBindImage(myImage);

		if(IL_FALSE == ilLoadL(IL_TYPE_UNKNOWN, lump, size_l))
		{
			myGlobalGame->mlog.LogError(LString("Invalid Lump Data\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_2D, texArray[texID]);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
				width = ilGetInteger(IL_IMAGE_WIDTH),
				height = ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), 
				ilGetInteger(IL_IMAGE_TYPE), ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (tClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
				width = ilGetInteger(IL_IMAGE_WIDTH),
				height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE),
				ilGetData());				
		}    	

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("2d Texture was loaded successfully from lump!"));

		return true;
	}

	RX_API bool LoadTextureForSkyBox(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_2D, texArray[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
						ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), ilGetData());			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?myGlobalGame->getHDRITextureFormat():ilGetInteger(IL_IMAGE_BPP),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), ilGetData());		
		}    	

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadTexture1D(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps,
							  GLuint &width, bool sClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 1D texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_1D, texArray[texID]);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild1DMipmaps(GL_TEXTURE_1D, ilGetInteger(IL_IMAGE_BPP), width = width = ilGetInteger(IL_IMAGE_WIDTH),
							  ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage1D(GL_TEXTURE_1D, 0, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
						 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());	
		}
	 	
		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("1D Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadTexture1DFromLump(GLvoid* lump, GLuint texArray[], GLuint texID, bool Mipmaps, GLuint size_l,
									  GLuint &width, bool sClampToEdge)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load 1D texture from lump..."));

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadL(IL_TYPE_UNKNOWN, lump, size_l))
		{
			myGlobalGame->mlog.LogError(LString("Invalid Lump Data\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_1D, texArray[texID]);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			gluBuild1DMipmaps(GL_TEXTURE_1D, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
							  ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, (sClampToEdge == true)?GL_CLAMP:GL_REPEAT);

			glTexImage1D(GL_TEXTURE_1D, 0, ilGetInteger(IL_IMAGE_BPP), width = ilGetInteger(IL_IMAGE_WIDTH),
						 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());	
		}
	 	
		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("1D Texture was loaded successfully from lump!"));

		return true;
	}


	RX_API bool LoadTextureKey(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps,
							   GLubyte r, GLubyte g, GLubyte b, GLuint &width, GLuint &height)
	{
		int i, j;
		myGlobalGame->mlog.LogInfo(LString("Attempting to load texture: %s with color key: (%d, %d, %d)", fileName, r, g, b));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		ILubyte* data;

		data = ilGetData();
		int sizeX = width = ilGetInteger(IL_IMAGE_WIDTH);
		int sizeY = height = ilGetInteger(IL_IMAGE_HEIGHT);

		GLubyte* newImage = new GLubyte[sizeX*sizeY*4];

		if(!newImage)
		{
			myGlobalGame->mlog.LogError(LString("Insufficient memory!"));
			return false;
		}

		for(i=0, j=0;i<sizeX*sizeY*3;i+=3,j+=4)
		{
			if(data[i] == r && data[i+1] == g && data[i+2] == b)
			{
				newImage[j+3] = 0;
			} else {
				newImage[j+3] = 255;
			}

			newImage[j]	  = data[i];
			newImage[j+1] = data[i+1];
			newImage[j+2] = data[i+2];
		}

		glGenTextures(1, &texArray[texID]);
		glBindTexture(GL_TEXTURE_2D, texArray[texID]);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, newImage);		
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, newImage);		
		}    
		
		if(newImage)
		{
			delete[] newImage;
		}

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API bool LoadTextureSingle(const char* fileName, GLuint* texID, bool Mipmaps)
	{
		myGlobalGame->mlog.LogInfo(LString("Attempting to load texture: %s", fileName));

		if(!fileName)
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(fileName))
		{
			myGlobalGame->mlog.LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGenTextures(1, texID);
		glBindTexture(GL_TEXTURE_2D, *texID);

		if(Mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
							  ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT),
							  GL_UNSIGNED_BYTE, ilGetData());	
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
						 ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT),
						 GL_UNSIGNED_BYTE, ilGetData());		
		}    	

		DeleteSingleImage(myImage);

		myGlobalGame->mlog.LogSuccessText(LString("Texture %s was loaded successfully!", fileName));

		return true;
	}

	RX_API void setTexture(unsigned int tex)
	{
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
	}

	RX_API void setMultiTexture(unsigned int tex, unsigned int texUNIT)
	{
		switch(texUNIT)
		{
		case 0:
		glActiveTextureARB(GL_TEXTURE0_ARB);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
		break;
		case 1:
		glActiveTextureARB(GL_TEXTURE1_ARB);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
		break;
		case 2:
		glActiveTextureARB(GL_TEXTURE2_ARB);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
		break;
		case 3:
		glActiveTextureARB(GL_TEXTURE3_ARB);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
		break;
		case 4:
		glActiveTextureARB(GL_TEXTURE4_ARB);
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_Textures[tex]);
		break;
		}
	}

	RX_API void FreeTextures()
	{
		glDeleteTextures(MAX_TEX, g_Textures);
		myGlobalGame->mlog.LogSuccessText(LString("All textures in g_Textures used successfully released!"));
	}

	RX_API void DeleteTexture(GLTexture &texture)
	{
		if(glIsTexture(texture))
		{
			glDeleteTextures(1, &texture);
		}
	}

	/* 
	===========================================
		class Texture1D
	===========================================
	*/

	Texture1D::Texture1D():mTextureType(GL_TEXTURE_1D)
	{
		this->mTextureUnit = 0;
		this->mWidth = 16;
		//this->mHeight = 1;
	}

	Texture1D::~Texture1D()
	{
		if(glIsTexture(this->mTexture))
		{
			myGlobalGame->mlog.LogSuccessText(LString("Texture \"%s\" was released!", this->fileName));
			glDeleteTextures(1, &this->mTexture);
		}
	}

	GLuint Texture1D::getTextureUnit(void)
	{
		return this->mTextureUnit;
	}

	const char* Texture1D::getFileName(void) const
	{
		return &this->fileName[0];
	}

	void Texture1D::bindTextureToTextureUnit(GLuint unit)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + unit);
		//glEnable(this->mTextureType);
		glBindTexture(this->mTextureType, this->mTexture);
		this->mTextureUnit = unit;
	}

	inline GLuint Texture1D::getTexture(void)
	{
		return this->mTexture;
	}

	inline GLuint Texture1D::getTextureType(void)
	{
		return this->mTextureType;
	}

	inline GLuint* Texture1D::getPointerToTexture(void)
	{
		return &this->mTexture;
	}

	void Texture1D::operator =(Texture1D &pCTexture)
	{
		this->mTexture = pCTexture.getTexture();
	}

	bool Texture1D::CreateEmptyTexture(int width, int height, int internalFormat/*Can be channels*/,
										int format, bool mipMapExt, int type,
										int minFilter, int magFilter,
										bool sClampToEdge, bool tClampToEdge)
	{
		// Create a pointer to store the blank image data
		//GLuint *pTexture = NULL;	

		myGlobalGame->mlog.LogInfo(LString("Creating empty 1D texture..."));

		// Allocate and init memory for the image array and point to it from pTexture
		/*
		pTexture = new GLuint [size * channels];

		if(!pTexture)
		{
			myGlobalGame->mlog.LogError(LString("An empty 1D texture failed to be created"));
			return false;
		}
		*/

		this->mWidth = width;	

		//memset(pTexture, 0, size * channels * sizeof(GLuint));	

		// Register the texture with OpenGL and bind it to the texture ID
		glGenTextures(1, &this->mTexture);								
		glBindTexture(this->mTextureType, this->mTexture);					
		
		// Create the texture and store it on the video card
		glTexImage1D(this->mTextureType, 0, internalFormat, this->mWidth, 0, format, GL_UNSIGNED_BYTE, 0);						
		
		// Set the texture quality
		glTexParameterf(this->mTextureType,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(this->mTextureType,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameterf(this->mTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		if(mipMapExt)
		{
			glGenerateMipmapEXT(this->mTextureType);
		}

		// Since we stored the texture space with OpenGL, we can delete the image data
		//delete [] pTexture;	

		myGlobalGame->mlog.LogSuccessText(LString("An empty 1D texture was successfully created!"));

		return true;
	}

	bool Texture1D::LoadTextureImage(const char* textureFileName, bool mipmaps,
									  bool sClampToEdge, bool tClampToEdge)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		strcpy(this->fileName, textureFileName);
		if(LoadTexture1D(textureFileName, &this->mTexture, 0, mipmaps, this->mWidth, sClampToEdge))
		{
			return true;
		}	

		return false;
	}

	void Texture1D::SetTextureClamping(bool sClampToEdge, bool tClampToEdge)
	{
		glBindTexture(this->mTextureType, this->mTexture);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	}

	bool Texture1D::LoadTextureImageFromLump(GLvoid* lump, GLuint size, bool mipmaps,
											  bool sClampToEdge, bool tClampToEdge)
	{
		if(this->mTextureType = GL_TEXTURE_CUBE_MAP_ARB)
		{
			myGlobalGame->mlog.LogError(LString("Use LoadTextureImageFromLumpCube for Cubemap textures"));
			return false;
		}

		if(LoadTexture1DFromLump(lump, &this->mTexture, 0, mipmaps, size, this->mWidth, sClampToEdge))
		{
			return true;
		}	

		return false;
	}

	bool Texture1D::LoadTextureImage_MipMaps(const char* textureFileName)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		strcpy(this->fileName, textureFileName);
		if(LoadTexture1D(textureFileName, &this->mTexture, 0, true, this->mWidth, true))
		{
			return true;
		}	

		return false;
	}


	void Texture1D::RenderCurrentViewportToTexture(void)
	{
		glBindTexture(GL_TEXTURE_1D, this->mTexture);
		glCopyTexSubImage1D(GL_TEXTURE_1D, 0, 0, 0, 0, this->mWidth);
	}

	void Texture1D::bindTexture(void)
	{
		glBindTexture(this->mTextureType, this->mTexture);
	}

	bool Texture1D::saveImageToDisk(const char* oFileName, int mipLevel, GLuint face)
	{
		ILuint myImage;
		myImage = GenerateSingleImage();

		if(!ilIsImage(myImage))
		{
			myGlobalGame->mlog.LogError(LString("There was an error saving %s (@Stage_1)", oFileName));

			return false;
		}

		ilBindImage(myImage); 

		int width, height;

		this->bindTextureToTextureUnit(0);

				
		if(this->mTextureType != GL_TEXTURE_CUBE_MAP_ARB)
		{
			if(face > 0)
			{
				face = 0;
			}
		} else {
			if(face < 1)
			{
				 face = 1;
			} else if(face > 6)
			{
				face = 6;
			}

			face++;
		}

		
		glGetTexLevelParameteriv(this->mTextureType + face, mipLevel, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(this->mTextureType + face, mipLevel, GL_TEXTURE_HEIGHT, &height);	
		

		if(IL_FALSE == ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL))
		{
			myGlobalGame->mlog.LogError(LString("There was an error saving (@Stage_2) %s\nErrorStrings are:\n", oFileName));

			ILenum Error;			

			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		glGetTexImage(this->mTextureType + face, mipLevel, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		
		if(IL_FALSE == ilSaveImage(oFileName))
		{
			myGlobalGame->mlog.LogError(LString("There was an error saving (@Stage_3) %s\nErrorStrings are:\n", oFileName));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}	

		DeleteSingleImage(myImage);

		return true;
	}

	void Texture1D::Free(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("1D Texture \"%s\" was released!", this->fileName));
		}
	}

	void Texture1D::ReleaseTextureOnly(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("A 1D texture was released"));
		}
	}

	GLuint Texture1D::getWidth(void) const
	{
		return this->mWidth;
	}

	Texture1D::Texture1D(GLuint type)
	{
		this->mTextureType = type;
	}

	/*
	=========================================================
		class Texture3D inherits from Texture1D
	=========================================================
	*/

	Texture3D::Texture3D():Texture1D(GL_TEXTURE_3D)
	{
	}

	Texture3D::~Texture3D()
	{	
		if(glIsTexture(this->mTexture))
		{
			myGlobalGame->mlog.LogSuccessText(LString("3D Texture \"%s\" was released!", this->fileName));
			glDeleteTextures(1, &this->mTexture);
		}	
	}

	void Texture3D::Free(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("3D Texture \"%s\" was released!", this->fileName));
		}
	}

	bool Texture3D::PrepareJitterMap(GLint size, GLint samples_u, GLint samples_v)
	{
		myGlobalGame->mlog.LogInfo(LString("Preparing jitter map for a 3D texture..."));

		if(!glTexImage3D)
		{
			myGlobalGame->mlog.LogError(LString("Texture3D::PrepareJitterMap failed!, please call CoreEngine::Initialize3DTextures first!"));
			return false;
		}

		glEnable(GL_TEXTURE_3D);

		glGenTextures(1, &this->mTexture);								
		glBindTexture(this->mTextureType, this->mTexture);	

		glTexParameteri(this->mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(this->mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_R, GL_REPEAT);

		signed char * data = new signed char[size * size * samples_u * samples_v * 4 / 2];	

		if(!data)
		{
			myGlobalGame->mlog.LogError(LString("There was an error creating a jitter map for a 3D texture"));
			return false;
		}

		for (int i = 0; i<size; i++) {
			for (int j = 0; j<size; j++) {
				float rot_offset = ((float)rand() / RAND_MAX - 1) * 2 * 3.1415926f;
				for (int k = 0; k<samples_u*samples_v/2; k++) {

					int x, y;
					float v[4];

					x = k % (samples_u / 2);
					y = (samples_v - 1) - k / (samples_u / 2);

					// generate points on a regular samples_u x samples_v rectangular grid
					v[0] = (float)(x * 2 + 0.5f) / samples_u;
					v[1] = (float)(y + 0.5f) / samples_v;
					v[2] = (float)(x * 2 + 1 + 0.5f) / samples_u;
					v[3] = v[1];
					
					// jitter position
					v[0] += ((float)rand() * 2 / RAND_MAX - 1) * (0.5f / samples_u);
					v[1] += ((float)rand() * 2 / RAND_MAX - 1) * (0.5f / samples_v);
					v[2] += ((float)rand() * 2 / RAND_MAX - 1) * (0.5f / samples_u);
					v[3] += ((float)rand() * 2 / RAND_MAX - 1) * (0.5f / samples_v);

					// warp to disk
					float d[4];
					d[0] = 1.0f / Math::rsqrt(v[1]) * Math::cos(2 * 3.1415926f * v[0]);
					d[1] = 1.0f / Math::rsqrt(v[1]) * Math::sin(2 * 3.1415926f * v[0]);
					d[2] = 1.0f / Math::rsqrt(v[3]) * Math::cos(2 * 3.1415926f * v[2]);
					d[3] = 1.0f / Math::rsqrt(v[3]) * Math::sin(2 * 3.1415926f * v[2]);

					data[(k * size * size + j * size + i) * 4 + 0] = (signed char)(d[0] * 127);
					data[(k * size * size + j * size + i) * 4 + 1] = (signed char)(d[1] * 127);
					data[(k * size * size + j * size + i) * 4 + 2] = (signed char)(d[2] * 127);
					data[(k * size * size + j * size + i) * 4 + 3] = (signed char)(d[3] * 127);				
				}
			}
		}	

		glTexImage3D(GL_TEXTURE_3D, 0, GL_SIGNED_RGBA_NV, size, size, samples_u * samples_v / 2, 0, GL_RGBA, GL_BYTE, data);

		delete [] data;
		data = 0;

		glDisable(GL_TEXTURE_3D);

		memset(this->fileName, 0, sizeof(char)*256);
		sprintf(this->fileName, "NO_NAME: Dynamically Created");

		myGlobalGame->mlog.LogSuccessText(LString("Jitter map successfully created for a 3D texture!"));

		return true;
	}


	/*
	=========================================================
		class Texture2D inherits from Texture1D
	=========================================================
	*/

	Texture2D::Texture2D():Texture1D(GL_TEXTURE_2D)
	{
	}

	Texture2D::~Texture2D()
	{	
		if(glIsTexture(this->mTexture))
		{
			myGlobalGame->mlog.LogSuccessText(LString("2D Texture \"%s\" was released!", this->fileName));
			glDeleteTextures(1, &this->mTexture);
		}	
	}

	void Texture2D::Free(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("2D Texture \"%s\" was released!", this->fileName));
		}
	}

	bool Texture2D::LoadTextureImage(const char* textureFileName, bool mipmaps,
									  bool sClampToEdge, bool tClampToEdge)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		strcpy(this->fileName, textureFileName);
		if(LoadTexture(textureFileName, &this->mTexture, 0, mipmaps, this->mWidth, this->mHeight,
					   sClampToEdge, tClampToEdge))
		{
			return true;
		}	

		return false;
	}

	void Texture2D::SetBorderColor(float r, float g, float b, float a)
	{
		GLfloat borderColor[4] = {r, g, b, a};
		glBindTexture(this->mTextureType, this->mTexture);
		glTexParameterfv(this->mTextureType, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	bool Texture2D::LoadTextureImageFromLump(GLvoid* lump, GLuint size, bool mipmaps,
											  bool sClampToEdge, bool tClampToEdge)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		sprintf(this->fileName, "Loaded from Lump Data");
		if(LoadTextureFromLump(lump, &this->mTexture, 0, mipmaps, size, this->mWidth, this->mHeight,
							   sClampToEdge, tClampToEdge))
		{
			return true;
		}	

		return false;
	}

	bool Texture2D::CreateEmptyTexture(int width, int height, int internalFormat, int format, bool mipMapExt, int type,
										int minFilter, int magFilter, bool sClampToEdge, bool tClampToEdge, int border)
	{
		// Create a pointer to store the blank image data
		//GLuint *pTexture = NULL;		

		myGlobalGame->mlog.LogInfo(LString("Creating empty 2D texture..."));

		// Allocate and init memory for the image array and point to it from pTexture

		/*
		pTexture = new GLuint [size * size * channels];

		if(!pTexture)
		{
			myGlobalGame->mlog.LogError(LString("An empty 2D texture failed to be created"));
			return false;
		}
		*/

		this->mWidth = width;
		this->mHeight = height;

		//memset(pTexture, 0, size * size * channels * sizeof(GLuint));	

		// Register the texture with OpenGL and bind it to the texture ID
		glGenTextures(1, &this->mTexture);								
		glBindTexture(this->mTextureType, this->mTexture);			

		if(format == GL_DEPTH_COMPONENT)
		{
			//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LESS);
		}
		
		// Set the texture quality
		glTexParameteri(this->mTextureType,GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(this->mTextureType,GL_TEXTURE_MAG_FILTER, magFilter);

		if(border == 1)
		{
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true)? GL_CLAMP_TO_BORDER : GL_REPEAT);//GL_CLAMP_TO_EDGE
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, (sClampToEdge == true)? GL_CLAMP_TO_BORDER : GL_REPEAT);
		} else {
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true)? GL_CLAMP_TO_EDGE : GL_REPEAT);//GL_CLAMP_TO_EDGE
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, (sClampToEdge == true)? GL_CLAMP_TO_EDGE : GL_REPEAT);
		}

		// Create the texture and store it on the video card
		glTexImage2D(this->mTextureType, 0, internalFormat, this->mWidth, this->mHeight, 0, format, type, 0/*pTexture*/);

		if(mipMapExt)
		{
			glGenerateMipmapEXT(this->mTextureType);
		}

		// Since we stored the texture space with OpenGL, we can delete the image data
		//delete [] pTexture;	

		if(format == GL_DEPTH_COMPONENT)
		{
			myGlobalGame->mlog.LogSuccessText(LString("An empty 2D Depth texture was successfully created!"));
		} else {
			myGlobalGame->mlog.LogSuccessText(LString("An empty 2D texture was successfully created!"));
		}

		return true;
	}

	void Texture2D::SetTextureClamping(bool sClampToEdge, bool tClampToEdge)
	{
		glBindTexture(this->mTextureType, this->mTexture);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, (tClampToEdge == true) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	}

	bool Texture2D::LoadTextureImage_MipMaps(const char* textureFileName)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		strcpy(this->fileName, textureFileName);
		if(LoadTexture(fileName, &this->mTexture, 0, true, this->mWidth, this->mHeight, true, true))
		{
			return true;
		}	

		return false;
	}

	void Texture2D::RenderCurrentViewportToTexture(void)
	{	
		glBindTexture(GL_TEXTURE_2D, this->mTexture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, this->mWidth, this->mHeight);
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, this->mWidth, this->mHeight, 0);
	}

	GLuint Texture2D::getHeight(void) const
	{
		return this->mHeight;
	}

	/*
	================================================================================================
		class TextureCube inherits from Texture1D, doesn't load cube map textures... see 
		class CSkyBox for skyboxes and cube map images
	================================================================================================
	*/

	TextureCube::TextureCube():Texture1D(GL_TEXTURE_CUBE_MAP_ARB)
	{
	}

	TextureCube::~TextureCube()
	{	
		if(glIsTexture(this->mTexture))
		{
			myGlobalGame->mlog.LogSuccessText(LString("Cubemap Texture \"%s\" was released!", this->fileName));
			glDeleteTextures(1, &this->mTexture);
		}	
	}

	void TextureCube::Free(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("Cubemap Texture \"%s\" was released!", this->fileName));
		}
	}

	bool TextureCube::CreateEmptyTexture(int width, int height, int internalFormat, int format, bool mipMapExt, int type,
										int minFilter, int magFilter, bool sClampToEdge, bool tClampToEdge,
										int border)
	{
		myGlobalGame->mlog.LogInfo(LString("Creating empty CUBEMAP texture..."));

		this->mWidth = width;
		this->mHeight = height;

		if(!this->mTextureType)
			this->mTextureType = GL_TEXTURE_CUBE_MAP_ARB;

		//memset(pTexture, 0, size * size * channels * sizeof(GLuint));	

		// Register the texture with OpenGL and bind it to the texture ID
		glGenTextures(1, &this->mTexture);								
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, this->mTexture);		
		
		if(format == GL_DEPTH_COMPONENT)
		{
			//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		}
		
		// Set the texture quality
		glTexParameteri(this->mTextureType,GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(this->mTextureType,GL_TEXTURE_MAG_FILTER, magFilter);

		if(border == 1)
		{
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true)? GL_CLAMP_TO_BORDER : GL_REPEAT);
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, (tClampToEdge == true)? GL_CLAMP_TO_BORDER : GL_REPEAT);
		} else {
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_S, (sClampToEdge == true)? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(this->mTextureType, GL_TEXTURE_WRAP_T, (tClampToEdge == true)? GL_CLAMP_TO_EDGE : GL_REPEAT);
		}

		// Create the texture and store it on the video card

		for(int i = 0; i < 6; i++)
		{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i,
									0, 
									internalFormat,
									this->mWidth, 
									this->mHeight,
									0,
									format, 
									type, 
									0);
		}
		//glTexImage2D(this->mTextureType, 0, internalFormat, this->mWidth, this->mHeight, 0, format, type, 0/*pTexture*/);

		if(mipMapExt)
		{
			glGenerateMipmapEXT(this->mTextureType);
		}

		// Since we stored the texture space with OpenGL, we can delete the image data
		//delete [] pTexture;	
		memset(this->fileName, 0, sizeof(char)*256);
		sprintf(this->fileName, "NO_NAME: Empty & Dynamically Created");

		if(format == GL_DEPTH_COMPONENT)
		{
			myGlobalGame->mlog.LogSuccessText(LString("An empty DEPTH CUBEMAP texture was successfully created!"));
		} else {
			myGlobalGame->mlog.LogSuccessText(LString("An empty CUBEMAP texture was successfully created!"));
		}

		return true;
	}

	void TextureCube::RenderCurrentViewportToTexture(int cubeFace)
	{
		if(cubeFace > 5 || cubeFace < 0)
		{
			myGlobalGame->mlog.LogError(LString("cubeFace (%d) parameter was invalid", cubeFace));
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, this->mTexture);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + cubeFace, 0, 0, 0, 0, 0, this->mWidth, this->mHeight);
	}

	bool TextureCube::LoadTextureImage(const char* textureFileName, bool mipmaps, GLuint HDRIFormat)
	{
		memset(this->fileName, 0, sizeof(char)*256);
		strcpy(this->fileName, textureFileName);
		if(LoadCrossCubeMapTexture(textureFileName, &this->mTexture, 0, mipmaps, this->mWidth, this->mHeight,
		   HDRIFormat))
		{
			return true;
		}	

		return false;
	}

	bool TextureCube::LoadTextureImageFromLumpCube(GLvoid* lump, GLuint size, bool mipmaps, GLuint HDRIFormat)
	{
		if(LoadCrossCubeMapTextureFromLump(lump, &this->mTexture, 0, mipmaps, size, this->mWidth,
											this->mHeight, HDRIFormat))
		{
			return true;
		}	

		return false;
	}

	bool TextureCube::GenerateNormalizationCubeMap(GLuint size)
	{
		glGenTextures(1, &this->mTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->mTexture);

		GLubyte* data = new GLubyte[size*size*3];

		float offset = 0.5f;
		float halfSize = size * 0.5f;
		Math::Vector3 tempDirVec;
		GLuint bytePtr = 0;
		GLuint i, j;

		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = halfSize;
				tempDirVec.y = (j+offset-halfSize);
				tempDirVec.z = -(i+offset-halfSize);
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		bytePtr = 0;
		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = -halfSize;
				tempDirVec.y = (j+offset-halfSize);
				tempDirVec.z = (i+offset-halfSize);
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		bytePtr = 0;
		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = i+offset-halfSize;
				tempDirVec.y = -halfSize;
				tempDirVec.z = j+offset-halfSize;
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		bytePtr = 0;
		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = i+offset-halfSize;
				tempDirVec.y = halfSize;
				tempDirVec.z = -(j+offset-halfSize);
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		bytePtr = 0;
		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = i+offset-halfSize;
				tempDirVec.y = (j+offset-halfSize);
				tempDirVec.z = halfSize;
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		bytePtr = 0;
		for(j=0; j<size; j++)
		{
			for(i=0; i<size; i++)
			{
				tempDirVec.x = -(i+offset-halfSize);
				tempDirVec.y = (j+offset-halfSize);
				tempDirVec.z = -halfSize;
				tempDirVec = RangeCompress(tempDirVec);

				data[bytePtr] = (GLubyte)(tempDirVec.x * 255.0f);
				data[bytePtr+1] = (GLubyte)(tempDirVec.y * 255.0f);
				data[bytePtr+2] = (GLubyte)(tempDirVec.z * 255.0f);

				bytePtr+=3;
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete [] data;

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		memset(this->fileName, 0, sizeof(char)*256);
		sprintf(this->fileName, "NO_NAME: Dynamically Created normalization cubemap");

		return true;
	}

	/*
	===========================================
		General TARGET Texture class
	===========================================
	*/

	TextureMutable::TextureMutable()
	{

	}

	TextureMutable::~TextureMutable()
	{
		if(glIsTexture(this->mTexture))
		{
			myGlobalGame->mlog.LogSuccessText(LString("A General Texture \"%s\" was released!", this->fileName));
			glDeleteTextures(1, &this->mTexture);
		}
	}

	void TextureMutable::setTarget(GLenum target)
	{
		this->mTextureType = target;
	}

	void TextureMutable::Free(void)
	{
		if(glIsTexture(this->mTexture))
		{		
			glDeleteTextures(1, &this->mTexture);
			myGlobalGame->mlog.LogSuccessText(LString("A General Texture \"%s\" was released!", this->fileName));
		}
	}

	/*
	===================================================================================
		ILUT Helper functions.. these are also methods in the OpenGL render engine
	===================================================================================
	*/

	RX_API void DeleteSingleImage(ILuint ImageName)
	{
		ilDeleteImages(1, &ImageName); 
		return;
	}

	RX_API ILuint GenerateSingleImage(void) 
	{
		ILuint ImageName; // The image name to return.
		ilGenImages(1, &ImageName); // Grab a new image name.
		return ImageName; // Go wild with the return value.
	} 
}