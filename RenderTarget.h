/*
=========================================================================================
	class RenderTargetError, RenderTarget, class RenderTarget_CUBEMAP -> class RenderTarget_ShadowCubeMap
=========================================================================================
*/

#ifndef _RenderTarget_H__
#define _RenderTarget_H__

#include "CoreEngine.h"
#include "Client.h"

namespace DifferentialArts
	{
	typedef GLuint RenderBuffer;

	/*
	=========================================================================================
		class RenderTargetError
		For general FBO outputs
	=========================================================================================
	*/

	/*!	\class RenderTargetError
		\brief FBO error class
		\author Rajesh Peter Douglas D'Monte

		This class is used for handling frame buffer object errors.
		Its used exclusively by RenderTarget.
	*/
	class RX_API RenderTargetError
	{
	protected:
		bool logOutput(char* cname, int mode, GLenum status);
	};

	/*
	=========================================================================================
		class RenderTarget
		For general FBO with 2D textures
	=========================================================================================
	*/
	/*!	\class RenderTarget
		\brief 2D FBO class
		\author Rajesh Peter Douglas D'Monte

		This class is used for using 2 dimensional frame buffer objects
	*/
	class RX_API RenderTarget: public RenderTargetError
	{
	public:
		TextureMutable* mTexture; //!< Texture attachments
		TextureMutable mDepthTexture;//!< Depth attachments

		RenderTarget(void); //!< Constructor
		~RenderTarget(void);//!< Deconstructor

		bool Initialize(int width, int height, bool mipmapped, int format = GL_RGBA,
						int internalFormat = GL_RGB8, int type = GL_UNSIGNED_BYTE,
						int samples = 0, int coverageSamples = 0, 
						int minFilter = GL_NEAREST, int magFilter = GL_NEAREST,
						int target = GL_TEXTURE_2D, bool depthRenderBuffer = true, bool textureRenderBuffer = true,
						GLuint renderTargets = 1, bool sClampToEdge = true, bool tClampToEdge = true,
						bool depthTexture = false, bool colorTextures = true, bool stencilRenderBuffer = false,
						int border = 0); 
		//!< Initializes the FBO object
		/*!<
			This method initializes the FBO object

			\param width Width of the FBO
			\param height Height of the FBO
			\param mipmapped Whether textures are mipmapped or not
			\param format OpenGL texture format
			\param internalFormat Internal OpenGL texture storage format
			\param type Data type
			\param samples Makes the FBO a MSAA FBO
			\param coverageSamples Makes the FBO a CSAA FBO
			\param minFilter Texture minification filter
			\param magFilter Texture magnification filter
			\param target The texture object target
			\param depthRenderBuffer Create a depth render buffer for this FBO
			\param textureRenderBuffer Create a texture render buffer for this FBO
			\param renderTargets Number of render targets
			\param sClampToEdge Enable texture clamp to edge on s direction
			\param tClampToEdge Enable texture clamp to edge on t direction
			\param depthTexture Enable depth texture attachment
			\param colorTextures Enable color texture attachment
			\param stencilRenderBuffer Create a stencil render buffer for this FBO

			\n

			If you want to use mSamples and mCoverage samples for MSAA and CSAA respectively then,
			you MUST call CoreEngine::InitializeMSAA and CoreEngine::InitializeCSAA respectively.
			You also need to blitMultisampleBufferToStandardFBO() for AA fbo's\n
			
			For MSAA and CSAA FBO's, Render buffers are \a compulsory. \n

			NOTE:\n \bug IF YOU ARE CREATING A MULTISAMPLING FBO, PLEASE CREATE IT AFTER THE INITIAL SCENE FBO
			OR THE DEPTH BUFFER WILL BE CORRUPTED. This happens on only certain cards.

			@return true if the method succeeds. \n
			For a more detailed list of errors in case of failure, look at the Log output.
		*/
		/*
		
		*/
		__forceinline void BeginRender(int sx = 0, int sy = 0); //!< Sets up viewport and renders to FBO
		__forceinline void EndRender(void); //!< Restores old viewport and unbinds FBO
		__forceinline void EndRenderWithoutUnbind(void); //!< Restores old viewport
		void Free(void);//!< Free resources allocated to the FBO
		__forceinline void blitBufferToRenderTarget(RenderTarget &fbo, GLbitfield buffers = GL_COLOR_BUFFER_BIT); //!< Blit a RenderTarget
		GLuint getFBO(void); //!< Returns the FBO OpenGL id.

		__forceinline void Bind(void);//!< Bind the FBO
		__forceinline void UnBind(void); //!< Unbinds the last FBO, that means all FBO's since only 1 FBO can be bound at a time

		inline void SetSingleAttachment(GLuint attachment = 0)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + attachment);
		}
		//!< Sets the FBO to use a single specified render target

		void UpdateMRTEnumBuffer(int useBufferSize); //!< Updates the draw buffer. WARNING: Make sure FBO is bound first!!! this is SLOW
		/*!<
			This method initializes the FBO object

			\param useBufferSize This is the number of attachments you want to attach in the attachedment buffer
		*/

		void SetAttachment(int RT_index, int attachement);
		//!< Sets the given render target index to use the given attachment

		GLuint getWidth(void) const;	//!< Returns the width of the render target
		GLuint getHeight(void) const;	//!< Returns the height of the render target
		GLuint getAttachments(void) const; //!< Returns the total attachments attached to the RenderTarget object

	protected:
		RenderBuffer* mRenderBuffersColor;
		RenderBuffer mRenderBufferDepth;

		bool bRenderBufferDepth;
		bool bRenderBufferColor;
		bool bRenderBufferStencil;

		bool bDepthTexture;
		bool bColorTextures;

		GLuint mFBO;	
		GLuint mWidth, mHeight;
		GLint mRenderTargets;
		int mSamples, mCoverageSamples;
		GLenum* MRT_attachements;
	};

	/*
	=========================================================================================
		class RenderTarget_CubeMap
		For CUBEMAP FBO with CUBEMAP textures
	=========================================================================================
	*/

	/*!	\class RenderTarget_CubeMap
		\brief Cubemap FBO class
		\author Rajesh Peter Douglas D'Monte

		This class is used for using Cubemap frame buffer objects. It is quite simplified
		when compared to the RenderTarget class
	*/
	class RX_API RenderTarget_CubeMap: public RenderTargetError
	{
	public:
		TextureCube mTexture; //!< Color texture attachment
		//TextureCube mDepthTexture;

		RenderTarget_CubeMap(void); //!< Constructor
		~RenderTarget_CubeMap(void); //!< Deconstructor

		virtual bool Initialize(int width, int height, bool mipmapped, int format = GL_RGBA,
						int internalFormat = GL_RGB8, int type = GL_UNSIGNED_BYTE,
						int minFilter = GL_NEAREST, int magFilter = GL_NEAREST,
						bool depthRenderBuffer = true, bool textureBuffer = true, 
						bool sClampToEdge = true, bool tClampToEdge = true, int border = 0); 
		//!< Initializes the cube map FBO object
		/*!<
			This method initializes the FBO object

			\param width Width of the FBO
			\param height Height of the FBO
			\param mipmapped Whether textures are mipmapped or not
			\param format OpenGL texture format
			\param internalFormat Internal OpenGL texture storage format
			\param type Data type
			\param minFilter Texture minification filter
			\param magFilter Texture magnification filter
			\param depthRenderBuffer Create a depth render buffer for this FBO
			\param textureBuffer Creates a color attachment and respective texture for this FBO
			\param renderTargets Coming soon! :D
			\param sClampToEdge Enable texture clamp to edge on s direction
			\param tClampToEdge Enable texture clamp to edge on t direction

			\n

			@return true if the method succeeds. \n
			For a more detailed list of errors in case of failure, look at the Log output.
		*/
		void BeginRender(int sx = 0, int sy = 0);//!< Sets up viewport and renders to FBO
		virtual void setFace(int cubeFace);//!< Sets which cube map face attachment to render to
		void EndRender(void); //!< Restores old viewport and unbinds FBO
		void Free(void); //!< Free resources allocated to the FBO
		GLuint getFBO(void); //!< Returns the FBO OpenGL id.

		void Bind(void);//!< Bind the FBO
		void UnBind(void); //!< Unbinds the last FBO, that means all FBO's since only 1 FBO can be bound at a time

		GLuint getWidth(void) const;	//!< Returns the width of the render target
		GLuint getHeight(void) const;	//!< Returns the height of the render target

	protected:
		GLuint mFBO;	
		GLuint mWidth, mHeight;	
		GLuint depthBufferIndex;

	private:	
	};

	/*
	=========================================================================================
		class RenderTarget_ShadowCubeMap
		For CUBEMAP FBO with SHADOW CUBEMAP textures
	=========================================================================================
	*/

	/*!	\class RenderTarget_ShadowCubeMap
		\brief Shadow Cube Map FBO class
		\author Rajesh Peter Douglas D'Monte

		This class is used for using Shadow Cubemap frame buffer objects used for
		OMNI directional light shadow maps. This class is a much more simplified version 
		of the RenderTarget_CubeMap class
	*/
	class RX_API RenderTarget_ShadowCubeMap: public RenderTarget_CubeMap
	{
	public:
		RenderTarget_ShadowCubeMap(void); //!< Constructor
		~RenderTarget_ShadowCubeMap(void); //!< Deconstructor

		virtual bool Initialize(int width, int height, bool mipmapped = false, int format = GL_ALPHA,
						int internalFormat = GL_ALPHA16F_ARB, int type = GL_FLOAT,
						int minFilter = GL_LINEAR, int magFilter = GL_LINEAR,
						bool depthBuffer = true, bool textureBuffer = false/* setting this arg, has no effect*/); 
		//!< Initializes the cube map FBO object. Leave the default parameters as they are.
		/*!<
			This method initializes the FBO object. \n
			Leave the default parameters as they are for shadow map objects.

			\param width Width of the FBO
			\param height Height of the FBO
			\param mipmapped Whether textures are mipmapped or not
			\param format OpenGL texture format
			\param internalFormat Internal OpenGL texture storage format
			\param type Data type
			\param minFilter Texture minification filter
			\param magFilter Texture magnification filter
			\param depthBuffer Create a depth render buffer for this FBO
			\param textureBuffer Doesn't affect anything in a RenderTarget_ShadowCubeMap
			\param renderTargets Number of render targets
			\param sClampToEdge Enable texture clamp to edge on s direction
			\param tClampToEdge Enable texture clamp to edge on t direction

			\n

			@return true if the method succeeds. \n
			For a more detailed list of errors in case of failure, look at the Log output.
		*/

		virtual void setFace(int cubeFace);//!< Sets which cube map face attachment to render to

	protected:
	};
}

#endif