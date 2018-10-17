/*
	Description: Skybox class and Lens flare object class

	classes contained are:

	class Skybox
	class LensFlareTexObj
*/


#include "SkyBox.h"
#include "CoreEngine.h"
#include "Console.h"

namespace DifferentialArts
{

	Skybox::~Skybox()
	{
		//this->Free();
	}

	Skybox::Skybox()
	{
		this->inUse = false;
		this->size = Math::Vector3(1,1,1);
	}

	void Skybox::Free()
	{
		if(!this->inUse)
			return;
		
		glDeleteLists(this->drawlist, 1);
		glDeleteLists(this->drawlist2, 1);
		this->inUse = false;
		gConsole->LogSuccessText(LString("Skybox succesfully released!"));
	}

	bool Skybox::CreateSkyboxFromCrossCubeMap(const char* skyBoxCrossTextureName, const Math::Vector3 &size, bool mipmaps, GLuint HDRIFormat)
	{
		gConsole->LogInfo(LString("Skybox from Cross Cube map Image init started..."));

		this->size = size;

		gConsole->LogInfo(LString("Cross Cube map image Skybox: %s", skyBoxCrossTextureName));

		gConsole->LogInfo(LString("Attempting to load texture: %s", skyBoxCrossTextureName));

		if(!skyBoxCrossTextureName)
		{
			gConsole->LogError(LString("Invalid File Name"));
			return false;
		}

		ILuint myImage;

		myImage = GenerateSingleImage();

		ilBindImage(myImage);

		if(IL_FALSE == ilLoadImage(skyBoxCrossTextureName))
		{
			gConsole->LogError(LString("Invalid File Data or missing file\nErrorStrings are:\n"));

			ILenum Error;
			while ((Error = ilGetError()) != IL_NO_ERROR)
			{
				gConsole->LogError(LString("%d: %s\n", Error, iluErrorString(Error)));
			}

			return false;
		}

		int _width = ilGetInteger(IL_IMAGE_WIDTH);
		int _height = ilGetInteger(IL_IMAGE_HEIGHT);
		int _depth = 0;
		int _elementSize = ilGetInteger(IL_IMAGE_BPP);

		//gConsole->LogError(LString("w: %d h: %d", _width, _height));

		if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
			return false;

		GLubyte *data = ilGetData();

		int fWidth = _width / 3;
		int fHeight = _height / 4;
		GLuint texID;

		GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
		GLubyte *ptr;

		/* POSITIVE_X */
		texID = RIGHT;
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[(((fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		}  

		/* NEGATIVE_X */
		texID = LEFT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Y */
		texID = FRONT;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Y */
		texID = BACK;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* POSITIVE_Z */
		texID = BOTTOM;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
			ptr += fWidth*_elementSize;
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		/* NEGATIVE_Z */
		texID = TOP;

		face = new GLubyte[ fWidth * fHeight * _elementSize];
		ptr = face;
		for (int j=0; j<fHeight; j++) {
			for (int i=0; i<fWidth; i++) {
				memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
				ptr += _elementSize;
			}
		}

		glGenTextures(1, &g_Textures[texID]);
		glBindTexture(GL_TEXTURE_2D, g_Textures[texID]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D,
						(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
						fWidth, fHeight, ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_TYPE), face);			
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0,
				(ilGetInteger(IL_IMAGE_TYPE)==GL_FLOAT)?HDRIFormat:_elementSize,
				fWidth, fHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_TYPE), face);		
		} 

		DeleteSingleImage(myImage);

		/* ===========BREAK================ */

		/* Create skybox display list */
		this->drawlist = glGenLists(1);
		glNewList(this->drawlist, GL_COMPILE);	
		glPushAttrib(GL_ENABLE_BIT);

		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);		
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);		

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, size.y, 0); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0,	size.z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	size.y, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0,	0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, size.z); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	0);	
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	size.y,	size.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0,0);		
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y,	size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x,  size.y,	0);
		glEnd();
		glPopAttrib();
		glEndList();	

		/* End of skybox display list */

		/* Create skybox display list FLIPPED */
		this->drawlist2 = glGenLists(1);
		glNewList(this->drawlist2, GL_COMPILE);	
		glPushAttrib(GL_ENABLE_BIT);

		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);		
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);		

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, size.y, 0); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0,	size.z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	size.y, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0,	0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, size.z); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	0);	
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	size.y,	size.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0,0);		
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y,	size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x,  size.y,	0);
		glEnd();
		glPopAttrib();
		glEndList();	

		/* End of skybox display list */

		gConsole->LogSuccessText(LString("Skybox '%s' succesfully created!", skyBoxCrossTextureName));
		this->inUse = true;

		return true;
	}

	bool Skybox::CreateSkybox(char* skyBoxTexFolderName, const Math::Vector3 &size, bool mipmaps)
	{
		char fileName[256];
		char formatFileName[256];
		char format[256];

		strcpy(formatFileName, skyBoxTexFolderName);
		strcat(formatFileName, "format.format");

		gConsole->LogInfo(LString("Skybox init started..."));

		std::ifstream fin(formatFileName);

		if(fin.fail())
		{
			gConsole->LogError(LString("Failed to read format file in skybox %s", skyBoxTexFolderName));
			return false;
		}

		fin>>format;

		fin.close();

		this->size = size;

		gConsole->LogInfo(LString("Skybox: %s", skyBoxTexFolderName));

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Back");
		strcat(fileName, format);	
		LoadTextureForSkyBox(fileName, g_Textures, BACK, mipmaps);

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Front");
		strcat(fileName, format);
		LoadTextureForSkyBox(fileName, g_Textures, FRONT, mipmaps);

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Bottom");
		strcat(fileName, format);
		LoadTextureForSkyBox(fileName, g_Textures, BOTTOM, mipmaps);

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Top");
		strcat(fileName, format);
		LoadTextureForSkyBox(fileName, g_Textures, TOP, mipmaps);

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Left");
		strcat(fileName, format);
		LoadTextureForSkyBox(fileName, g_Textures, LEFT, mipmaps);

		strcpy(fileName, skyBoxTexFolderName);
		strcat(fileName, "Right");
		strcat(fileName, format);
		LoadTextureForSkyBox(fileName, g_Textures, RIGHT, mipmaps);

		this->setPosition(Math::Vector3(0, 0, 0));

		/* Create skybox display list */
		this->drawlist = glGenLists(1);
		glNewList(this->drawlist, GL_COMPILE);	
		//glPushAttrib(GL_ENABLE_BIT);

		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_LIGHTING);		
		//glEnable(GL_TEXTURE_2D);
		//glColor4f(1, 1, 1, 1);	
		//glDisable(GL_CULL_FACE);

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, size.y, 0); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/
		
		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	0,	size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y, size.z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	0,	0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, 0, size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y,	0);	
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	size.y,	size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	0,0);		
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, 0, size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, size.y,	size.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x,  size.y,	0);
		glEnd();
		//glPopAttrib();
		glEndList();	

		/* End of skybox display list */

		/* Create skybox display list FLIPPED */
		this->drawlist2 = glGenLists(1);
		glNewList(this->drawlist2, GL_COMPILE);	
		//glPushAttrib(GL_ENABLE_BIT);

		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_LIGHTING);		
		//glEnable(GL_TEXTURE_2D);
		//glColor4f(1, 1, 1, 1);		

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y, 0); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, size.y, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/
		
		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0,	size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y, size.z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(0.0f, 1.0f);  glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0,	0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, 0, size.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, size.y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x, size.y, size.z); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0,	size.y,	0);	
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0,	size.y,	size.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0,	0, size.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0,	0,0);		
			
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glTexCoord2f(0.0f, 1.0f); glVertex3f( size.x, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( size.x, 0, size.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( size.x, size.y,	size.z); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f( size.x,  size.y,	0);
		glEnd();
		//glPopAttrib();
		glEndList();	
		//end of flipped list
		/* End of skybox display list */

		/* Skybox shadow display lists */

		this->drawlistShadow = glGenLists(1);
		glNewList(this->drawlistShadow, GL_COMPILE);	

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glVertex3f( size.x, 0, 0);
			glVertex3f( size.x, size.y, 0); 
			glVertex3f(0, size.y, 0);
			glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);
		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glVertex3f(0,	0,	size.z);
			glVertex3f(0,	size.y, size.z);
			glVertex3f( size.x, size.y, size.z); 
			glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glVertex3f(0,	0,	0);
			glVertex3f(0,	0, size.z);
			glVertex3f( size.x, 0, size.z); 
			glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glVertex3f( size.x, size.y, 0);
			glVertex3f( size.x, size.y, size.z); 
			glVertex3f(0,	size.y,	size.z);
			glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glVertex3f(0,	size.y,	0);	
			glVertex3f(0,	size.y,	size.z); 
			glVertex3f(0,	0, size.z);
			glVertex3f(0,	0,0);		
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glVertex3f( size.x, 0, 0);
			glVertex3f( size.x, 0, size.z);
			glVertex3f( size.x, size.y,	size.z); 
			glVertex3f( size.x,  size.y,	0);
		glEnd();
		//glPopAttrib();
		glEndList();	

		/* End of skybox display list shadow */
		/* Create skybox display list FLIPPED */
		this->drawlistShadow2 = glGenLists(1);
		glNewList(this->drawlistShadow2, GL_COMPILE);	

		// Bind the BACK texture of the sky map to the BACK side of the cube
		glBindTexture(GL_TEXTURE_2D, g_Textures[BACK]);

		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the BACK Side
			glVertex3f( size.x, 0, 0);
			glVertex3f( size.x, size.y, 0); 
			glVertex3f(0, size.y, 0);
			glVertex3f(0,	0,	0);
			
		glEnd();

		// Bind the FRONT texture of the sky map to the FRONT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[FRONT]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);	
		
			// Assign the texture coordinates and vertices for the FRONT Side
			glVertex3f(0,	0,	size.z);
			glVertex3f(0,	size.y, size.z);
			glVertex3f( size.x, size.y, size.z); 
			glVertex3f( size.x, 0, size.z);
		glEnd();	

		// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[TOP]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
		
			// Assign the texture coordinates and vertices for the BOTTOM Side
			glVertex3f(0,	0,	0);
			glVertex3f(0,	0, size.z);
			glVertex3f( size.x, 0, size.z); 
			glVertex3f( size.x, 0,0);
		glEnd();

		// Bind the TOP texture of the sky map to the TOP side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[BOTTOM]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the TOP Side
			glVertex3f( size.x, size.y, 0);
			glVertex3f( size.x, size.y, size.z); 
			glVertex3f(0,	size.y,	size.z);
			glVertex3f(0,	size.y,	0);
			
		glEnd();

		// Bind the LEFT texture of the sky map to the LEFT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[LEFT]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		
			
			// Assign the texture coordinates and vertices for the LEFT Side
			glVertex3f(0,	size.y,	0);	
			glVertex3f(0,	size.y,	size.z); 
			glVertex3f(0,	0, size.z);
			glVertex3f(0,	0,0);		
			
		glEnd();

		// Bind the RIGHT texture of the sky map to the RIGHT side of the box
		glBindTexture(GL_TEXTURE_2D, g_Textures[RIGHT]);

		// Start drawing the side as a QUAD
		glBegin(GL_QUADS);		

			// Assign the texture coordinates and vertices for the RIGHT Side
			glVertex3f( size.x, 0, 0);
			glVertex3f( size.x, 0, size.z);
			glVertex3f( size.x, size.y,	size.z); 
			glVertex3f( size.x,  size.y,	0);
		glEnd();
		//glPopAttrib();
		glEndList();	
		//end of flipped shadow list

		gConsole->LogSuccessText(LString("Skybox '%s' succesfully created!", skyBoxTexFolderName));
		this->inUse = true;
		return true;
	}

	void Skybox::setPosition(const Math::Vector3 &pos)
	{
		mv_pos = Math::Vector3((pos.x - size.x)  * 0.5f, (pos.y - size.y) * 0.5f, (pos.z - size.z) * 0.5f);
	}

	void Skybox::Render(void)
	{
		if(!inUse)
			return;	

		//glEnable(GL_CULL_FACE);
		glPushMatrix();
			glTranslatef(mv_pos.x, mv_pos.y, mv_pos.z);		
			glCallList(this->drawlist);
		glPopMatrix();
	}

	void Skybox::RenderFlip(void)
	{
		if(!inUse)
			return;	

		glPushMatrix();		
			glTranslatef(mv_pos.x, mv_pos.y, mv_pos.z);	
			glCallList(this->drawlist2);
		glPopMatrix();
	}

	void Skybox::RenderShadow(void)
	{
		if(!inUse)
			return;	

		//glEnable(GL_CULL_FACE);
		glPushMatrix();
			glTranslatef(mv_pos.x, mv_pos.y, mv_pos.z);		
			glCallList(this->drawlistShadow);
		glPopMatrix();
	}

	void Skybox::RenderFlipShadow(void)
	{
		if(!inUse)
			return;	

		glPushMatrix();		
			glTranslatef(mv_pos.x, mv_pos.y, mv_pos.z);	
			glCallList(this->drawlistShadow2);
		glPopMatrix();
	}

	/*
	======================================================================
		LENS FLARE TEXTURE OBJECT
	======================================================================
	*/

	LensFlareTexObj::LensFlareTexObj(void)
	{
		this->pos = Math::Vector3(0, 0, 0);
		bigGlowTexture = 0;
		starTexture	   = 0;
		glowTexture	   = 0;
		haloTexture	   = 0;
	}

	LensFlareTexObj::LensFlareTexObj(char* bigGlow, char* star, char* glow, char* halo,
									 float srcScale, float scale, float maxDistance, bool infinite,
									 float sunLumino, float glowSunLumino)
	{
		this->pos = Math::Vector3(0, 0, 0);

		GLuint width, height;

		gConsole->LogInfo(LString("Lens flare init started..."));

		strcpy(bigGlowAddress, bigGlow);
		strcpy(starAddress, star);
		strcpy(glowAddress, glow);
		strcpy(haloAddress, halo);

		LoadTexture(bigGlow, &bigGlowTexture, bigGlowTexture, true, width, height, true, true);
		LoadTexture(star, &starTexture, starTexture, true, width, height, true, true);
		LoadTexture(glow, &glowTexture, glowTexture, true, width, height, true, true);
		LoadTexture(halo, &haloTexture, haloTexture, true, width, height, true, true);

		this->scale = scale;
		this->srcScale = srcScale;
		this->maxDistance = maxDistance;
		this->infinite = infinite;
		this->sunLumino = sunLumino;
		this->glowSunLumino = glowSunLumino;

		this->touchedLumino = sunLumino*glowSunLumino;

		gConsole->LogSuccessText(LString("Lens flare object succesfully created!"));
	}

	void LensFlareTexObj::Load(char* bigGlow, char* star, char* glow, char* halo,
							   float srcScale, float scale, float maxDistance, bool infinite,
							   float sunLumino, float glowSunLumino)
	{
		GLuint width, height;

		gConsole->LogInfo(LString("Lens flare init started..."));

		strcpy(bigGlowAddress, bigGlow);
		strcpy(starAddress, star);
		strcpy(glowAddress, glow);
		strcpy(haloAddress, halo);

		LoadTexture(bigGlow, &bigGlowTexture, bigGlowTexture, true, width, height, true, true);
		LoadTexture(star, &starTexture, starTexture, true, width, height, true, true);
		LoadTexture(glow, &glowTexture, glowTexture, true, width, height, true, true);
		LoadTexture(halo, &haloTexture, haloTexture, true, width, height, true, true);

		this->scale = scale;
		this->srcScale = srcScale;
		this->maxDistance = maxDistance;
		this->infinite = infinite;
		this->sunLumino = sunLumino;
		this->glowSunLumino = glowSunLumino;

		this->touchedLumino = sunLumino*glowSunLumino;

		gConsole->LogSuccessText(LString("Lens flare object succesfully created!"));
	}

	LensFlareTexObj::~LensFlareTexObj(void)
	{
		this->Free();
	}

	void LensFlareTexObj::Free(void)
	{
		if(glIsTexture(bigGlowTexture))
		{
			glDeleteTextures(1, &bigGlowTexture);
		}

		if(glIsTexture(starTexture))
		{
			glDeleteTextures(1, &starTexture);
		}

		if(glIsTexture(glowTexture))
		{
			glDeleteTextures(1, &glowTexture);
		}

		if(glIsTexture(haloTexture))
		{
			glDeleteTextures(1, &haloTexture);
		}

		gConsole->LogSuccessText(LString("Lens flare Data succesfully released!"));
	}

	void LensFlareTexObj::useTexture(GLuint &texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	bool LensFlareTexObj::LoadFromFile(char* file, const Math::Vector3 &pos)
	{
		this->pos = pos;

		gConsole->LogInfo(LString("%s lens flare is loading...", file));
		std::ifstream fin;

		char strb[128];
		char bigGlow[256], star[256], glow[256], halo[256];
		float srcScale, scale, maxDistance, sunLumino, glowSunLumino;
		bool infinite = false;
		int tempInfinite;

		fin.open(file);

		if(fin.fail())
		{
			gConsole->LogError(LString("%s lens flare failed to load!", file));
			return false;
		}

		fin>>strb>>bigGlow;
		fin>>strb>>star;
		fin>>strb>>glow;
		fin>>strb>>halo;

		fin>>strb>>srcScale;
		fin>>strb>>scale;
		fin>>strb>>maxDistance;
		fin>>strb>>tempInfinite;

		fin>>strb>>sunLumino;
		fin>>strb>>glowSunLumino;

		fin.close();

		if(tempInfinite == 1) { infinite = true; }

		gConsole->LogSuccessText(LString("%s lens flare loaded successfully", file));

		return true;
	}

}