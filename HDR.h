/*
	This is the Post Process HDR Code. Data at this point of time is irrelevant,
	even name is quite irrelevant?

	anyway,
	classes contained are,

	HDR -> CRxHDR_And_Overlay

	the HDR class also controls, Depth of field effects, HDR effects, and bloom

	Don't use CRxHDR_And_Overlay, its deprecated
*/

#ifndef _HDR_H_
#define _HDR_H_

#include "CoreEngine.h"
#include "VertexBuffer.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "Client.h"

namespace DifferentialArts
{
	#define USE_DISPLAY_LIST

	/*
	=================================================
		class HDR
	=================================================
	*/

	/*!	\class HDR
		\brief HDR and bloom effect class
		\author Rajesh Peter Douglas D'Monte	

		Use this class if you would like to High dynamic range
		rendering and bloom in your application.

		\n
		Note:\n
		HDR needs a floating point FBO in order to work.
	*/
	class RX_API HDR
	{
	public:
		HDR(void); //!<Constructor
		~HDR(void);//!<Deconstructor

		virtual bool Initialize(int mode = 0, int width = 512, int height = 512,
								bool sClampToEdge = false, bool tClampToEdge = false,
								int internalFormat = GL_RGBA16F_ARB); //!< Initializes the HDR object
		/*!<
			This method initializes the HDR object. It relies on the shaders and effects that accompany the
			engine.

			\param mode HDR mode. 0 is default HDR with bloom, 1 adds radial post process + bloom, 2 adds crepuscular light post process + bloom
			\param width Width of the HDR object
			\param height Height of the HDR object
			\param sClampToEdge Clamp the texture to edge on the s direction
			\param tClampToEdge Clamp the texture to edge on the t direction
			\param internalFormat HDR internal texture format

			@return true if the initialization was successfull
		*/ 
		virtual void Free(void); //!<Free all resources allocated by the HDR object

		virtual void Render(RenderTarget* scene); //!< Renders the scene with HDR and Bloom

		void SetMode(int mode = 0);//!< Sets the HDR mode, look at HDR::Initialize for more information

		/* TONE MAP SHADER CONTROLS */		
		void setBlurAmount(float x = 0.0f);	//!< Set minimum blur amount
		void setLensExposure(float x);//!< Set lens exposure
		void IncreaseExposure(void); //!<Increases the
		void DecreaseExposure(void); //!<Halves the lens exposure
		void IncreaseExposureBySteps(void); //!<Doubles the lens exposure
		void DecreaseExposureBySteps(void); //!<Halves the lens exposure		
		void IncreaseBlur(void); //!<Increases the blur by 0.1
		void DecreaseBlur(void); //!<Decreases the blur by 0.1

		void setGamma(float x = 1.0f);	//!< Set the Gamma of the HDR output
		void IncreaseGamma(void); //!<Increases the gamma by 0.1
		void DecreaseGamma(void); //!<Decreases the gamma by 0.1		

		/* DOWNSAMPLE SHADER CONTROLS */
		void setTexelScale(float x, float y); //!Set downsample of texture, Eg 2/width, 2/height

		/* DEPTH OF FIELD SPECIFIC CONTROLS */
		void setDOF(float minDist = 100.0f); //!< Set distance for Depth of field (DOF)
		void setFocalFallOff(float ff = 600.0f, float infiniteDistance = 1000.0f); //!< Set clear distance and falloff distance for DOF

		/* MORE RADIAL SETTING CONTROLS */
		void SetRadialCardinalOrigin(float x = 0.5f, float y = 0.5f);	//!< Set radial blur origin. Reference (0,0) is at BOTTOM LEFT of screen, only valid when mode = 1
		void setRadialScaleDown(float x = 0.95f);//!< Set the radial blur texture rate of scaling, only valid when mode = 1
		void IncreaseRadialBlur(void); //!<Increases the radial blur by 0.1, only valid when mode = 1. Radial blur amount is clamped between 0 and 1.
		void DecreaseRadialBlur(void); //!<Decreases the radial blur by 0.1, only valid when mode = 1. Radial blur amount is clamped between 0 and 1.
		void setRadialBlurAmount(float x = 0.5f); //!< Sets the radial blur amount, only valid when mode = 1
		void commitRadialSettingsToGPU(void); //!< Commits the radial settings changes to GPU

		bool ReloadHDREffect(void); //!< Reloads the HDR effect.

		void AssignScreenNormalMap(Texture2D* pTexture); //!< Assigns a screen normal map texture

		void NullifyScreenNormalMap(void); //!< Removes the screen normal map effect

		float fUVAnimation[3]; //!< For uv animation the texcoord, don't forget to commit your UV animation to the GPU

		void commitUVAnimationToGPU(void);
		//void comittEyeSpaceView(Math::Vector3 &v);
		//void comittProjectionMatrix(const float* m);
		//void commitViewInverseProjectionMatrix(const float* m);
		//void commitPreviousViewProjectionMatrix(const float* m);

		bool bSSAO;

		#ifdef USE_DISPLAY_LIST
			DisplayList mScreenList;
		#else
			VertexSingleTextureCoord mScreenVertices[4];
			VertexBuffer mVBOScreen;
		#endif

	protected:
		RenderTarget mGaussianBlur, mDownSample;	
	
		//CGparameter viewProjectionInverseMatrix[2];
		//CGparameter previous_viewProjectionMatrix[2];
		//CGparameter eyeSpaceView[2];
		//CGparameter projectionMatrix[2];


		/* SHADERS */
		//Shader downSampleShader;	

		Texture2D mNoiseTexture, *mpNormalMap;

		/* TONE MAP SHADER PROPERTIES */
		float fblurAmount, flensExposure, fRadialBlurAmount, fGamma, fInfiniteDistance;
		//float fRadialCardinalOriginX, fRadialCardinalOriginY, fRadialScaleDown;

		float fRadialSettings[4];
		CGparameter radialSettings[2];

		CGparameter blurAmount[2], lensExposure[2], gamma[2]; 
		//CGparameter radialBlurAmount, radialScaleDown, radialCardinalOrigin;

		CGparameter uvAnimation[2];			

		/*
			Depth of Field specific variables
			===================================
			NOTE: FocalFallOff is the distance beyond minDis that is "clear", beyond this things become blurry
		*/
		CGparameter focalParams[2], calFocalParams[2];
		float fMinDistance, fMaxDistance, fOneOverMinDistance, fFocalFallOff, fOneOverFocalFallOff; 
		
		/* DOWN SAMPLE SHADER PROPERTIES */
		CGparameter texelScaleSize[2];

		/* EFFECTS */
		Effect mGaussianEffect, mHDREffect[2], downSampleSSAO[2];

		/* EFFECT PARAMETERS */
		CGparameter verticalDir, horizontalDir; //For mGaussianEffect

		int mWidth, mHeight;

		/* TONE MAP SHADER CONTROLS */
		void verifyBlurAmount(void);

		/* DOWNSAMPLE SHADER CONTROLS */
		void DownSample(RenderTarget *sourceBuffer, RenderTarget *destinationBuffer);	

		int miMode, effectSelect;		
	};
}

#endif