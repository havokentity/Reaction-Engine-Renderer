/*
	Self Descriptive header

	contains standalone texture helper functions

	classes contained/hierarchy,

	class Texture1D
			|
			|
			|===> class Texture2D
			|				|
			|				|
			|				|===> class TextureMutable
			|
			|===> class Texture3D
			|
			|
			|===> class TextureCube			
*/

#ifndef TEX_H__
#define TEX_H__

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ilut.lib")
#pragma comment(lib, "ilu.lib")

#ifndef INCLUDE_SKYBOX_ONLY
	#include "CoreEngine.h"
#endif

#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>

#include <gl/glext.h>
#include "Client.h"

namespace DifferentialArts
{
	#define MAX_TEX 256
	#define TEXTURE_INDEX_START 7

	/*
		This is a global texture array, preferably use this to store your textures
		start from 7 Onwards in the array, the indices before 7 are used for
		skybox textures and font texture.
		If you use this array for your textures, you dont need to bother about
		freeing them at all, the FreeTextures() function takes care of it
	*/

	typedef GLuint GLTexture; 
	extern GLuint g_Textures[MAX_TEX]; 

	//Loads nearly any TEXTURE format, into a GLuint texture array
	RX_API bool LoadTexture(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps,
							GLuint &width, GLuint &height, bool sClampToEdge, bool tClampToEdge);

	//Loads nearly any TEXTURE format from cross cube maps, into a GLuint texture array
	RX_API bool LoadCrossCubeMapTexture(const char* fileName, GLuint texArray[], GLuint texID, bool mipmaps,
										GLuint &width, GLuint &height,
										GLuint HDRIFormat = GL_RGBA16F_ARB);

	//Loads nearly any TEXTURE format, into a single GLuint (loads from lump)
	RX_API bool LoadTextureFromLump(GLvoid* lump, GLuint texArray[], GLuint texID, bool Mipmaps, GLuint size_l,
									GLuint &width, GLuint &height, bool sClampToEdge, bool tClampToEdge); 

	//Loads nearly any TEXTURE format, into a single GLuint
	RX_API bool LoadTexture(const char* fileName, GLuint texture, bool Mipmaps, GLuint &width, GLuint &height,
							bool sClampToEdge, bool tClampToEdge); 

	//Loads nearly any TEXTURE format from cross cube maps, into a single GLuint
	RX_API bool LoadCrossCubeMapTexture(const char* fileName, GLuint texture, bool mipmaps, GLuint &width, GLuint &height,
										GLuint HDRIFormat = GL_RGBA16F_ARB);

	//Loads nearly any TEXTURE format, into a single GLuint (loads from lump)
	RX_API bool LoadCrossCubeMapTextureFromLump(GLvoid* lump, GLuint texture, bool mipmaps, GLuint size_l,
												GLuint &width, GLuint &height,
												GLuint HDRIFormat = GL_RGBA16F_ARB); 

	//Loads nearly any TEXTURE format, into a GLuint texture array (loads from lump)
	RX_API bool LoadCrossCubeMapTextureFromLump(GLvoid* lump, GLuint texArray[], GLuint texID, bool mipmaps,
												GLuint size_l,
												GLuint &width, GLuint &height,
												GLuint HDRIFormat = GL_RGBA16F_ARB); 

	//Loads nearly any TEXTURE format, into a single GLuint (loads from lump)
	RX_API bool LoadTextureFromLump(GLvoid* lump, GLuint texture, bool mipmaps, GLuint size_l,
									GLuint &width, GLuint &height,
									GLuint HDRIFormat = GL_RGBA16F_ARB,
									bool sClampToEdge = true, bool tClampToEdge = true); 

	//Loads a 1 dimensional texture, Loads nearly any TEXTURE format
	RX_API bool LoadTexture1D(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps, GLuint &width,
							  bool sClampToEdge); 

	//Loads a 1 dimensional texture, Loads nearly any TEXTURE format (loads from lump)
	RX_API bool LoadTexture1DFromLump(GLvoid* lump, GLuint texArray[], GLuint texID, bool Mipmaps,
									  GLuint size_l, GLuint &width, bool sClampToEdge); 

	//Loads a texture and excludes the given R,G,B colour pixels (colour key) Loads nearly any TEXTURE format
	RX_API bool LoadTextureKey(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps,
							   GLubyte r, GLubyte g, GLubyte b, GLuint &width, GLuint &height); 

	//Loads a single texture, dont use this, has bugs
	RX_API bool LoadTextureSingle(const char* fileName, GLuint* id, bool Mipmaps); 

	//Sets the current used texture to the specified texture
	RX_API void setTexture(unsigned int tex);

	//Sets the current used texture to the specified texture, used for multitexturing,  SPecify texture unit, 4 MAX
	RX_API void setMultiTexture(unsigned int tex, unsigned int texUNIT); //DEPRECATED DON'T EVER EVER USE

	//Frees all textures in g_Textures, doesn't free all textures used in engine
	RX_API void FreeTextures();

	//Delete a texture referenced by a GLuint
	RX_API void DeleteTexture(GLTexture &texture);

	//Generate a single ilut image
	RX_API ILuint GenerateSingleImage(void);

	// Delete the image name. of ilut
	RX_API void DeleteSingleImage(ILuint ImageName);

	/*===========DONT BOTHER ABOUT THE BELOW FUNCTIONS, DONT USE THEM EITHER==========*/

	//Dont bother about the below two functions, they are internal to the framework
	RX_API bool LoadTextureForSkyBox(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps);

	//Loads JPG texture only, into a GLuint texture array
	//RX_API bool LoadTextureJPG(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps, float reduce); 

	//Loads a texture and excludes the given R,G,B colour pixels (colour key) JPG ONLY
	//RX_API bool LoadTextureJPGKey(const char* fileName, GLuint texArray[], GLuint texID, bool Mipmaps, float reduce, GLubyte r, GLubyte g, GLubyte b); 


	/*===========END OF DONT BOTHER==========*/
	/* Font texture */
	//#define FONT_TEXTURE 0
	/* SkyBox defines, used by class Skybox */
	/*
	#define FRONT		1
	#define BACK		2
	#define TOP			3
	#define BOTTOM		4
	#define LEFT		5
	#define RIGHT		6
	*/

	enum ENUM_BASE_TEXTURE_SET_ENUMERATION
	{
		FONT_TEXTURE,
		FRONT,
		BACK,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	/*
	==========================================================
		class Texture1D
	==========================================================
	*/

	/*!	\class Texture1D
		\brief Handles single dimensional textures.
		\author Rajesh Peter Douglas D'Monte

		Use this class if you would like to load and texture map
		and objects with single dimensional textures.	
	*/

	class RX_API Texture1D
	{
	public:
		Texture1D();//!< Constructor, this constructor sets the texture type to GL_TEXTURE_1D automatically	
		
		~Texture1D();//!< Destructor, doesn't do anything, Instead call Free() method. 
		/*!< 
			This is for memory security reasons. In case external data relied on this class and
			the deconstructor was called, then the program will crash. So the user is expected
			to free objects manually.

			@see Free()
		*/

		GLuint getTextureUnit(void);//!< Returns the current texture unit the texture object is using
		/*!<
			Retrieves the current texture unit the texture object is using

			@return A \a GLuint that represents the texture unit the current texture object is bound to.
		*/
		void bindTextureToTextureUnit(GLuint unit);//!< Binds the texture to texture unit 'GLuint unit'
		void bindTexture(void); 
		//!< Set this texture to render onwards on geometry (uses texture the last active texture unit)
		
		bool saveImageToDisk(const char* oFileName, int mipLevel = 0, GLuint face = 0); //!< Saves the texture to disk
		/*!<
			Use this method to save your texture onto disk.

			\param oFileName The path on disk to which the texture is to be saved.
			\param mipLevel For specifying the mipmap level to be saved.
			\param face  This parameter is ignored for Texture1D and Texture2D.
			\a face is to be used with TextureCube only. \a Range [1, 6].

			@return true if the function succeeds
		*/

		inline GLuint getTextureType(void);//!< Returns the texture type. For Texture1D, it will return \a GL_TEXTURE_1D
		inline GLuint getTexture(void);//!< Returns the texture object, a GLuint
		inline GLuint* getPointerToTexture(void);//!< Returns the pointer to the texture object, a GLuint
		void operator=(Texture1D &pCTexture);
		//!< This doesn't copy the contents of the given texture. It just assigns the same texture ID.
		
		virtual bool LoadTextureImage(const char* textureFileName, bool mipmaps = false,
									  bool sClampToEdge = true,
									  bool tClampToEdge = true /*this parameter is ignored if texture is 1D*/);
		//!< Loads a texture from disk. 
		/*!<
			\param textureFileName The texture path on disk.
			\param mipmaps Specifies if the texture is mipmapped or not.
			\param sClampToEdge Specifies if the texture is s clamped or not.
			\param tClampToEdge Specifies if the texture is t clamped or not. Ignored for Texture1D.

			@return true if the function succeeds
		*/

		virtual void SetTextureClamping(bool sClampToEdge = true, bool tClampToEdge = true); 
		//!< Sets the texture clamping
		/*!<
			\param sClampToEdge Specifies if the texture is s clamped or not.
			\param tClampToEdge Specifies if the texture is t clamped or not. Ignored for Texture1D.

			@return true if the function succeeds
		*/		

		virtual bool LoadTextureImageFromLump(GLvoid* lump, GLuint size, bool mipmaps = false,
											  bool sClampToEdge = true,
											  bool tClampToEdge = true /*this parameter is ignored*/);
		//!< Loads a texture from a data lump. 
		/*!<
			\param lump Pointer to data lump.
			\param size The size of the lump. If you want no bounds checking, specify 0.
			\param mipmaps Specifies if the texture is mipmapped or not.
			\param sClampToEdge Specifies if the texture is s clamped or not.
			\param tClampToEdge Specifies if the texture is t clamped or not. Ignored for Texture1D.

			@return true if the function succeeds
		*/

		virtual bool LoadTextureImage_MipMaps(const char* textureFileName); 
		//!< Deprecated. Use LoadTextureImage() or LoadTextureImageFromLump() instead
		/*!<
			\deprecated
			@see LoadTextureImage()
			@see LoadTextureImageFromLump()
		*/

		virtual bool CreateEmptyTexture(int width, int height, int internalFormat/*Can be channels*/,
										int format, bool mipMapExt, int type = GL_UNSIGNED_BYTE,
										int minFilter = GL_NEAREST, int magFilter = GL_NEAREST,
										bool sClampToEdge = true, bool tClampToEdge = true);
		//!< Creates an empty texture
		/*!<
			\param width The texture width
			\param height The texture height. Ignored in Texture1D.
			\param internalFormat The internal format of the texture. Can be number of channels. \a Range [0, 4].
			\param format The texture format. Eg: \a GL_RGB, \a GL_RGBA.
			\param mipMapExt Specifies if the texture is to be mipmapped or not.
			\param type The texture format type. Eg: \a GL_UNSIGNED_BYE, \a GL_INT.

			@return true if the function succeeds
		*/

		virtual void RenderCurrentViewportToTexture(void);
		//!< Renders the current viewport to this texture object. Doesn't work for floating point textures.
		virtual void Free(void);	
		//!< Frees the texture object and prints the path name, if the texture had a path name.
		void ReleaseTextureOnly(void);	//!< Deprecated. Frees the texture object and doesn't print the path name,
		/*!<
			\deprecated

			@see Free()
		*/

		GLuint getWidth(void) const; //!< Returns the width of the texture

		const char* getFileName(void) const; //!< Returns the file name of the texture

	protected:
		GLuint mTextureUnit;
		GLuint mTextureType;
		GLuint mTexture;
		char fileName[256];
		GLuint mWidth;

		GLuint mFileLoadMode;

		Texture1D(GLuint type);
	};

	/*
	==========================================================
		class Texture2D inherits from Texture1D
	==========================================================
	*/

	/*!	\class Texture2D
		\brief Class for handling 2 dimensional textures.
		\author Rajesh Peter Douglas D'Monte	

		Use this class if you would like to load and texture map
		and objects with 2 dimensional textures.
	*/

	class RX_API Texture2D: public Texture1D
	{
	public:
		Texture2D();//!< Constructor
		~Texture2D();//!< Destructor. Does Nothing. Call Free() instead.
		/*!< 
			This is for memory security reasons. In case external data relied on this class and
			the deconstructor was called, then the program will crash. So the user is expected
			to free objects manually.

			@see Free()
		*/

		virtual bool LoadTextureImage(const char* textureFileName, bool mipmaps = false,
									  bool sClampToEdge = true, bool tClampToEdge = true);
		virtual bool LoadTextureImageFromLump(GLvoid* lump, GLuint size, bool mipmaps = false,
											  bool sClampToEdge = true, bool tClampToEdge = true);

		void SetBorderColor(float r = 0.6f, float g = 0.6f, float b = 0.6f, float a = 1.0f);

		virtual bool CreateEmptyTexture(int width, int height, int internalFormat/*Can be channels*/,
										int format, bool mipMapExt, int type = GL_UNSIGNED_BYTE,
										int minFilter = GL_NEAREST, int magFilter = GL_NEAREST,
										bool sClampToEdge = true, bool tClampToEdge = true,
										int border = 0); //Format can be GL_RGB, GL_DEPTH_COMPONENT etc
		virtual void RenderCurrentViewportToTexture(void);

		virtual void SetTextureClamping(bool sClampToEdge = true, bool tClampToEdge = true); 
		//!< Sets the texture clamping
		/*!<
			\param sClampToEdge Specifies if the texture is s clamped or not.
			\param tClampToEdge Specifies if the texture is t clamped or not. Ignored for Texture1D.

			@return true if the function succeeds
		*/

		/*
			Method Name: LoadTextureImage_MipMaps
			IMPORTANT: This function is deprecated! use LoadTextureImage instead.
			For backward compatibility reasons, this method is retained
		*/
		virtual bool LoadTextureImage_MipMaps(const char* textureFileName);
		virtual void Free(void);

		GLuint getHeight(void) const; //!< Returns the height of the texture

	protected:	
		GLuint mHeight;
	};

	/*
	==================================================================================================================
		class Texture3D inherits from Texture1D

		WARNING: USEABLE FUNCTIONS ARE, Free, PrepareJitterMap, other functions are NOT to be used with 3D textures
	==================================================================================================================
	*/

	/*!	\class Texture3D
		\brief Class for handling 3 dimensional textures.
		\author Rajesh Peter Douglas D'Monte	

		Use this class if you would like to prepare 3D jittermap textures.
		
		\bug None of the inherited functions work except for Free(). This class is to be used exclusively for 
		preparing jitter maps alone.
	*/
	class RX_API Texture3D: public Texture1D
	{
	public:
		Texture3D();//!< Constructor
		~Texture3D();//!< Destructor. Does Nothing. Call Free() instead.
		/*!< 
			This is for memory security reasons. In case external data relied on this class and
			the deconstructor was called, then the program will crash. So the user is expected
			to free objects manually.

			@see Free()
		*/

		bool PrepareJitterMap(GLint size, GLint samples_u, GLint samples_v); //!< Prepares a jitter map as a 3D texture
		/*!<
			Prepares a jitter map as a 3D texture.

			\param size The resolution of the 3D texture. Eg: \a 16
			\param samples_u The number of u samples. Eg: \a 4
			\param samples_v The number of v samples. Eg: \a 8

			@return true if the function succeeds
		*/	

		virtual void Free(void);

	protected:
		GLuint mHeight, mDepth;
	};

	/*
		class TextureCube inherits from Texture1D, doesn't load cube map 2D textures... see 
		class CSkyBox for skyboxes and cube map images
	*/

	/*!	\class TextureCube
		\brief Class for handling cubemap textures.
		\author Rajesh Peter Douglas D'Monte	

		Use this class if you would like to load and texture map
		and objects with cube map textures.
	*/
	class RX_API TextureCube: public Texture1D
	{
	public:
		TextureCube();//!< Constructor
		~TextureCube();//!< Destructor. Does Nothing. Call Free() instead.
		/*!< 
			This is for memory security reasons. In case external data relied on this class and
			the deconstructor was called, then the program will crash. So the user is expected
			to free objects manually.

			@see Free()
		*/

		virtual bool LoadTextureImage(const char* textureFileName, bool mipmaps = false, GLuint HDRIFormat = GL_RGBA16F_ARB);
		//!< Loads only from CROSS CUBE MAP IMAGE
		/*!<
			The method definition is different from that of Texture1D

			\param textureFileName The file path to load from.
			\param mipmaps Specifies whether the texture is to be mipmapped or not.
			\param HDRIFormat Specifies the texture format.

			@return true if the function succeeds
		*/
		virtual bool LoadTextureImageFromLumpCube(GLvoid* lump, GLuint size, bool mipmaps = false,
											  GLuint HDRIFormat = GL_RGBA16F_ARB);
		//!< Loads a cube map texture from lump data.
		/*!<
			Loads a cube map texture from lump data

			\param lump Pointer to data lump.
			\param size The size of the lump. If you want no bounds checking, specify 0.
			\param mipmaps Specifies if the texture is mipmapped or not.
			\param HDRIFormat Specifies the texture format.

			@return true if the function succeeds
		*/

		virtual bool CreateEmptyTexture(int width, int height, int internalFormat/*Can be channels*/,
										int format, bool mipMapExt, int type = GL_UNSIGNED_BYTE,
										int minFilter = GL_NEAREST, int magFilter = GL_NEAREST,
										bool sClampToEdge = true, bool tClampToEdge = true,
										int border = 0); //Format can be GL_RGB, GL_DEPTH_COMPONENT etc
		virtual void RenderCurrentViewportToTexture(int cubeFace);

		bool GenerateNormalizationCubeMap(GLuint size);//!< Generates a normalization cubemap.
		/*!<
			This method is used to generate a normalization cubemap

			\param size Specifies the resolution of the normalization cube map. Has to be a perfect square and
			a power of two. Eg: \a 256, \a 128, \a 512 etc

			@return true if the function succeeds
		*/
		virtual void Free(void);

	protected:
		GLuint mHeight;
	};

	/*
	========================================================================================
		General TARGET Texture class (GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_NV ONLY)
	========================================================================================
	*/

	/*!	\class TextureMutable
		\brief Class for handling 2D textures.
		\author Rajesh Peter Douglas D'Monte	

		Use this class if you would like to load and texture map
		and objects with 2D textures. It differs from Texture2D by giving the user
		the ability to change the texture object type to \a GL_TEXTURE_RECTANGLE_NV
	*/

	class RX_API TextureMutable: public Texture2D
	{
	public:
		TextureMutable();//!< Constructor
		~TextureMutable();//!< Destructor. Does Nothing. Call Free() instead.
		/*!< 
			This is for memory security reasons. In case external data relied on this class and
			the deconstructor was called, then the program will crash. So the user is expected
			to free objects manually.

			@see Free()
		*/

		/* target can be GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_NV */
		void setTarget(GLenum target); //!< MUST BE CALLED BEFORE you can use an instance of this class.
		/*!<
			MUST BE CALLED BEFORE you can use an instance of this class

			\param target Specifies the texture object target. Can be \a GL_TEXTURE_2D or \a GL_TEXTURE_RECTANGLE_NV.
		*/
		virtual void Free(void);
	};
}

#endif