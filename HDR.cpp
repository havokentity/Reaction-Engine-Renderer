/*
	This is the Post Process HDR Code. Data at this point of time is irrelevant,
	even name is quite irrelevant?

	anyway,
	classes contained are,

	HDR -> CRxHDR_And_Overlay
*/

#include "HDR.h"

namespace DifferentialArts
{
	/*
		class HDR
	*/

	HDR::HDR(void)
	{

	}

	HDR::~HDR(void)
	{
		//this->Free();
	}

	void HDR::Free(void)
	{
		this->mNoiseTexture.Free();
		this->mHDREffect[0].Free();
		this->mHDREffect[1].Free();
		this->downSampleSSAO[0].Free();
		this->downSampleSSAO[1].Free();
		this->mGaussianBlur.Free();
		this->mGaussianEffect.Free();
		this->mDownSample.Free();
	#ifdef USE_DISPLAY_LIST	
		myGlobalGame->deleteDisplayList(&this->mScreenList);	
	#else
		this->mVBOScreen.Free();
	#endif
		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a High Dynamic Range Object"));
	}

	/*
	================================================================================
		HDR::Initialize
	================================================================================
	*/

	bool HDR::Initialize(int mode, int width, int height, bool sClampToEdge, bool tClampToEdge,
						  int internalFormat)
	{
		this->effectSelect = 0;
		this->mpNormalMap = 0;
		this->bSSAO = 0;

		myGlobalGame->mlog.LogInfo(LString("Initializing a High Dynamic Range object..."));	

	#ifdef USE_DISPLAY_LIST
		myGlobalGame->createDisplayList(false, &this->mScreenList);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 1); glVertex2i(0, 0);
				glTexCoord2f(0, 0); glVertex2i(0, 768);
				glTexCoord2f(1, 0); glVertex2i(1024, 768);
				glTexCoord2f(1, 1); glVertex2i(1024, 0);
			glEnd();
		myGlobalGame->endDisplayList();

	#else
		mScreenVertices[0].nx = 0;
		mScreenVertices[0].ny = 1;
		mScreenVertices[0].nz = 0;

		mScreenVertices[1].nx = 0;
		mScreenVertices[1].ny = 1;
		mScreenVertices[1].nz = 0;

		mScreenVertices[2].nx = 0;
		mScreenVertices[2].ny = 1;
		mScreenVertices[2].nz = 0;

		mScreenVertices[3].nx = 0;
		mScreenVertices[3].ny = 1;
		mScreenVertices[3].nz = 0;

		mScreenVertices[0].tu = 0;
		mScreenVertices[0].tv = 1;
		mScreenVertices[0].vx = 0;
		mScreenVertices[0].vy = 0;
		mScreenVertices[0].vz = 0;

		mScreenVertices[1].tu = 0;
		mScreenVertices[1].tv = 0;
		mScreenVertices[1].vx = 0;
		mScreenVertices[1].vy = 768;
		mScreenVertices[1].vz = 0;

		mScreenVertices[2].tu = 1;
		mScreenVertices[2].tv = 0;
		mScreenVertices[2].vx = 1024;
		mScreenVertices[2].vy = 768;
		mScreenVertices[2].vz = 0;

		mScreenVertices[3].tu = 1;
		mScreenVertices[3].tv = 1;
		mScreenVertices[3].vx = 1024;
		mScreenVertices[3].vy = 0;
		mScreenVertices[3].vz = 0;
	#endif

		mGaussianBlur.Initialize(width, height, false, GL_RGBA, internalFormat, GL_FLOAT, 0, 0,
								  GL_LINEAR, GL_LINEAR, GL_TEXTURE_2D, false, false,
								  1, sClampToEdge, tClampToEdge);

		mDownSample.Initialize(width, height, false, GL_RGBA, internalFormat, GL_FLOAT, 0, 0,
							   GL_LINEAR, GL_LINEAR, GL_TEXTURE_2D, false, false,
							   1, sClampToEdge, tClampToEdge);
		mDownSample.UnBind();

		mNoiseTexture.LoadTextureImage("Data/Textures/noise.png", false, false, false);

	#ifdef USE_DISPLAY_LIST
	#else
		if(!this->mVBOScreen.InitializeTEX1(4, mScreenVertices))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}
	#endif

		this->mWidth = width;
		this->mHeight = height;

		/* TONE MAP SHADER */
		this->miMode  = mode;

		this->fRadialSettings[0] = 0.95f;
		this->fRadialSettings[1] = 0.1f;
		this->fRadialSettings[2] = 0.5f;
		this->fRadialSettings[3] = 0.5f;

		fUVAnimation[0]	= 0.0f;
		fUVAnimation[1] = 0.0f;
		fUVAnimation[2] = 0.05f;

		this->fblurAmount = 0.5f;
		this->flensExposure = 2.0f;	
		this->fGamma = 1.f;	

		this->ReloadHDREffect();			

		this->setFocalFallOff(600.0f, 1000.0f);
		this->setDOF(100.0f);

		this->commitUVAnimationToGPU();
		
		/* DOWN SAMPLE SHADER */
		/*
		if(!downSampleShader.LoadVertexProgram("Data/Shaders/HDR/downSample4.vcg", 0))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}

		if(!downSampleShader.LoadFragmentProgram("Data/Shaders/HDR/downSample4.pcg", 0))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}	

		downSampleShader.CombineVertexAndFragmentPrograms(false);
		//downSampleShader.AssignParameterToVertexProgram(texelScaleSize, "texelScaleSize");
		downSampleShader.AssignParameterToCombinedProgram(texelScaleSize, "texelScaleSize");
		*/		

		/* GAUSSIAN EFFECT */
		if(!mGaussianEffect.LoadEffect("Data/Shaders/HDR/GaussianBlur.cgfx"))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}

		mGaussianEffect.AssignParameterToEffect(this->horizontalDir, "horizontalDir");
		mGaussianEffect.AssignParameterToEffect(this->verticalDir, "verticalDir");

		/* SAMPLER VARIBLES ASSIGNMENT TO EFFECT */
		cgGLSetParameter2f(horizontalDir, 1.0f/((float)(this->mWidth)), 0);
		cgGLSetParameter2f(verticalDir, 0, 1.0f/((float)(this->mHeight)));

		myGlobalGame->mlog.LogSuccessText(LString("Successfully initialized a High Dynamic Range Object"));

		myGlobalGame->mlog.bindIntegerVariable("HDR::miMode", &this->miMode);
		
		return true;
	}

	bool HDR::ReloadHDREffect(void)
	{
		int i = 0;
		int totalEffects = 2;		

		for(i = 0; i < totalEffects; i++)
		{
			this->mHDREffect[i].Free();
			downSampleSSAO[i].Free();
		}

		

		if(!downSampleSSAO[0].LoadEffect("Data/Shaders/HDR/DownSampleAndSSAO.cgfx"))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}

		if(!downSampleSSAO[1].LoadEffect("Data/Shaders/HDR/DownSample.cgfx"))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}	

		downSampleSSAO[0].AssignParameterToEffect(texelScaleSize[0], "texelScaleSize");
		downSampleSSAO[1].AssignParameterToEffect(texelScaleSize[1], "texelScaleSize");

		cgGLSetParameter2f(texelScaleSize[0], (float)2.0f/this->mWidth, (float)2.0f/this->mHeight);
		cgGLSetParameter2f(texelScaleSize[0], (float)2.0f/this->mWidth, (float)2.0f/this->mHeight);

		if(!this->mHDREffect[0].LoadEffect("Data/Shaders/HDR/HDR.cgfx"))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}

		if(!this->mHDREffect[1].LoadEffect("Data/Shaders/HDR/HDRNormalMapDisplacement.cgfx"))
		{
			myGlobalGame->mlog.LogError(LString("High Dynamic Range Object failed to initialize"));
			return false;
		}		

		for(i = 0; i < totalEffects; i++)
		{
			mHDREffect[i].AssignParameterToEffect(radialSettings[i], "radialSettings");

			/*
				Depth of Field specific variables
			*/
			mHDREffect[i].AssignParameterToEffect(focalParams[i], "focalParams");	
			mHDREffect[i].AssignParameterToEffect(calFocalParams[i], "calFocalParams");	

			mHDREffect[i].AssignParameterToEffect(uvAnimation[i], "uvAnimation");	

			//mHDREffect[i].AssignParameterToEffect(viewProjectionInverseMatrix[i], "viewProjectionInverseMatrix");	
			//mHDREffect[i].AssignParameterToEffect(previous_viewProjectionMatrix[i], "previous_viewProjectionMatrix");				

			mHDREffect[i].AssignParameterToEffect(blurAmount[i], "blurAmount");
			mHDREffect[i].AssignParameterToEffect(lensExposure[i], "lensExposure");
			mHDREffect[i].AssignParameterToEffect(gamma[i], "gamma");	

			//mHDREffect[i].AssignParameterToEffect(eyeSpaceView[i], "eyeSpaceView");
			//mHDREffect[i].AssignParameterToEffect(projectionMatrix[i], "projectionMatrix");
			
			cgGLSetParameter4fv(this->radialSettings[i], this->fRadialSettings);			

			cgGLSetParameter1f(blurAmount[i], this->fblurAmount);
			cgGLSetParameter1f(lensExposure[i], this->flensExposure);		
			cgGLSetParameter1f(gamma[i], this->fGamma);

			cgGLSetParameter3f(this->focalParams[i], this->fMinDistance, this->fMaxDistance, this->fOneOverMinDistance);
			cgGLSetParameter2f(this->calFocalParams[i], this->fFocalFallOff, this->fOneOverFocalFallOff);

			cgGLSetParameter3fv(this->uvAnimation[i], fUVAnimation);
		}				

		return true;
	}

	/*
	void HDR::comittEyeSpaceView(Math::Vector3 &v)
	{
		cgGLSetParameter3f(this->eyeSpaceView[effectSelect], v.x, v.y, v.z);;
	}

	void HDR::comittProjectionMatrix(const float* m)
	{
		cgGLSetMatrixParameterfr(this->projectionMatrix[effectSelect], m);;
	}
	
	void HDR::commitViewInverseProjectionMatrix(const float* m)
	{
		cgGLSetMatrixParameterfr(this->viewProjectionInverseMatrix[effectSelect], m);;
	}

	void HDR::commitPreviousViewProjectionMatrix(const float* m)
	{
		cgGLSetMatrixParameterfr(this->previous_viewProjectionMatrix[effectSelect], m);
	}*/

	void HDR::commitRadialSettingsToGPU(void)
	{
		cgGLSetParameter4fv(this->radialSettings[effectSelect], this->fRadialSettings);
	}

	void HDR::commitUVAnimationToGPU(void)
	{
		cgGLSetParameter3fv(this->uvAnimation[effectSelect], fUVAnimation);
	}

	void HDR::setDOF(float minDist)
	{
		this->fMinDistance = minDist;
		this->fMaxDistance = this->fMinDistance + this->fFocalFallOff;
		this->fOneOverMinDistance = 1/this->fMinDistance;

		cgGLSetParameter3f(this->focalParams[effectSelect], this->fMinDistance, this->fMaxDistance, this->fOneOverMinDistance);
	}

	void HDR::setFocalFallOff(float ff, float infiniteDistance)
	{
		this->fFocalFallOff = ff;
		this->fInfiniteDistance = infiniteDistance;
		this->fOneOverFocalFallOff = 1/fInfiniteDistance;
		
		cgGLSetParameter2f(this->calFocalParams[effectSelect], this->fFocalFallOff, this->fOneOverFocalFallOff);
	}

	void HDR::setRadialScaleDown(float x)
	{
		if(this->miMode != 1)
		{
			return;
		}

		this->fRadialSettings[0] = x;

		//cgGLSetParameter1f(radialScaleDown, this->fRadialScaleDown);
	}

	void HDR::SetRadialCardinalOrigin(float x, float y)
	{
		if(this->miMode != 1)
		{
			return;
		}

		this->fRadialSettings[2] = (x < 0.0f) ? 0 : ((x > 1.0f) ? 1.0f : x);
		this->fRadialSettings[3] = (y < 0.0f) ? 0 : ((y > 1.0f) ? 1.0f : y);

		//cgGLSetParameter2f(radialCardinalOrigin, this->fRadialCardinalOriginX, this->fRadialCardinalOriginY);
	}

	void HDR::setLensExposure(float x)
	{
		this->flensExposure = x;
		cgGLSetParameter1f(lensExposure[effectSelect], this->flensExposure);	
	}

	void HDR::setBlurAmount(float x)
	{
		this->fblurAmount = x;
		verifyBlurAmount();
		cgGLSetParameter1f(blurAmount[effectSelect], this->fblurAmount);
	}

	void HDR::setRadialBlurAmount(float x)
	{	
		if(this->miMode != 1)
		{
			return;
		}

		this->fRadialSettings[1] = x;

		//cgGLSetParameter1f(radialBlurAmount, this->fRadialBlurAmount);
	}

	void HDR::IncreaseExposure(void)
	{
		this->flensExposure *= 2;
		cgGLSetParameter1f(lensExposure[effectSelect], this->flensExposure);	
	}

	void HDR::DecreaseExposure(void)
	{	
		this->flensExposure *= 0.5f;	
		cgGLSetParameter1f(lensExposure[effectSelect], this->flensExposure);	
	}

	void HDR::IncreaseExposureBySteps(void)
	{
		this->flensExposure += 0.1f;
		cgGLSetParameter1f(lensExposure[effectSelect], this->flensExposure);	
	}

	void HDR::DecreaseExposureBySteps(void)
	{
		this->flensExposure -= 0.1f;
		cgGLSetParameter1f(lensExposure[effectSelect], this->flensExposure);	
	}

	void HDR::IncreaseBlur(void)
	{
		this->fblurAmount += 0.1f;
		this->verifyBlurAmount();
		cgGLSetParameter1f(blurAmount[effectSelect], this->fblurAmount);
	}

	void HDR::DecreaseBlur(void)
	{
		this->fblurAmount -= 0.1f;
		this->verifyBlurAmount();
		cgGLSetParameter1f(blurAmount[effectSelect], this->fblurAmount);
	}

	void HDR::IncreaseRadialBlur(void)
	{	
		if(this->miMode != 1)
		{
			return;
		}
		
		this->fRadialBlurAmount += 0.1f;
		//cgGLSetParameter1f(radialBlurAmount, this->fRadialBlurAmount);
	}

	void HDR::DecreaseRadialBlur(void)
	{	
		if(this->miMode != 1)
		{
			return;
		}

		this->fRadialBlurAmount -= 0.1f;

		//cgGLSetParameter1f(radialBlurAmount, this->fRadialBlurAmount);
	}

	void HDR::setGamma(float x)
	{
		this->fGamma = x;
		cgGLSetParameter1f(gamma[effectSelect], this->fGamma);	
	}

	void HDR::IncreaseGamma(void)
	{
		this->fGamma += 0.1f;
		cgGLSetParameter1f(gamma[effectSelect], this->fGamma);	
	}

	void HDR::DecreaseGamma(void)
	{
		this->fGamma -= 0.1f;
		cgGLSetParameter1f(gamma[effectSelect], this->fGamma);	
	}

	void HDR::DownSample(RenderTarget *sourceBuffer, RenderTarget *destinationBuffer)
	{
		destinationBuffer->BeginRender(0, 0);
		
		//myGlobalGame->EnableShaderProfiles();
		//this->downSampleShader.BindCombinedProgram();
		downSampleSSAO[!bSSAO].BindFirstPass();
		
		myGlobalGame->SaveAndSetup2DScreen(1024, 768);

		sourceBuffer->mTexture[0].bindTextureToTextureUnit(0);		

		if(bSSAO)
		{
			sourceBuffer->mTexture[2].bindTextureToTextureUnit(3);
			sourceBuffer->mTexture[1].bindTextureToTextureUnit(2);			

			this->mNoiseTexture.bindTextureToTextureUnit(4);
		}

	#ifdef USE_DISPLAY_LIST
		glCallList(this->mScreenList);	
	#else
		this->mVBOScreen.RenderTEX1_Continuos(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
	#endif
	}

	void HDR::SetMode(int mode)
	{
		this->miMode = (mode < 0) ? mode = 0 : ((mode > 2) ? mode = 2 : mode);
	}

	void HDR::Render(RenderTarget *scene)
	{
		/* DOWN SAMPLE PASS */
	#ifdef USE_DISPLAY_LIST
	#else
		myGlobalGame->EnableClientVBOState(0);
	#endif

		this->DownSample(scene, &mDownSample);

		// GAUSSIAN BLUR PASS (BLOOM) 
		// Horizontal Pass 
		this->mGaussianBlur.Bind();	
			this->mGaussianEffect.BindFirstPass();
			 mDownSample.mTexture[0].bindTextureToTextureUnit(0);
	#ifdef USE_DISPLAY_LIST
			glCallList(this->mScreenList);
	#else
			this->mVBOScreen.RenderTEX1_Continuos(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
	#endif

		// Vertical Pass 		
		this->mGaussianBlur.Bind();	
			this->mGaussianEffect.BindNextPass();
			mGaussianBlur.mTexture[0].bindTextureToTextureUnit(0);
	#ifdef USE_DISPLAY_LIST
			glCallList(this->mScreenList);
	#else
			this->mVBOScreen.RenderTEX1_Continuos(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
	#endif

		this->mDownSample.Bind();	
			this->mGaussianEffect.BindNextPass();
			this->mGaussianBlur.mTexture[0].bindTextureToTextureUnit(0);
	#ifdef USE_DISPLAY_LIST
			glCallList(this->mScreenList);
	#else
			this->mVBOScreen.RenderTEX1_Continuos(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
	#endif
		this->mDownSample.EndRender();

		/* TONE MAPPING PASS */
		if(miMode == 0)
		{
			mHDREffect[effectSelect].BindFirstPass();
		} else if(miMode == 1) 
		{
			mHDREffect[effectSelect].BindNamedPass("PhotographicToneMappingRadial");
		} else if(miMode == 2) 
		{
			mHDREffect[effectSelect].BindNamedPass("PhotographicToneMappingCrepuscular");
		}

		// bind textures
		//Bind scene texture
		scene->mTexture[0].bindTextureToTextureUnit(0);

		//Binds the blurred scene and SSAO map
		mDownSample.mTexture[0].bindTextureToTextureUnit(1);	

		//Binds the view space depth map for DOF
		scene->mTexture[2].bindTextureToTextureUnit(2);

		//Binds the ss normal map 
		scene->mTexture[1].bindTextureToTextureUnit(3);

		if(this->mpNormalMap && bSSAO)
		{
			this->mpNormalMap->bindTextureToTextureUnit(5);
		}

		#ifdef USE_DISPLAY_LIST		
				glCallList(this->mScreenList);
		#else
				this->mVBOScreen.RenderTEX1_Continuos(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
		#endif

		glActiveTextureARB(GL_TEXTURE0_ARB);
		//glEnable(GL_TEXTURE_2D);
		myGlobalGame->RestoreOldScreen();

		//toneMapShader.UnBindLastPixelProgram();
		//myGlobalGame->DisableClientVBOState(0);
	#ifdef USE_DISPLAY_LIST
	#else
		myGlobalGame->UnBindLastBuffer();
	#endif
		mHDREffect[effectSelect].ResetLastPass();		
	}

	void HDR::verifyBlurAmount(void)
	{
		this->fblurAmount = (this->fblurAmount>1.0f)?1.0f:this->fblurAmount;
		this->fblurAmount = (this->fblurAmount<0.0f)?0.0f:this->fblurAmount;
	}

	/* DOWNSAMPLE SHADER CONTROLS */
	void HDR::setTexelScale(float x, float y)
	{
		cgGLSetParameter2f(texelScaleSize[0], x, y);
		cgGLSetParameter2f(texelScaleSize[1], x, y);
	}

	void HDR::NullifyScreenNormalMap(void)
	{
		this->mpNormalMap = 0;

		effectSelect = 0;
	}

	void HDR::AssignScreenNormalMap(Texture2D* pTexture)
	{
		this->mpNormalMap = pTexture;

		effectSelect = 1;
	}
}