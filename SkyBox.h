/*
	class Skybox, LensFlareTexObj
*/

#ifndef SKYBOX_H_
#define SKYBOX_H_

#ifndef INCLUDE_SKYBOX_ONLY
	#define INCLUDE_SKYBOX_ONLY
#endif

#pragma comment(lib, "opengl32.lib")

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)

#include <windows.h>

/* OpenGL required headers */
#include <gl/gl.h>
#include <gl/glext.h>

#include <fstream>

#include "Texture.h"
#include "utils/3DMATH.H"

namespace DifferentialArts
{
	/* There can be only One SkyBox Object, u'll have to re - use it*/
	/*!	\class Skybox
		\brief For using skyboxes
		\author Rajesh Peter Douglas D'Monte

		An instance of this class is already declared in CoreEngine. Use that
		instead of declaring your own instance
	*/
	class RX_API Skybox
	{
	public:
		Skybox(); //!< Constructor
		~Skybox(); //!< Destructor

		/* 
			The skybox texture folder should have pictures like eg: 

			back.jpg,
			front.jpg, 
			bottom.jpg,
			TOP.jpg,
			left.jpg,
			right.jpg

			You can even use bitmaps, or nearly any format
		*/
		bool CreateSkybox(char* skyBoxTexFolderName /* Enter the skybox's texture folder name only */,
						  const Math::Vector3 &size, bool mipmaps = true);
		//!< Loads a skybox from a folder
		/*!< Loads a skybox from a folder. Make sure the folder has a format.format file.
			 The format.format file should contain the extension of the skybox image format.

			 Eg: \n
			 \verbatim
			 .tga
			 \endverbatim
			
		*/
		bool CreateSkyboxFromCrossCubeMap(const char* skyBoxCrossTextureName, const Math::Vector3 &size, bool mipmaps, GLuint HDRIFormat = GL_RGBA16F_ARB);
		//!< Loads a skybox from a Cross cube map image file. For HDR textures only.
		/* Render flipped doesn't work properly for crossCubeMaps*/
		void Free(); //!< Releases resources allocated for the skybox

		/*
			If dynamic is true, pos is the position of the player, or camera in use,
			otherwise (dynamic is false), 
			pos becomes the world absolute position
		*/
		void setPosition(const Math::Vector3 &pos); //!< Sets the position of the skybox
		void Render(void); //!< Renders the sky box
		void RenderFlip(void); //!< Renders the sky box flipped.
		void RenderShadow(void); //!< Renders the sky box for shadow pass
		void RenderFlipShadow(void); //!< Renders the sky box flipped for shadow pass

	private:
		Math::Vector3 mv_pos;
		Math::Vector3 size;
		bool inUse;
		GLuint drawlist;
		GLuint drawlist2;
		GLuint drawlistShadow;
		GLuint drawlistShadow2;
		static int count;		
	};


	/*
	======================================================================
		LENS FLARE TEXTURE OBJECT
	======================================================================
	*/

	class RX_API LensFlareTexObj
	{
	public:
		float scale;
		float srcScale;
		float maxDistance;
		bool infinite;
		float sunLumino;
		float glowSunLumino;
		float touchedLumino;
		Math::Vector3 pos;

		LensFlareTexObj();
		LensFlareTexObj(char* bigGlow, char* star, char* glow, char* halo, float srcScale,
						float scale, float maxDistance, bool infinite, float sunLumino, float glowSunLumino);
		~LensFlareTexObj();

		GLuint bigGlowTexture;
		GLuint starTexture;
		GLuint glowTexture;
		GLuint haloTexture;

		char bigGlowAddress[256];
		char starAddress[256];
		char glowAddress[256];
		char haloAddress[256];

		void Load(char* bigGlow, char* star, char* glow, char* halo, float srcScale,
				  float scale, float maxDistance, bool infinite, float sunLumino, float glowSunLumino);

		bool LoadFromFile(char* file, const Math::Vector3 &pos);
		void useTexture(GLuint &texture);

	private:
		void Free(void);
		
	};
}

#endif