/*
==================================================================
	class RenderTarget, class RenderTarget_CUBEMAP
==================================================================
*/

#include "RenderTarget.h"

namespace DifferentialArts
{
	/*
	==================================================================
		class RenderTargetError
		For general FBO outputs
	==================================================================
	*/
	bool RenderTargetError::logOutput(char* cname, int mode, GLenum status)
	{
		myGlobalGame->mlog.LogInfo(LString(cname));

		switch(mode)
		{
		case 0:
			myGlobalGame->mlog.LogInfo(LString("Depth buffer..."));
			break;
		case 1:
			myGlobalGame->mlog.LogInfo(LString("Color buffer..."));
			break;
		case 2:
			myGlobalGame->mlog.LogInfo(LString("Stencil and depth buffer..."));
			break;
		default:
			myGlobalGame->mlog.LogError(LString("Invalid FBO error mode!"));
			break;
		}

		switch( status )
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				myGlobalGame->mlog.LogSuccessText(LString("GL_FRAMEBUFFER_COMPLETE_EXT!"));
				return true;
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				myGlobalGame->mlog.LogError(LString("GL_FRAMEBUFFER_UNSUPPORTED_EXT!"));
				return false;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: 
				myGlobalGame->mlog.LogError(LString("FBO has one or several image attachments with different internal formats"));
				return false;
	 
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: 
				myGlobalGame->mlog.LogError(LString("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT"));
				return false;

			case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT: 
				myGlobalGame->mlog.LogError(LString("GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT"));
				return false;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: 
				myGlobalGame->mlog.LogError(LString("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT"));
				return false;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:                   
				myGlobalGame->mlog.LogError(LString("FBO incomplete draw buffer"));
				return false;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:                   
				myGlobalGame->mlog.LogError(LString("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT"));
				return false;

			default:
				myGlobalGame->mlog.LogError(LString("Something went wrong in Initialization! Error code: %d", status));
				return false;
				break;
		}
	}

	/*
	==================================================================
		class RenderTarget
		For general FBO with 2D textures
	==================================================================
	*/

	RenderTarget::RenderTarget(void)
	{
		this->mWidth = 16;
		this->mHeight = 16;
		this->mRenderBuffersColor = 0;
		this->bRenderBufferDepth = false;
		this->bRenderBufferColor = false;
		this->bRenderBufferStencil = false;
		this->MRT_attachements = 0;
		this->mTexture = 0;		
	}

	RenderTarget::~RenderTarget(void)
	{
	}

	void RenderTarget::Free(void)
	{	
		if(!myGlobalGame->isFBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("FBO is not supported, cannot use any class RenderTarget objects!"));
			return;
		}

		if(this->MRT_attachements)
		{
			delete[] this->MRT_attachements;
			this->MRT_attachements = 0;
		}

		if(this->bRenderBufferColor)
		{
			for(GLint i = 0; i < this->mRenderTargets; i++)
			{
				glDeleteRenderbuffersEXT(1, &this->mRenderBuffersColor[i]);
			}

			if(this->mRenderBuffersColor)
			{
				delete[] this->mRenderBuffersColor;
				this->mRenderBuffersColor = 0;
			}
		}

		if(this->bRenderBufferStencil || this->bRenderBufferDepth)
		{
			glDeleteRenderbuffersEXT(1, &this->mRenderBufferDepth);
		}

		if(this->bDepthTexture)
		{
			this->mDepthTexture.ReleaseTextureOnly();
		}

		if(this->bColorTextures)
		{
			if(this->mTexture)
			{
				for(GLint i = 0; i < this->mRenderTargets; i++)
				{
					this->mTexture[i].ReleaseTextureOnly();
				}

				delete[] this->mTexture;
				this->mTexture = 0;
			}
		}

		glDeleteFramebuffersEXT( 1, &this->mFBO );	

		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a FBO!"));
	}

	void RenderTarget::UpdateMRTEnumBuffer(int useBufferSize)
	{
		glDrawBuffers(useBufferSize, this->MRT_attachements);
	}

	void RenderTarget::SetAttachment(int RT_index, int attachement)
	{
		this->MRT_attachements[RT_index] = GL_COLOR_ATTACHMENT0_EXT + attachement;
	}

	bool RenderTarget::Initialize(int width, int height, bool mipmapped, int format, int internalFormat, int type, 
						  int samples, int coverageSamples, int minFilter, int magFilter, int target,
						  bool depthRenderBuffer, bool textureRenderBuffer, GLuint renderTargets,
						  bool sClampToEdge, bool tClampToEdge, bool depthTexture, bool colorTextures,
						  bool stencilRenderBuffer, int border)
	{	
		if(!myGlobalGame->isFBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("FBO is not supported, cannot use any class RenderTarget objects!"));
			return false;
		}

		this->mWidth = width;		
		this->mHeight = height;		
		this->mSamples = samples;
		this->mCoverageSamples = coverageSamples;

		this->bRenderBufferColor = textureRenderBuffer;
		this->bRenderBufferDepth = depthRenderBuffer;
		this->bRenderBufferStencil = stencilRenderBuffer;

		if (mSamples > 0)
		{
			this->bRenderBufferColor = true;
			this->bRenderBufferDepth = true;
		}

		this->bDepthTexture = depthTexture;
		this->bColorTextures = colorTextures;

		glGenFramebuffersEXT(1, &this->mFBO);
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );

		this->mRenderTargets = renderTargets;	

		if(this->mRenderTargets <= 0 && this->mRenderTargets >= myGlobalGame->getMaxRenderTargets())
		{
			myGlobalGame->mlog.LogError(LString("FBO failed to initialized, Render target out of range!"));
			return false;
		}

		this->MRT_attachements = new GLenum[this->mRenderTargets];

		if(!this->MRT_attachements)
		{
			myGlobalGame->mlog.LogError(LString("FBO failed to initialized, insufficient memory!"));

			return false;
		}

		if(this->bColorTextures)
		{
			this->mTexture = 0;
			this->mTexture = new TextureMutable[this->mRenderTargets];

			if(!this->mTexture)
			{
				myGlobalGame->mlog.LogError(LString("FBO failed to initialized, failed to initialize texture objects!"));
				return false;
			}
		}
		  
		if(this->bRenderBufferColor)
		{
			this->mRenderBuffersColor = new RenderBuffer[this->mRenderTargets];

			if(!this->mRenderBuffersColor)
			{
				myGlobalGame->mlog.LogError(LString("FBO failed to initialized, insufficient memory!"));
				return false;
			}
		}
		
		//FOR COLOR COMPONENTS

		//For MSAA / CSAA, then render buffers are compulsory
		if (mSamples > 0)
		{
			//Render buffers for colour attachments
			for(GLint i = 0; i < this->mRenderTargets; i++)
			{
				glGenRenderbuffersEXT(1, &mRenderBuffersColor[i]);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->mRenderBuffersColor[i]);

				if ((mCoverageSamples > 0) && myGlobalGame->isCSAASupported())
				{
					glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER_EXT,
						mCoverageSamples, mSamples, internalFormat, this->mWidth, this->mHeight);
				} else {
					glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, mSamples,
						internalFormat, this->mWidth, this->mHeight);
				}
			
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
							GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, this->mRenderBuffersColor[i]);
				//this->mMRT.push_back(GL_COLOR_ATTACHMENT0_EXT + i);
				this->SetAttachment(i, i);
			}

			if(this->bColorTextures)
			{
				for(GLint i = 0; i < this->mRenderTargets; i++)
				{
					mTexture[i].setTarget(target);
					mTexture[i].CreateEmptyTexture(this->mWidth, this->mHeight, internalFormat, format, mipmapped,
												   type, minFilter, magFilter, sClampToEdge, tClampToEdge, border); 

					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, target, mTexture[i].getTexture(), 0);				
				}			
			} else {
				 glDrawBuffer(GL_NONE);
				 glReadBuffer(GL_NONE);
			}
		} else { // If no CSAA / MSAA
			if(this->bRenderBufferColor)
			{
				for(GLint i = 0; i < this->mRenderTargets; i++)
				{
					glGenRenderbuffersEXT(1, &mRenderBuffersColor[i]);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderBuffersColor[i]);

					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalFormat, this->mWidth, this->mHeight);

					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
						GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, mRenderBuffersColor[i]);

					this->SetAttachment(i, i);
				}
			}

			if(this->bColorTextures)
			{
				for(GLint i = 0; i < this->mRenderTargets; i++)
				{
					mTexture[i].setTarget(target);
					mTexture[i].CreateEmptyTexture(this->mWidth, this->mHeight, internalFormat, format, mipmapped,
												   type, minFilter, magFilter, sClampToEdge, tClampToEdge, border); 

					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, target, mTexture[i].getTexture(), 0);
					//this->mMRT.push_back(GL_COLOR_ATTACHMENT0_EXT + i);
					this->SetAttachment(i, i);
				}

			} else {
				 glDrawBuffer(GL_NONE);
				 glReadBuffer(GL_NONE);
			}		
		}			

		GLenum status;

		if(this->bRenderBufferColor || this->bColorTextures)
		{
			this->UpdateMRTEnumBuffer(this->mRenderTargets);

			status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

			if(!this->logOutput(__FUNCTION__, 1, status))
			{
				return false;
			}
		}

		if(this->bRenderBufferDepth && this->bRenderBufferStencil) //FOR THE DEPTH & STENCIL COMPONENT
		{
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );

			if (mSamples > 0)
			{
				glGenRenderbuffersEXT(1, &this->mRenderBufferDepth);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);

				if ((mCoverageSamples > 0) && myGlobalGame->isCSAASupported())
				{
					glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER_EXT,
						mCoverageSamples, mSamples, GL_DEPTH24_STENCIL8_EXT, this->mWidth, this->mHeight);
				} else {
					glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, mSamples,
						GL_DEPTH24_STENCIL8_EXT, this->mWidth, this->mHeight);
				}

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
							GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
							GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);
			} else {
				glGenRenderbuffersEXT(1, &mRenderBufferDepth);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderBufferDepth);

				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, this->mWidth, this->mHeight);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
					GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mRenderBufferDepth);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
							GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);

				if(this->bDepthTexture)
				{
					mDepthTexture.setTarget(target);
					mDepthTexture.CreateEmptyTexture(this->mWidth, this->mHeight, GL_DEPTH24_STENCIL8_EXT,
						GL_DEPTH_STENCIL_EXT, false, GL_UNSIGNED_INT_24_8_EXT, minFilter, magFilter, 
						sClampToEdge, tClampToEdge, border); 

					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, target,
						mDepthTexture.getTexture(), 0);

					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, target,
						mDepthTexture.getTexture(), 0);
				}
			}

			status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

			if(!this->logOutput(__FUNCTION__, 2, status))
			{
				return false;
			}	
		} else if(this->bRenderBufferDepth) //FOR THE DEPTH COMPONENT
		{
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );

			if (mSamples > 0)
			{
				glGenRenderbuffersEXT(1, &this->mRenderBufferDepth);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);

				if ((mCoverageSamples > 0) && myGlobalGame->isCSAASupported())
				{
					glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER_EXT,
						mCoverageSamples, mSamples, GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);
				} else {
					glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, mSamples,
						GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);
				}

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
							GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->mRenderBufferDepth);
			} else {
				glGenRenderbuffersEXT(1, &mRenderBufferDepth);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderBufferDepth);

				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
					GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mRenderBufferDepth);

				if(this->bDepthTexture)
				{
					mDepthTexture.setTarget(target);
					mDepthTexture.CreateEmptyTexture(this->mWidth, this->mHeight, GL_DEPTH_COMPONENT24,
						GL_DEPTH_COMPONENT, false, GL_UNSIGNED_INT, minFilter, magFilter, sClampToEdge, tClampToEdge,
						border); 

					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, target,
						mDepthTexture.getTexture(), 0);
				}
			}	 		
		} else {
			if(this->bDepthTexture)
			{
				mDepthTexture.setTarget(target);
				mDepthTexture.CreateEmptyTexture(this->mWidth, this->mHeight, GL_DEPTH_COMPONENT24,
					GL_DEPTH_COMPONENT, false, GL_UNSIGNED_INT, minFilter, magFilter, sClampToEdge,
					tClampToEdge, border); 

				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, target,
					mDepthTexture.getTexture(), 0);
			}		

			status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

			if(!this->logOutput(__FUNCTION__, 0, status))
			{
				return false;
			}	
		}

		return true;
	}

	__forceinline void RenderTarget::BeginRender(int sx, int sy)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO );

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(sx, sy, this->mWidth, this->mHeight);
	}

	GLuint RenderTarget::getFBO(void)
	{
		return this->mFBO;
	}

	GLuint RenderTarget::getAttachments(void) const
	{
		return this->mRenderTargets;
	}

	__forceinline void RenderTarget::blitBufferToRenderTarget(RenderTarget &fbo, GLbitfield buffers)
	{
		//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		//glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		//glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, fbo.getFBO());
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo.getFBO());
		//glBindFramebufferEXT(GL_COLOR_ATTACHMENT0_EXT, fbo.getFBO());

		/*
		fbo.PopDrawBufferForMRT();
		fbo.UpdateMRTEnumBuffer();
		*/	


		//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		//glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		
		
		glBlitFramebufferEXT(0, 0, this->mWidth, this->mHeight, 0, 0, fbo.mWidth, fbo.mHeight, buffers, GL_NEAREST);

		/*
		fbo.PopDrawBufferForMRT();
		fbo.PushDrawBufferForMRT(1);
		fbo.UpdateMRTEnumBuffer();

		glBlitFramebufferEXT(0, 0, this->mWidth, this->mHeight, 0, 0, this->mWidth, this->mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		fbo.PopDrawBufferForMRT();

		fbo.PushDrawBufferForMRT(0);
		fbo.PushDrawBufferForMRT(1);
		fbo.UpdateMRTEnumBuffer();
		*/
	}

	__forceinline void RenderTarget::EndRender(void)
	{
		glPopAttrib();
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}

	__forceinline void RenderTarget::EndRenderWithoutUnbind(void)
	{
		glPopAttrib();
	}

	__forceinline void RenderTarget::Bind(void)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );
	}

	__forceinline void RenderTarget::UnBind(void)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}

	GLuint RenderTarget::getWidth(void) const
	{
		return this->mWidth;
	}

	GLuint RenderTarget::getHeight(void) const
	{
		return this->mHeight;
	}

	/*
	==================================================================
		class RenderTarget_CubeMap
		For CUBEMAP FBO with CUBEMAP textures
	==================================================================
	*/
	RenderTarget_CubeMap::RenderTarget_CubeMap(void)
	{
		this->mWidth = 16;
		this->mHeight = 16;
		this->depthBufferIndex = 0;
	}

	RenderTarget_CubeMap::~RenderTarget_CubeMap(void)
	{
	}

	void RenderTarget_CubeMap::Free(void)
	{	
		if(!myGlobalGame->isFBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("FBO is not supported, cannot use any class RenderTarget or RenderTarget_CubeMap objects!"));
			return;
		}

		this->mTexture.ReleaseTextureOnly();
		//this->mDepthTexture.ReleaseTextureOnly();

		glDeleteFramebuffersEXT( 1, &this->mFBO );	

		if(depthBufferIndex)
		{
			glDeleteRenderbuffersEXT(1, &depthBufferIndex);
		}

		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a Cube Map FBO!"));
	}

	bool RenderTarget_CubeMap::Initialize(int width, int height, bool mipmapped, int format,
								  int internalFormat, int type, int minFilter, int magFilter,
								  bool depthRenderBuffer, bool textureBuffer,
								  bool sClampToEdge, bool tClampToEdge, int border)
	{	
		if(!myGlobalGame->isFBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("FBO is not supported, cannot use any class RenderTarget or RenderTarget_CubeMap objects!"));
			return false;
		}

		this->mWidth = width;		
		this->mHeight = height;		

		glGenFramebuffersEXT(1, &this->mFBO);
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );
		  
		/* FOR THE TEXTURE/COLOR COMPONENT */	

		mTexture.CreateEmptyTexture(this->mWidth, this->mHeight, internalFormat, format, mipmapped,
									type, minFilter, magFilter, sClampToEdge, tClampToEdge, border); 

		if(textureBuffer)
		{
			//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalFormat, this->mWidth, this->mHeight);
			for(int i=0; i<6; i++)
			{
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
					GL_COLOR_ATTACHMENT0_EXT,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT+i,
					mTexture.getTexture(), 0);
			}
		}

		GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
		
		if(!this->logOutput(__FUNCTION__, 1, status))
		{
			return false;
		}

		/* FOR THE DEPTH COMPONENT */

		if(depthRenderBuffer)
		{
			/*
			glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );

			glGenRenderbuffersEXT(1, this->mDepthTexture.getPointerToTexture());
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->mDepthTexture.getTexture());
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
						GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->mDepthTexture.getTexture());
			*/

			glGenRenderbuffersEXT(1, &depthBufferIndex);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferIndex);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
									 GL_RENDERBUFFER_EXT, depthBufferIndex);

			status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

			if(!this->logOutput(__FUNCTION__, 0, status))
			{
				return false;
			}
		}

		return true;
	}

	void RenderTarget_CubeMap::BeginRender(int sx, int sy)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO );

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(sx, sy, this->mWidth, this->mHeight);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();	

		gluPerspective(90.0f, 1,
					   myGlobalGame->getZNear(), myGlobalGame->getZFar());
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	}

	void RenderTarget_CubeMap::setFace(int cubeFace)
	{
		if(cubeFace > 5 || cubeFace < 0)
		{
			myGlobalGame->mlog.LogError(LString("cubeFace (%d) parameter was invalid", cubeFace));
		}

		/*

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + cubeFace,
								  mTexture.getTexture(), 0);*/

		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + cubeFace,
								  mTexture.getTexture(), 0);
	}

	GLuint RenderTarget_CubeMap::getFBO(void)
	{
		return this->mFBO;
	}

	void RenderTarget_CubeMap::EndRender(void)
	{
		glPopAttrib();
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void RenderTarget_CubeMap::Bind(void)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );
	}

	void RenderTarget_CubeMap::UnBind(void)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}

	GLuint RenderTarget_CubeMap::getWidth(void) const
	{
		return this->mWidth;
	}

	GLuint RenderTarget_CubeMap::getHeight(void) const
	{
		return this->mHeight;
	}

	/*
	==================================================================
		class RenderTarget_ShadowCubeMap
		For CUBEMAP FBO with SHADOW CUBEMAP textures
	==================================================================
	*/

	RenderTarget_ShadowCubeMap::RenderTarget_ShadowCubeMap(void)
	{
		this->mWidth = 16;
		this->mHeight = 16;
	}

	RenderTarget_ShadowCubeMap::~RenderTarget_ShadowCubeMap(void)
	{
	}

	bool RenderTarget_ShadowCubeMap::Initialize(int width, int height, bool mipmapped, int format,
								  int internalFormat, int type, int minFilter, int magFilter,
								  bool depthBuffer, bool textureBuffer)
	{	
		if(!myGlobalGame->isFBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("FBO is not supported, cannot use any class RenderTarget or RenderTarget_CubeMap objects!"));
			return false;
		}

		int status;

		this->mWidth = width;		
		this->mHeight = height;		

		glGenFramebuffersEXT(1, &this->mFBO);
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, this->mFBO  );
		  
		/* FOR THE TEXTURE/COLOR COMPONENT */	

		mTexture.CreateEmptyTexture(this->mWidth, this->mHeight, internalFormat, format, mipmapped, type, minFilter, magFilter); 	

		/* FOR THE DEPTH COMPONENT */

		if(depthBuffer)
		{				
			
			glGenRenderbuffersEXT(1, &depthBufferIndex);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferIndex);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, this->mWidth, this->mHeight);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
									 GL_RENDERBUFFER_EXT, depthBufferIndex);	


			
			for(int i=0;i<6;i++)
			{
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
									  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i,
									  mTexture.getTexture(), 0);
			}
			

			//glDrawBuffer(GL_NONE);
			//glReadBuffer(GL_NONE); 

			status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

			if(!this->logOutput(__FUNCTION__, 0, status))
			{
				return false;
			}
		}

		return true;
	}

	void RenderTarget_ShadowCubeMap::setFace(int cubeFace)
	{
		if(cubeFace > 5 || cubeFace < 0)
		{
			myGlobalGame->mlog.LogError(LString("Shadow cubeFace (%d) parameter was invalid", cubeFace));
		}
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + cubeFace,
								  mTexture.getTexture(), 0);
	/*
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + cubeFace,
								  mDepthTexture.getTexture(), 0);
								  */
	}
}