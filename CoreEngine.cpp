/*
================================================================
	classes contained are:

	class CoreEngine
================================================================
*/


#include "CoreEngine.h"

/*
================================================================
	 multitexturing capabilities extension capabilities
================================================================
*/
#include "GL_MultiTextureAssigners.h"

namespace DifferentialArts
{	
	/*
	================================================================
		Extensions
	================================================================
	*/

	/*
	================================================================
		WINDOWS SPECIFIC VSYNC CONTROL.. USING WGL
	================================================================
	*/
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
	RX_API PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

	/*
	================================================================
		WGL function used for AntiAliasing
	================================================================
	*/
	typedef BOOL (APIENTRY *PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC, const int *, const FLOAT *, UINT, int *, UINT *);
	RX_API PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 0;	

	/*
	================================================================
		ARB_vertex_buffer_object
	================================================================
	*/
	RX_API PFNGLBINDBUFFERARBPROC           glBindBufferARB           = NULL;
	RX_API PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB        = NULL;
	RX_API PFNGLGENBUFFERSARBPROC           glGenBuffersARB           = NULL;
	RX_API PFNGLISBUFFERARBPROC             glIsBufferARB             = NULL;
	RX_API PFNGLBUFFERDATAARBPROC           glBufferDataARB           = NULL;
	RX_API PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB        = NULL;
	RX_API PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB     = NULL;
	RX_API PFNGLMAPBUFFERARBPROC            glMapBufferARB            = NULL;
	RX_API PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB          = NULL;
	RX_API PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
	RX_API PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB    = NULL;

	RX_API PFNGLMULTIDRAWELEMENTSPROC	    glMultiDrawElements		  = NULL; //For glMultiDrawElements

	/*
	================================================================
		OpenGL Hardware occlusion query extensions
	================================================================
	*/
	RX_API PFNGLGENQUERIESARBPROC        glGenQueriesARB        = NULL;
	RX_API PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB     = NULL;
	RX_API PFNGLISQUERYARBPROC           glIsQueryARB           = NULL;
	RX_API PFNGLBEGINQUERYARBPROC        glBeginQueryARB        = NULL;
	RX_API PFNGLENDQUERYARBPROC          glEndQueryARB          = NULL;
	RX_API PFNGLGETQUERYIVARBPROC        glGetQueryivARB        = NULL;
	RX_API PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB  = NULL;
	RX_API PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB = NULL;

	/*
	================================================================
		OpenGL Frame buffer objects extensions
	================================================================
	*/
	RX_API PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = NULL;
	RX_API PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
	RX_API PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
	RX_API PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;
	RX_API PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
	RX_API PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
	RX_API PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = NULL;
	RX_API PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
	RX_API PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
	RX_API PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
	RX_API PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
	RX_API PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = NULL;
	RX_API PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
	RX_API PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = NULL;
	RX_API PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
	RX_API PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
	RX_API PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;
	RX_API PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;
	RX_API PFNGLDRAWBUFFERSPROC glReadBuffers = NULL;

	/*
	================================================================
		FOR MSAA in frame buffers 
	================================================================
	*/
	RX_API PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT = NULL;
	RX_API PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT = NULL;
	RX_API PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV = NULL;

	/* 
	================================================================
		For 3D Textures
	================================================================
	*/
	RX_API PFNGLTEXIMAGE3DPROC glTexImage3D = NULL;


	/* 
	================================================================
		For ARB point sprites extension
	================================================================
	*/
	RX_API PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB  = NULL;
	RX_API PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = NULL;

	/* 
	================================================================
		OpenGL Shader functions
	================================================================
	*/
	RX_API PFNGLCREATESHADEROBJECTARBPROC	glCreateShaderObjectARB = NULL;
	RX_API PFNGLSHADERSOURCEARBPROC			glShaderSourceARB = NULL;
	RX_API PFNGLCOMPILESHADERARBPROC		glCompileShaderARB = NULL;
	RX_API PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB = NULL;
	RX_API PFNGLATTACHOBJECTARBPROC			glAttachObjectARB = NULL;
	RX_API PFNGLLINKPROGRAMARBPROC			glLinkProgramARB = NULL;
	RX_API PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB = NULL;
	RX_API PFNGLUNIFORM1IARBPROC			glUniform1iARB = NULL;
	RX_API PFNGLUNIFORM1FARBPROC			glUniform1fARB = NULL;
	RX_API PFNGLUNIFORM2FARBPROC			glUniform2fARB = NULL;
	RX_API PFNGLUNIFORM3FARBPROC			glUniform3fARB = NULL;
	RX_API PFNGLUNIFORM4FARBPROC			glUniform4fARB = NULL;
	RX_API PFNGLGETUNIFORMLOCATIONARBPROC	glGetUniformLocationARB = NULL;
	RX_API PFNGLDETACHOBJECTARBPROC			glDetachObjectARB = NULL;
	RX_API PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB = NULL;
	RX_API PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB = NULL;
	RX_API PFNGLGETINFOLOGARBPROC			glGetInfoLogARB = NULL;

	//====================================

	void CoreEngine::Default_Constructor(void)
	{
		this->mAppObject.width = 800;								
		this->mAppObject.height = 600;								
		this->mAppObject.mColorBits = 24;	
		this->mAppObject.mRefreshRate = 60;	
		this->mAppObject.mStencilBits = 0;	
		this->mAppObject.mAlphaBits = 0;
		this->mAppObject.mDepthBits = 24;	
		
		this->mAppObject.mouseSensitivity = 0.01f;
		fov =  45.0f;

		zNear = 1.0;
		zFar = 1000.0;	
		
		//pause = false;	
		currentMenu = 0;
		
		this->lastBlurTime = 0.0f;
		this->mBlurQuality = HIGH_QUALITY;
		this->mfBlurAlpha = 0.8f;
		this->mfBlurRate = 0.02f;
		//this->active = false;
		this->mAppObject.volume = 1;
		this->mPresentID = 0;
		this->mbEnginePaused = false;
		this->bUseEngineSound = false;
		this->bUseOpenAL = true;	
		this->bMSAA = false;
		this->bCSAA = false;
		this->b3Dtexture = false;
		this->miHDRITextureFormat = GL_RGBA16F_ARB; //WARNING
		this->mbHDR = false;
		this->mLuaScriptEngine = 0;
		this->bPointSprites = false;
	}

	CoreEngine::CoreEngine(void)
	{
		this->Default_Constructor();
	}

	CoreEngine::~CoreEngine(void)
	{
	}

	void CoreEngine::useEngineSound(bool bOpenAL)
	{
		this->bUseEngineSound = true;
		this->bUseOpenAL = bOpenAL;
	}

	bool CoreEngine::usingEngineSound(void) const
	{
		return bUseEngineSound;
	}

	bool CoreEngine::isUsingOpenAL(void) const
	{
		return bUseOpenAL;
	}

	void CoreEngine::SetFullScreen(bool value)
	{
		if(this->mAppObject.m_bFullScreen != (int)value)
		{
			if(!this->mAppObject.m_bFullScreen)
			{
				ChangeDisplaySettings(NULL, 0);			
				this->mAppObject.m_bFullScreen = value;
			} else {
				this->mAppObject.ChangeToFullScreen();
				this->mAppObject.m_bFullScreen = value;
			}
		}	
	}

	void CoreEngine::InitGLGAME(HWND hWnd)
	{
		ilInit();
		ilutInit();	

		this->bVSYNCsupport = false;
		this->bVBO = false;
		this->bAntiAliasSupport = false;
		this->bShaderSupport = false;
		this->mAppObject.m_hWnd = hWnd;// Assign the window handle to our member window handle
		this->mbHDR = false;
		this->miHDRITextureFormat = GL_RGBA16F_ARB;

		if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
		{
				MessageBox(NULL, "Older version of DevIL detected...exiting!", "Error", MB_OK);
				mlog.LogError(LString("Older version for DevIL!"));
				this->getApp()->end();
				return;
		} 

		ilEnable(IL_FILE_OVERWRITE);
		
		GetClientRect(this->mAppObject.m_hWnd, &this->mAppObject.m_rRect);					// Assign the windows rectangle to a global RECT
		GetWindowRect(this->mAppObject.m_hWnd, &this->mAppObject.m_rWindowRect);
		InitializeOpenGL(this->mAppObject.m_rRect.right, this->mAppObject.m_rRect.bottom);	// Init OpenGL with the global rect

		/* Get Extensions and other System Information */
		this->QuerySystemInformation();
		this->glExtensions = (char*)glGetString(GL_EXTENSIONS);
		
		/* initialize our multitexturing functions */
		glActiveTextureARB		 = (PFNGLACTIVETEXTUREARBPROC)		    wglGetProcAddress("glActiveTextureARB");

		glMultiTexCoord2fARB	 = (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

		glMultiTexCoord4fARB	 = (PFNGLMULTITEXCOORD4FARBPROC)		wglGetProcAddress("glMultiTexCoord4fARB");
		glMultiTexCoord4fvARB	 = (PFNGLMULTITEXCOORD4FVARBPROC)		wglGetProcAddress("glMultiTexCoord4fvARB");

		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");

		this->bMultiTexture = true;
		
		if(!glActiveTextureARB || !glMultiTexCoord2fARB || !glClientActiveTextureARB)
		{
			this->bMultiTexture = false;
			mlog.LogInfo(LString("MultiTexturing is NOT available!"));
		}

		if(this->bMultiTexture)
		{
			mlog.LogInfo(LString("MultiTexturing is available!"));

			int maxTexUnits;

			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTexUnits);

			mlog.LogInfo(LString("Total Texture units available: %d", maxTexUnits));
		} else {
			mlog.LogInfo(LString("MultiTexturing is 'NOT' available!"));
		}

		this->InitializeVertexBufferObjects();
		this->InitializeVSYNC();
		this->InitializeHarwareOcclusionQuery();
		this->InitializeFrameBufferObjects();
		this->InitializeHDRExtensions();
		this->InitializePointSprite();
		
		this->mAppObject.windowPosition.x = 0;
		this->mAppObject.windowPosition.y = 0;
		currentMenu = 0;
		this->mAppObject.Mouse.mouseWheel = 0;

		RECT sysRect;
		GetWindowRect(GetDesktopWindow(), &sysRect);

		this->mAppObject.systemResolution.x = sysRect.right;
		this->mAppObject.systemResolution.y = sysRect.bottom;

		//Setup ilut to know the graphic adapters info
		//ilutRenderer(ILUT_OPENGL);	

		mlog.LogInfo(LString("System Resolution:"));
		mlog.LogInfo(LString("	Width: %d", this->mAppObject.systemResolution.x));
		mlog.LogInfo(LString("	Height: %d", this->mAppObject.systemResolution.y));

		this->mAppObject.frLastTime = 0;
		//pause = false;

		this->mLightHandler.Initialize();

		char temp[256];
		for(int  i = 0; i < RX_CUSTOM_MAX_LIGHTS; i++)
		{
			sprintf(temp, "light[%d].pos", i);
			this->mlog.bindFloatVariable(temp, &this->mLightHandler.light[i].pos.x, &this->mLightHandler.light[i].pos.y, &this->mLightHandler.light[i].pos.z);
			sprintf(temp, "light[%d].enabled", i);
			this->mlog.bindBooleanVariable(temp, &this->mLightHandler.light[i].bEnabled);
		}

		this->mTextureStateSystem = TextureStateHandler();

		mlog.LogSuccessText(LString("Lighting engine initialized!"));	
		
		this->mLuaScriptEngine = new LuaScriptEngine();
		if(!this->mLuaScriptEngine)
		{
			this->mlog.LogError(LString("Error initializing LUA scripting engine"));
		} else {
			this->mLuaScriptEngine->Initialize(true);
		}	

		this->mAppObject.m_fGameTime = 0.0f;
		this->mAppObject.m_fActualTime = 0.0f;
		this->mAppObject.m_ftRatio = 1.0f;
		this->lastBlurTime = 0.0f;
		this->mBlurQuality = HIGH_QUALITY;
		this->mfBlurAlpha = 0.8f;
		this->mfBlurRate = 0.02f;
		this->mbEnginePaused = false;

		//Max render targets query
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &this->iMaxRenderTargets);
		this->mlog.LogInfo(LString("Max Render Targets: %d", this->iMaxRenderTargets));

		glGetIntegerv(GL_MAX_SAMPLES_EXT, &this->iMaxSamples);
		this->mlog.LogInfo(LString("Max Samples for FBO: %d", this->iMaxSamples));

		this->mlog.LogInfo(LString("Creating SoundSystem sound object..."));

		this->mSoundSystem = 0;

		this->mSoundSystem = new SoundSystem;	

		if(!this->mSoundSystem)
		{
			this->mlog.LogError(LString("There was an error allocating memory for SoundSystem sound object!"));
			this->getApp()->end();
		}

		this->mlog.LogSuccessText(LString("Sound object CRxSoundSystem successfully created!"));
	}

	void CoreEngine::ForceSwapBuffers(void)
	{
		SwapBuffers(this->getApp()->getDC());
	}

	void CoreEngine::SetHDRITextureFormat(HDRI_Texture_formats format)
	{
		this->miHDRITextureFormat = format;
	}

	bool CoreEngine::isHDRSupported(void)
	{
		return this->mbHDR;
	}

	void CoreEngine::InitializeOpenGL(int width, int height) 
	{ 
		mlog.LogInfo(LString("Initializing OpenGL..."));
		this->mAppObject.m_hDC = GetDC(this->mAppObject.m_hWnd);								// This sets our global HDC
															// We don't free this hdc until the end of our program
		if (!this->mAppObject.bSetupPixelFormat(this->mAppObject.m_hDC))				// This sets our pixel format/information
		{
			mlog.LogError(LString("Error setting up of pixel format"));
			PostQuitMessage (0);							// If there's an error, quit		
			return;
		}

		mlog.LogSuccessText(LString("Setup of pixel format successful"));

		this->mAppObject.m_hRC = wglCreateContext(this->mAppObject.m_hDC);					// This creates a rendering context from our hdc
		wglMakeCurrent(this->mAppObject.m_hDC, this->mAppObject.m_hRC);						// This makes the rendering context we just created the one we want to use
		
		SizeOpenGLScreen(this->mAppObject.width, this->mAppObject.height);					// Setup the screen translations and viewport

		mlog.LogInfo(LString("Dimensions for OpenGL to use:"));
		mlog.LogInfo(LString("	Width: %d", this->mAppObject.width));
		mlog.LogInfo(LString("	Height: %d", this->mAppObject.height));
		mlog.LogInfo(LString("	Color bits: %d", this->mAppObject.mColorBits));
		mlog.LogInfo(LString("	Depth bits: %d", this->mAppObject.mDepthBits));
		mlog.LogInfo(LString("	Alpha bits: %d", this->mAppObject.mAlphaBits));
		mlog.LogInfo(LString("	Stencil bits: %d", this->mAppObject.mStencilBits));
		mlog.LogInfo(LString("	Refresh Rate: %d", this->mAppObject.mRefreshRate));
		mlog.LogInfo(LString("================="));
		
		this->mAppObject.windowPosition.x = 0;
		this->mAppObject.windowPosition.y = 0;

		//pause = false;

		this->mAppObject.m_fGameTime = 0.0f;
		this->mAppObject.m_fActualTime = 0.0f;
		this->mAppObject.m_ftRatio = 1.0f;

		mlog.LogSuccessText(LString("OpenGL Succesfully Initialized!"));
	}

	void CoreEngine::SizeOpenGLScreen(int w, int h)
	{
		if(h == 0)
			h = 1;

		glViewport(0, 0, w, h);	

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(this->fov, (float)w/(float)h, this->zNear, this->zFar);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	double CoreEngine::getZNear(void)
	{
		return this->zNear;
	}

	double CoreEngine::getZFar(void)
	{
		return this->zFar;
	}

	float CoreEngine::getFOV(void)
	{
		return this->fov;
	}

	void CoreEngine::ChangeFOV(float newFOV)
	{
		this->fov = newFOV;
		SizeOpenGLScreen(this->mAppObject.width, this->mAppObject.height);
	}

	void CoreEngine::IncreaseFOV(float df)
	{
		this->fov += df;
		SizeOpenGLScreen(this->mAppObject.width, this->mAppObject.height);
	}

	void CoreEngine::DecreaseFOV(float df)
	{
		this->fov -= df;
		SizeOpenGLScreen(this->mAppObject.width, this->mAppObject.height);
	}

	ILuint CoreEngine::GenerateSingleImage(void) const
	{
		ILuint ImageName; // The image name to return.
		ilGenImages(1, &ImageName); // Grab a new image name.
		return ImageName; // Go wild with the return value.
	} 

	void CoreEngine::DeleteSingleImage(ILuint ImageName) const
	{
		ilDeleteImages(1, &ImageName); // Delete the image name. 
		return;
	}

	void CoreEngine::DeInit()
	{			
		if(this->mLuaScriptEngine)
		{
			this->mLuaScriptEngine->Free();
			delete this->mLuaScriptEngine;
			this->mLuaScriptEngine = 0;
		}

		this->mSkyBox.Free();

		this->mAppObject.DestroyWindowObject();

		this->mAppObject.winMessage = false;

		ilShutDown();

		time_t curTime;
		time(& curTime);
		mlog.LogInfo( LString("<h3>RX Engine Log Closed</h3>") );
		mlog.LogInfo( LString("<b>Log Shutdown Time:</b> <u>%s</u>", ctime(&curTime)) );

		mlog.log.LogText(LString("</table><br><h2><font color = ffffff>Program closed Successfully from DeInit!</br></h2><hr></html>"));
		UnregisterClass(CLASSNAME, this->mAppObject.m_hInstance);
	}

	bool CoreEngine::SetupFromConfigurationFile(char* fileName)
	{
		const int size = 32;
		char holdingString[size];
		this->mAppObject.width = -1;
		short tempBoolToShort;

		std::ifstream fin(fileName);

		if(fin.fail())
		{
			return false;
		}

		fin>>holdingString>>this->mAppObject.m_bFullScreen;	
		fin>>holdingString>>this->mAppObject.width;
		if(this->mAppObject.width == -1) return false;
		fin>>holdingString>>this->mAppObject.height;
		fin>>holdingString>>this->mAppObject.mRefreshRate;
		fin>>holdingString>>this->mAppObject.mColorBits;
		fin>>holdingString>>this->mAppObject.mouseSensitivity;	
		fin>>holdingString>>this->mAppObject.volume;	
		fin>>holdingString>>tempBoolToShort;
		this->mAppObject.useMouse = (tempBoolToShort == 1);

		fin.close();

		mlog.LogSuccessText(LString("SetupFromConfigurationFile is SUCCESS!"));
		
		if(this->mAppObject.m_bFullScreen)
		{
			this->mAppObject.ChangeToFullScreen();
			this->mAppObject.dwStyle = WS_EX_TOPMOST | WS_CLIPSIBLINGS | WS_CLIPCHILDREN|WS_POPUP;
			mlog.LogInfo(LString("Fullscreen detected... not hiding cursor! You should hide cursor manually!"));	
		} else {
			this->mAppObject.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}

		return true;
	}

	bool CoreEngine::SaveScreenshot(const char* fileName, const char* format) const
	{
		glPushAttrib(GL_ENABLE_BIT);

		unsigned int tempshot = 0;
		char append[16];
		char extension[16];
		char final[256];

		FILE* fp = 0;

		do
		{
			if(fp)
			{
				fclose(fp);
			}

			memset(append, 0, sizeof(char)*16);
			memset(final, 0, sizeof(char)*256);
			sprintf(append, "_%d.", tempshot);
			sprintf(extension, "%s", format);
			strcpy(final, fileName);
			strcat(final, append);
			strcat(final, extension);
			fp = fopen(final, "rb");	
			tempshot++;			
		} while(fp);		

		int width = this->mAppObject.width;
		int height = this->mAppObject.height;

		//ilutRenderer(ILUT_OPENGL);	

		ILuint myImage;
		myImage = GenerateSingleImage();
		if(!ilIsImage(myImage))
		{
			return false;
		}

		ilBindImage(myImage); 	

		if(!ilutGLScreen())
			return false;	

		ilSaveImage(final); 

		DeleteSingleImage(myImage);

		glPopAttrib();	

		return true;
	}

	char* CoreEngine::GetExtensions(void)
	{
		return this->glExtensions;
	}

	bool CoreEngine::supportsMultiTexture(void)
	{
		return this->bMultiTexture;
	}


	void CoreEngine::QuerySystemInformation(void)
	{
		this->mySystem.strVendor = (char*)glGetString(GL_VENDOR);
		this->mySystem.strRenderer = (char*)glGetString(GL_RENDERER);
		this->mySystem.strGLVersion = (char*)glGetString(GL_VERSION);
		this->mySystem.glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);		
	}

	/* 
	================================================================
		Should be called only after QuerySystemInformation is called
	================================================================
	*/
	void CoreEngine::OutputSystemInformation(void)
	{		
		mlog.LogInfo(LString("========System Information========"));
		mlog.LogInfo(LString("System GPU Vendor: %s", this->mySystem.strVendor));
		mlog.LogInfo(LString("System Renderer: %s", this->mySystem.strRenderer));
		mlog.LogInfo(LString("System OpenGL Version: %s", this->mySystem.strGLVersion));
		mlog.LogInfo(LString("System GLSL Version: %s", this->mySystem.glslVersion));
		//mlog.OutInfo(LString("System OpenGL Supported extensions:\n\n %s\n", this->glExtensions));
		char* tempString;

		tempString = new char[8192];

		if(!tempString)
		{
			return;
		}

		sprintf(tempString, "System OpenGL Supported extensions:\n\n %s\n", this->glExtensions);
		mlog.OutInfo(tempString);
		mlog.LogInfo(LString("========End of System Information========\n"));

		delete[] tempString;
		tempString = 0;
	}

	TypeSystemInformation CoreEngine::getSystemInfo(void)
	{
		return this->mySystem;
	}

	void CoreEngine::Setup2DScreen(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, width, height, 0, 0, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void CoreEngine::SaveAndSetup2DScreen(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, width, height, 0, 0, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	void CoreEngine::RestoreOldScreen(void)
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void CoreEngine::Setup3DScreen(float fov, double zNear, double zFar)
	{
		this->fov = fov;	

		this->zFar = zFar;
		this->zNear = zNear;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	

		gluPerspective(this->fov, (float)this->mAppObject.getDimensions().x/(float)this->mAppObject.getDimensions().y, zNear, zFar);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void CoreEngine::SetupAppropriate3DScreen(void)
	{
		this->fov = fov;	

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	

		gluPerspective(this->fov, (float)this->mAppObject.getDimensions().x/(float)this->mAppObject.getDimensions().y, this->zNear, this->zFar);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void CoreEngine::Setup3DScreenView(float fov, float width, float height, double zNear, double zFar)
	{
		this->fov = fov;	

		this->zFar = zFar;
		this->zNear = zNear;

		glViewport(0, 0, (GLsizei)width, (GLsizei)height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	

		gluPerspective(this->fov, (float)width/(float)height, zNear, zFar);
		
		glMatrixMode(GL_MODELVIEW);
	}

	void CoreEngine::SetupCubeMapView(int cubeFace, const Math::Vector3 &objPos)
	{
		switch(cubeFace)
		{
			case 0: 
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 1:
				glRotatef( 90.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 2:
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
				break;
			case 3:
				glRotatef( 90.0f, 1.0f, 0.0f, 0.0f); 
				break;
			case 4: 
				glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
				break;
			case 5:
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;
		}

		glTranslatef(-objPos.x, -objPos.y, -objPos.z);
	}

	void CoreEngine::set(float fov, double zNear, double zFar, LPCTSTR programTitle)
	{		
		this->mAppObject.m_FrameInterval = 0;
		this->mAppObject.m_FPS = 0;
		this->mAppObject.m_fFrameIntervalSeconds = 0.0f;
		this->mAppObject.winMessage = true;		

		this->fov =  fov;
		
		this->zNear = zNear;
		this->zFar = zFar;
		this->mAppObject.frLastTime = 0;
		this->mAppObject.dwStyle = 0;
		this->mAppObject.tFPS = 0;
		//pause = false;
		
		this->mAppObject.windowPosition.x = 0;
		this->mAppObject.windowPosition.y = 0;
		currentMenu = 0;
		this->mAppObject.Mouse.mouseWheel = 0;
		this->mAppObject.mWindowName = programTitle;

		initRender = false;
		bMultiTexture = false;	
	}

	void CoreEngine::set(float fov, double zNear, double zFar, LPCTSTR programTitle, int x, int y)
	{		
		this->mAppObject.m_FrameInterval = 0;
		this->mAppObject.m_FPS = 0;
		this->mAppObject.m_fFrameIntervalSeconds = 0.0f;
		this->mAppObject.winMessage = true;		

		this->fov =  fov;
		
		this->zNear = zNear;
		this->zFar = zFar;
		this->mAppObject.frLastTime = 0;
		this->mAppObject.dwStyle = 0;
		this->mAppObject.tFPS = 0;
		//pause = false;
		
		this->mAppObject.windowPosition.x = x;
		this->mAppObject.windowPosition.y = y;
		currentMenu = 0;
		this->mAppObject.Mouse.mouseWheel = 0;
		this->mAppObject.mWindowName = programTitle;

		initRender = false;
		bMultiTexture = false;	
	}

	void CoreEngine::renderInitialized(void)
	{
		this->initRender = true;
		this->mlog.LogSuccessText(LString("InitRender success!"));
	}

	bool CoreEngine::isRenderInitialized(void)
	{
		return this->initRender;
	}

	void CoreEngine::SetupFogGame(float start, float end, int mode /* GL_LINEAR, GL_EXP, GL_EXP2 */,
								   float* fogColor, float density)
	{
		glFogi(GL_FOG_MODE, mode);
		glFogf(GL_FOG_START, start);
		glFogf(GL_FOG_END, end);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, density);
	}

	void CoreEngine::PrepareForParticleRender(void)
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDisable(GL_LIGHTING);
		glDepthMask(GL_FALSE);
	}

	void CoreEngine::EndParticleRender(void)
	{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	void CoreEngine::tempSetViewport(int w, int h)
	{
		glViewport(0, 0, w, h);
	}

	void CoreEngine::resetViewport(void)
	{
		glViewport(0, 0, this->mAppObject.width, this->mAppObject.height);
	}

	void CoreEngine::PrepareForCursorRender(void)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho(0, 1024, 0, 768, 0, 1.0);	

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);
	}

	void CoreEngine::EndCursorRender(void)
	{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void CoreEngine::DrawCursor(GLuint texture, float x, float y, float sx, float sy)
	{
		glPushMatrix();
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTranslatef(x/float(this->mAppObject.getRect().right)*1024, (1 - y/float(this->mAppObject.getRect().bottom))*768, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex2i((int)sx, (int)sy);
			glTexCoord2f(0, 1.0f); glVertex2i(0, (int)sy);
			glTexCoord2f(0, 0); glVertex2i(0, 0);
			glTexCoord2f(1.0f, 0); glVertex2i((int)sx, 0);				
		glEnd();

		//glDisable(GL_BLEND);
		glPopMatrix();
	}

	void CoreEngine::DrawCursorDirect(GLuint texture, float x, float y, float sx, float sy)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho(0, 1024, 0, 768, 0, 1.0);	

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTranslatef(x/float(this->mAppObject.getRect().right)*1024, (1 - y/float(this->mAppObject.getRect().bottom))*768, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex2i((int)sx, (int)sy);
			glTexCoord2f(0, 1.0f); glVertex2i(0, (int)sy);
			glTexCoord2f(0, 0); glVertex2i(0, 0);
			glTexCoord2f(1.0f, 0); glVertex2i((int)sx, 0);	
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDisable(GL_BLEND);
	}


	void CoreEngine::RenderMotionBlurTexture(void)
	{
		this->SaveAndSetup2DScreen(this->mBlurQuality, this->mBlurQuality);	
		
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glEnable(GL_BLEND);

		glColor4f(1, 1, 1, this->mfBlurAlpha);

		glBindTexture(GL_TEXTURE_2D, this->mBlurTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0, -1);	glVertex2i(0, this->mBlurQuality);
			glTexCoord2f(1, -1); glVertex2i(this->mBlurQuality, this->mBlurQuality);
			glTexCoord2f(1, 0);	glVertex2i(this->mBlurQuality, 0);
			glTexCoord2f(0, 0);	glVertex2i(0, 0);
		glEnd();		
		
		this->RestoreOldScreen();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	void CoreEngine::PrepareBlurTexture(void)
	{
		glGenTextures(1, (GLuint*)&this->mBlurTexture);								
		glBindTexture(GL_TEXTURE_2D, this->mBlurTexture);					
		
		// Create the texture and store it on the video card
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mBlurTexture, this->mBlurTexture, 0, GL_RGB, GL_UNSIGNED_INT, 0);						
		
		// Set the texture quality
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	void CoreEngine::InitMotionBlurSystem(GLuint quality, float alpha, float blurRate)
	{
		this->mBlurQuality = quality;
		this->mfBlurRate = blurRate;
		this->mfBlurAlpha = alpha;

		this->PrepareBlurTexture();
	}

	void CoreEngine::SetMotionBlurSystem(GLuint quality, float alpha, float blurRate)
	{
		this->mBlurQuality = quality;
		this->mfBlurRate = blurRate;
		this->mfBlurAlpha = alpha;
	}

	void CoreEngine::RenderMotionBlurToTexture(void sceneDraw())
	{
		float localBlurRate = (this->mfBlurRate==AUTO_BLUR_RATE)?this->mAppObject.m_fFrameIntervalSeconds:this->mfBlurRate;
		float currentTime = this->mAppObject.m_fGameTime;
		if( (currentTime - this->lastBlurTime) > localBlurRate )
		{
			this->lastBlurTime = currentTime;	

			glViewport(0, 0, this->mBlurQuality, this->mBlurQuality);		
			
			sceneDraw(); //Update the blur scene
			this->RenderMotionBlurTexture();						
		
			glBindTexture(GL_TEXTURE_2D, this->mBlurTexture);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, this->mBlurQuality, this->mBlurQuality, 0);

			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);	

			glViewport(0, 0, this->mAppObject.getRect().right, this->mAppObject.getRect().bottom);
		}	
	}

	void CoreEngine::RenderMotionBlurToScene(void sceneDraw())
	{
		float localBlurRate = (this->mfBlurRate==AUTO_BLUR_RATE)?this->mAppObject.m_fFrameIntervalSeconds:this->mfBlurRate;
		float currentTime = this->mAppObject.m_fGameTime;
		if( (currentTime - this->lastBlurTime) > localBlurRate )
		{
			this->lastBlurTime = currentTime;	

			glViewport(0, 0, this->mBlurQuality, this->mBlurQuality);		
			
			sceneDraw(); //Update the blur scene
			this->RenderMotionBlurTexture();						
		
			glBindTexture(GL_TEXTURE_2D, this->mBlurTexture);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, this->mBlurQuality, this->mBlurQuality, 0);

			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);	

			glViewport(0, 0, this->mAppObject.getRect().right, this->mAppObject.getRect().bottom);
		}

		this->RenderMotionBlurTexture();
	}

	/*
	bool CoreEngine::isActivated(void)
	{
		return this->active;
	}

	bool CoreEngine::UpdateGameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_ACTIVATE:							// Watch For Window Activate Message
			{
				if (!HIWORD(wParam))					// Check Minimization State
				{
					this->active=true;						// Program Is Active

					if(myGlobalGame->pause)
					{
						myGlobalGame->pause = false;
						myGlobalGame->mAppObject.Mouse.Hide();
						ResumeAllSounds();
					}
				}
				else
				{
					this->active=false;						// Program Is No Longer Active

					if(!myGlobalGame->pause)
					{
						myGlobalGame->pause = true;
						myGlobalGame->mAppObject.Mouse.Show();
						StopAllSounds();
					}
				}

				return false;								// Return To The Message Loop
			}
			case WM_SYSCOMMAND:							// Intercept System Commands	
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return false;							// Prevent From Happening
				}
				break;									// Exit
			case WM_CREATE:
				break;
			case WM_MOUSEMOVE:
				myGlobalGame->mAppObject.Mouse.x = LOWORD(lParam);
				myGlobalGame->mAppObject.Mouse.y = HIWORD(lParam);			
				break;
			case WM_KEYDOWN:
				this->mAppObject.characterReleased = wParam;
				myGlobalGame->mAppObject.mKeyWParam = wParam;
				myGlobalGame->mAppObject.mKeyLParam = lParam;				
				break;		
			case WM_SIZE:										// If the window is resized
				if(!myGlobalGame->mAppObject.isFullScreen())
				{
					myGlobalGame->SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
					GetClientRect(hWnd, &myGlobalGame->mAppObject.getRect());				// Get the window rectangle			
				}
				break; 
			case WM_MOVE:
				myGlobalGame->mAppObject.windowPosition.x = (int)LOWORD(lParam);
				myGlobalGame->mAppObject.windowPosition.y = (int)HIWORD(lParam);
				break;
			case WM_LBUTTONDOWN:	myGlobalGame->mAppObject.Mouse.LeftButton.SetState(MOUSE_DOWN);	break;
			case WM_LBUTTONUP:		myGlobalGame->mAppObject.Mouse.LeftButton.SetState(MOUSE_UP);	
									myGlobalGame->mAppObject.Mouse.LeftButton.SetRelease(true);
											break;
			case WM_RBUTTONDOWN:	myGlobalGame->mAppObject.Mouse.RightButton.SetState(MOUSE_DOWN);	break;
			case WM_RBUTTONUP:		myGlobalGame->mAppObject.Mouse.RightButton.SetState(MOUSE_UP);
									myGlobalGame->mAppObject.Mouse.RightButton.SetRelease(true);
											break;
			case WM_MBUTTONDOWN:	myGlobalGame->mAppObject.Mouse.MiddleButton.SetState(MOUSE_DOWN);	break;
			case WM_MBUTTONUP:		myGlobalGame->mAppObject.Mouse.MiddleButton.SetState(MOUSE_UP);
									myGlobalGame->mAppObject.Mouse.MiddleButton.SetRelease(true);
											break;
			case WM_MOUSEWHEEL:
				myGlobalGame->mAppObject.Mouse.mouseWheel = (short)HIWORD(wParam);
				break;
			/*
			case WM_DESTROY:
				this->mlog.LogSuccessText(LString("WM_DESTROY case confirmed"));
				myGlobalGame->mAppObject.end();
				return false;
				break;
			*//*
			case WM_CLOSE:
				this->mlog.LogSuccessText(LString("WM_CLOSE case confirmed"));
				myGlobalGame->mAppObject.end();
				return false;
				break;
			case WM_QUIT:
				this->mlog.LogSuccessText(LString("WM_QUIT case confirmed"));
				myGlobalGame->mAppObject.end();
				return false;
				break;
			default:		
				break;
		}

		return true;
	}*/

	inline Application* CoreEngine::getApp(void)
	{
		return &this->mAppObject;
	}

	bool CoreEngine::isOccluded(const Math::Vector3 &p, GLdouble* wx, GLdouble* wy, GLdouble* wz)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLfloat dbuf;

		gluProject(p.x, p.y, p.z, mvmat, projmat, viewport, wx, wy, wz);

		glReadPixels((int)*wx, (int)*wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dbuf);

		if(dbuf < *wz)
		{
			return true;
		}

		return false;
	}

	bool CoreEngine::isScreenPointOccluded(GLdouble wx, GLdouble wy, GLdouble wz)
	{
		GLfloat dbuf;

		glReadPixels((int)wx, (int)wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dbuf);

		if(dbuf < wz)
		{
			return true;
		}

		return false;
	}

	/*
	GLuint CoreEngine::getSelectedObject(int x, int y, int w, int h)
	{
		int objectsFound = 0;							
		int	viewportCoords[4] = {0};				

		// This will hold the ID's of the objects we click on.
		// We make it an arbitrary number of 32 because openGL also stores other information
		// that we don't care about.  There is about 4 slots of info for every object ID taken up.
		unsigned int selectBuffer[32] = {0};				
															
		// glSelectBuffer is what we register our selection buffer with.  The first parameter
		// is the size of our array.  The next parameter is the buffer to store the information found.
		// More information on the information that will be stored in selectBuffer is further below.

		glSelectBuffer(32, selectBuffer);					// Setup our selection buffer to accept object ID's

		// This function returns information about many things in OpenGL.  We pass in GL_VIEWPORT
		// to get the view port coordinates.  It saves it like a RECT with {top, left, bottom, right}

		glGetIntegerv(GL_VIEWPORT, viewportCoords);			// Get the current view port coordinates

		// Now we want to get out of our GL_MODELVIEW matrix and start effecting our
		// GL_PROJECTION matrix.  This allows us to check our X and Y coords against 3D space.

		glMatrixMode(GL_PROJECTION);						// We want to now effect our projection matrix
		
		glPushMatrix();										// We push on a new matrix so we don't effect our 3D projection

			// This makes it so it doesn't change the frame buffer if we render into it, instead, 
			// a record of the names of primitives that would have been drawn if the render mode was
			// GL_RENDER are now stored in the selection array (selectBuffer).

			glRenderMode(GL_SELECT);						// Allows us to render the objects, but not change the frame buffer

			glLoadIdentity();								// Reset our projection matrix

			// gluPickMatrix allows us to create a projection matrix that is around our
			// cursor.  This basically only allows rendering in the region that we specify.
			// If an object is rendered into that region, then it saves that objects ID for us (The magic).
			// The first 2 parameters are the X and Y position to start from, then the next 2
			// are the width and height of the region from the starting point.  The last parameter is
			// of course our view port coordinates.  You will notice we subtract "y" from the
			// BOTTOM view port coordinate.  We do this to flip the Y coordinates around.  The 0 y
			// coordinate starts from the bottom, which is opposite to window's coordinates.
			// We also give a 2 by 2 region to look for an object in.  This can be changed to preference.

			gluPickMatrix(x, viewportCoords[3] - y, 2, 2, viewportCoords);

			// Next, we just call our normal gluPerspective() function, exactly as we did on startup.
			// This is to multiply the perspective matrix by the pick matrix we created up above. 

			gluPerspective(this->fov, (float)w/(float)h, this->zNear, this->zFar);
			
			glMatrixMode(GL_MODELVIEW);						// Go back into our model view matrix
		
			RenderScene();									// Now we render into our selective mode to pinpoint clicked objects

			// If we return to our normal render mode from select mode, glRenderMode returns
			// the number of objects that were found in our specified region (specified in gluPickMatrix())

			objectsFound = glRenderMode(GL_RENDER);			// Return to render mode and get the number of objects found

			glMatrixMode(GL_PROJECTION);					// Put our projection matrix back to normal.
		glPopMatrix();										// Stop effecting our projection matrix

		glMatrixMode(GL_MODELVIEW);							// Go back to our normal model view matrix

		// PHEW!  That was some stuff confusing stuff.  Now we are out of the clear and should have
		// an ID of the object we clicked on.  objectsFound should be at least 1 if we found an object.

		if (objectsFound > 0)
		{		
			// If we found more than one object, we need to check the depth values
			// of all the objects found.  The object with the LEAST depth value is
			// the closest object that we clicked on.  Depending on what you are doing,
			// you might want ALL the objects that you clicked on (if some objects were
			// behind the closest one), but for this tutorial we just care about the one
			// in front.  So, how do we get the depth value?  Well, The selectionBuffer
			// holds it.  For every object there is 4 values.  The first value is
			// "the number of names in the name stack at the time of the event, followed 
			// by the minimum and maximum depth values of all vertices that hit since the 
			// previous event, then followed by the name stack contents, bottom name first." - MSDN
			// The only ones we care about are the minimum depth value (the second value) and
			// the object ID that was passed into glLoadName() (This is the fourth value).
			// So, [0 - 3] is the first object's data, [4 - 7] is the second object's data, etc...
			// Be carefull though, because if you are displaying 2D text in front, it will
			// always find that as the lowest object.  So make sure you disable text when
			// rendering the screen for the object test.  I use a flag for RenderScene().
			// So, lets get the object with the lowest depth!		

			// Set the lowest depth to the first object to start it off.
			// 1 is the first object's minimum Z value.
			// We use an unsigned int so we don't get a warning with selectBuffer below.
			unsigned int lowestDepth = selectBuffer[1];

			// Set the selected object to the first object to start it off.
			// 3 is the first object's object ID we passed into glLoadName().
			int selectedObject = selectBuffer[3];

			// Go through all of the objects found, but start at the second one
			for(int i = 1; i < objectsFound; i++)
			{
				// Check if the current objects depth is lower than the current lowest
				// Notice we times i by 4 (4 values for each object) and add 1 for the depth.
				if(selectBuffer[(i * 4) + 1] < lowestDepth)
				{
					// Set the current lowest depth
					lowestDepth = selectBuffer[(i * 4) + 1];

					// Set the current object ID
					selectedObject = selectBuffer[(i * 4) + 3];
				}
			}
			
			// Return the selected object
			return selectedObject;
		}

		// We didn't click on any objects so return 0
		return 0;

	}*/

	GLuint CoreEngine::createObject(void)
	{
		return ++this->mPresentID;
	}

	Math::Vector3 CoreEngine::projectPoint(const Math::Vector3 &point)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLdouble wx, wy, wz;
		//GLfloat dbuf;
		Math::Vector3 out = Math::Vector3(0, 0, 0);

		gluProject(point.x, point.y, point.z, mvmat, projmat, viewport, &wx, &wy, &wz);
		
		out.x = (float) wx;
		out.y = (float) wy;
		out.z = (float) wz;

		return out;
	}

	Math::Vector3 CoreEngine::unProjectPoint(const Math::Vector3 &point)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLdouble wx, wy, wz;
		//GLfloat dbuf;
		Math::Vector3 out = Math::Vector3(0, 0, 0);

		gluUnProject((GLdouble)point.x, (GLdouble)point.y, (GLdouble)point.z, mvmat, projmat, viewport, &wx, &wy, &wz);
		
		out.x = (float) wx;
		out.y = (float) wy;
		out.z = (float) wz;

		return out;
	}

	Math::Vector3 CoreEngine::unProjectPointFlipY(const Math::Vector3 &point)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLdouble wx, wy, wz;
		//GLfloat dbuf;
		Math::Vector3 out = Math::Vector3(0, 0, 0);

		gluUnProject((GLdouble)point.x, (GLdouble)(viewport[3] - point.y - 1), (GLdouble)point.z, mvmat, projmat, viewport, &wx, &wy, &wz);
		
		out.x = (float) wx;
		out.y = (float) wy;
		out.z = (float) wz;

		return out;
	}

	bool CoreEngine::InitializeVertexBufferObjects()
	{
		if( strstr( this->glExtensions, "ARB_vertex_buffer_object" ) == NULL )
		{
			this->bVBO = false;
			this->mlog.LogError(LString("Vertex buffer objects not supported!"));
			return false;
		}
		else
		{
			glBindBufferARB           = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
			glDeleteBuffersARB        = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
			glGenBuffersARB           = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
			glIsBufferARB             = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
			glBufferDataARB           = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
			glBufferSubDataARB        = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
			glGetBufferSubDataARB     = (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress("glGetBufferSubDataARB");
			glMapBufferARB            = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
			glUnmapBufferARB          = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
			glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
			glGetBufferPointervARB    = (PFNGLGETBUFFERPOINTERVARBPROC)wglGetProcAddress("glGetBufferPointervARB");

			glMultiDrawElements		  = (PFNGLMULTIDRAWELEMENTSPROC)wglGetProcAddress("glMultiDrawElements");

			if( !glBindBufferARB || !glDeleteBuffersARB || !glGenBuffersARB || 
				!glIsBufferARB || !glBufferDataARB || !glBufferSubDataARB || 
				!glGetBufferSubDataARB || !glMapBufferARB || !glUnmapBufferARB || 
				!glGetBufferParameterivARB || !glGetBufferPointervARB )
			{
				this->bVBO = false;
				this->mlog.LogError(LString("One or more ARB_vertex_buffer_object functions were not created.. VBO may not function"));
				return false;
			}
			
			if(!glMultiDrawElements)
			{
				this->bVBO = false;
				this->mlog.LogError(LString("VBO class needs glMultiDrawElements, which is not supported"));
				return false;
			}
		}

		this->bVBO = true;
		this->mlog.LogSuccessText(LString("VBO is successfully initialized using ARB_vertex_buffer_object"));
		return true;
	}

	bool CoreEngine::InitializeFrameBufferObjects()
	{
		if( strstr( this->glExtensions, "GL_EXT_framebuffer_object" ) == NULL )
		{
			this->bFBO = false;
			this->mlog.LogError(LString("Frame buffer objects not supported!"));
			return false;
		}
		else
		{
			glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
			glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
			glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
			glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
			glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
			glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
			glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
			glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
			glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
			glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
			glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
			glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
			glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
			glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
			glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
			glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
			glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
			glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");

			if( !glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT || 
				!glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT || 
				!glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT || 
				!glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT || 
				!glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT||  
				!glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT || !glDrawBuffers)
			{
				this->bFBO = false;
				this->mlog.LogError(LString("One or more EXT_framebuffer_object functions were not created.. FBO may not function"));
				return false;
			}
		}

		this->bFBO = true;
		this->mlog.LogSuccessText(LString("FBO is successfully initialized using EXT_framebuffer_object"));
		return true;
	}

	bool CoreEngine::InitializePointSprite(void)
	{
		if( strstr( this->glExtensions, "GL_ARB_point_parameters" ) == NULL )
		{
			this->bPointSprites = false;
			this->mlog.LogError(LString("ARB Point sprite extension not supported!"));
			return false;
		} else {
			glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
			glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

			if( !glPointParameterfARB || !glPointParameterfvARB )
			{
				this->bPointSprites = false;
				this->mlog.LogError(LString("One or more GL_ARB_point_sprite functions were not created.. Point sprite may not function"));
				return false;
			}
		}

		this->bPointSprites = true;
		this->mlog.LogSuccessText(LString("GL_ARB_point_sprite extension successfully initialized!"));
		return true;
	}

	bool CoreEngine::isPointSpriteSupported(void) const
	{
		return this->bPointSprites;
	}

	bool CoreEngine::InitializeMSAA(void)
	{

		if( strstr( this->glExtensions, "GL_EXT_framebuffer_blit" ) == NULL )
		{
			this->bMSAA = false;
			this->mlog.LogError(LString("GL_EXT_framebuffer_blit not supported! MSAA Init failed!"));
			return false;
		}

		if( strstr( this->glExtensions, "GL_EXT_framebuffer_multisample" ) == NULL )
		{
			this->bMSAA = false;
			this->mlog.LogError(LString("GL_EXT_framebuffer_multisample not supported! MSAA Init failed!"));
			return false;
		}

		glBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXTPROC)wglGetProcAddress("glBlitFramebufferEXT");	
		glRenderbufferStorageMultisampleEXT = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)wglGetProcAddress("glRenderbufferStorageMultisampleEXT");

		if( !glBlitFramebufferEXT || !glRenderbufferStorageMultisampleEXT)
		{
			this->bMSAA = false;
			this->mlog.LogError(LString("One or more GL_EXT_framebuffer_multisample functions were not created.. MSAA may not function"));
			return false;
		}

		this->bMSAA = true;

		this->mlog.LogSuccessText(LString("MSAA is successfully initialized using GL_EXT_framebuffer_multisample"));

		return true;
	}

	bool CoreEngine::InitializeCSAA(void)
	{
		if( strstr( this->glExtensions, "GL_NV_framebuffer_multisample_coverage" ) == NULL )
		{
			this->bCSAA = false;
			this->mlog.LogError(LString("GL_NV_framebuffer_multisample_coverage not supported! CSAA Init failed!"));
			return false;
		}

		glRenderbufferStorageMultisampleCoverageNV = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC)wglGetProcAddress("glRenderbufferStorageMultisampleCoverageNV");

		if( !glRenderbufferStorageMultisampleCoverageNV)
		{
			this->bCSAA = false;
			this->mlog.LogError(LString("One or more GL_NV_framebuffer_multisample_coverage functions were not created.. CSAA may not function"));
			return false;
		}

		this->bCSAA = true;

		this->mlog.LogSuccessText(LString("CSAA is successfully initialized using GL_NV_framebuffer_multisample_coverage"));

		return true;
	}

	bool CoreEngine::Initialize3DTextures(void)
	{
		if( strstr( this->glExtensions, "GL_EXT_texture3D" ) == NULL )
		{
			this->b3Dtexture = false;
			this->mlog.LogError(LString("GL_EXT_texture3D not supported! 3D Textures Init failed!"));
			return false;
		}

		glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");

		this->b3Dtexture = true;

		this->mlog.LogSuccessText(LString("3D textures support is successfully initialized!"));

		return true;
	}

	bool CoreEngine::is3DTexturesSupported(void)
	{
		return this->b3Dtexture;
	}

	bool CoreEngine::isVBOSupported()
	{
		return this->bVBO;
	}

	bool CoreEngine::isMSAASupported()
	{
		return this->bMSAA;
	}

	bool CoreEngine::isCSAASupported()
	{
		return this->bCSAA;
	}

	bool CoreEngine::isFBOSupported()
	{
		return this->bFBO;
	}

	/*
	================================================================
			Display list creation helpers
	================================================================
	*/
	void CoreEngine::createDisplayList(bool execute, DisplayList* display)
	{
		(*display) = glGenLists(1);

		if(execute == true)
		{
			glNewList((*display), GL_COMPILE_AND_EXECUTE);
		} else {
			glNewList((*display), GL_COMPILE);
		}
	}

	void CoreEngine::endDisplayList(void)
	{
		glEndList();
	}

	void CoreEngine::callDisplayList(DisplayList* display)
	{
		glCallList((*display));
	}

	void CoreEngine::deleteDisplayList(DisplayList* display)
	{
		glDeleteLists((*display), 1);
	}

	void CoreEngine::deleteDisplayLists(DisplayList* display, GLuint range)
	{
		glDeleteLists((*display), range);
	}


	/*
	================================================================
			Lighting Helpers
	================================================================
	*/
	void CoreEngine::EnableLighting(void)
	{
		glEnable(GL_LIGHTING);
	}

	void CoreEngine::DisableLighting(void)
	{
		glDisable(GL_LIGHTING);
	}

	/*
	================================================================
			Vsynce control, depends on windows.. wgl
	================================================================
	*/
	void CoreEngine::InitializeVSYNC(void)
	{
		  if( strstr( this->glExtensions, "WGL_EXT_swap_control" ) == 0 )
		  {
			  this->bVSYNCsupport = false;
			  this->mlog.LogError(LString("WGL_EXT_swap_control NOT FOUND!, VSYNC NOT SUPPORTED"));
			  return; 
		  }
		  else
		  {
				wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

				if(!wglSwapIntervalEXT)
				{
					this->bVSYNCsupport = false;
					this->mlog.LogError(LString("WGL_EXT_swap_control FOUND but error in finding Proc address for wglSwapIntervalEXT"));
				}

				this->bVSYNCsupport = true;
				this->mlog.LogSuccessText(LString("VSYNC IS SUPPORTED!"));
		  }
	}

	bool CoreEngine::InitializeHDRExtensions(void)
	{
		if( strstr( this->glExtensions, "GL_ARB_texture_float" ) == 0 )
		{
			this->mbHDR = false;
			this->mlog.LogError(LString("GL_ARB_texture_float NOT FOUND!, HDR TEXTURES NOT SUPPORTED"));
			return false; 
		}

		if( strstr( this->glExtensions, "GL_ARB_color_buffer_float" ) == 0 )
		{
			this->mbHDR = false;
			this->mlog.LogError(LString("GL_ARB_color_buffer_float NOT FOUND!, HDR NOT SUPPORTED"));
			return false; 
		}

		this->mbHDR = true;
		this->mlog.LogSuccessText(LString("HDR is supported!"));
		return true;
	}

	GLuint CoreEngine::getHDRITextureFormat(void)
	{
		return this->miHDRITextureFormat;
	}

	void CoreEngine::EnableVSYNC(void)
	{
		if(this->bVSYNCsupport)
		{
			wglSwapIntervalEXT(1);
		}
	}

	void CoreEngine::DisableVSYNC(void)
	{
		if(this->bVSYNCsupport)
		{
			wglSwapIntervalEXT(0);
		}
	}

	/*
	================================================================
			Multisample Functions (WARNING: WINDOWS SPECIFIC)
	================================================================
	*/
	bool CoreEngine::InitializeAntialiasing(int samples)
	{
		this->mlog.LogSuccessText(LString("AntiAliasing/MultiSampling is being initialized..."));
		this->bAntiAliasSupport = false;

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB"); /* Go get that function pointer. */

		int attributes[] = { /* Basic attributes with multi-sampling support. */
		  WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		  WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		  WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		  WGL_COLOR_BITS_ARB, this->mAppObject.mColorBits,
		  WGL_ALPHA_BITS_ARB, this->mAppObject.mAlphaBits,
		  WGL_DEPTH_BITS_ARB, this->mAppObject.mDepthBits,
		  WGL_STENCIL_BITS_ARB, this->mAppObject.mStencilBits,
		  WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		  WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		  WGL_SAMPLES_ARB, samples, // Can also be set to 2.
		  0,
		};

		int pf = 0; unsigned int n = 0;

		if (wglChoosePixelFormatARB != NULL)
		{
			BOOL bResult = wglChoosePixelFormatARB(this->mAppObject.m_hDC, attributes, NULL, 1, &pf, &n);

			if (bResult != FALSE && n > 0)
			{
				this->mAppObject.DestroyWindowObject();

				if(this->mAppObject.m_bFullScreen)
				{
					this->mAppObject.ChangeToFullScreen();
					this->mAppObject.dwStyle = WS_EX_TOPMOST | WS_CLIPSIBLINGS | WS_CLIPCHILDREN|WS_POPUP;
					mlog.LogInfo(LString("Fullscreen detected... not hiding cursor! You must hide cursor manually!"));	
				} else {
					this->mAppObject.dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
				}
				
				this->mAppObject.m_hWnd = this->mAppObject.createWindow();	
				this->mAppObject.InitializeFromPixelFormatForMultiSample(pf);

				SizeOpenGLScreen(this->mAppObject.width, this->mAppObject.height);

				this->bAntiAliasSupport = true;
				this->mlog.LogSuccessText(LString("AntiAliasing/MultiSampling is supported!"));
			}
		}

		if (this->bAntiAliasSupport == false)
		{
			this->mlog.LogError(LString("Sorry, no suitable pixel format was available supporting hardware AntiAliasing/MultiSampling"));
			return false;
		}	

		ShowWindow(this->mAppObject.getHandle(), SW_SHOWNORMAL);

		return true;
	}

	void CoreEngine::EnableAntialiasing(void)
	{
		glEnable(GL_MULTISAMPLE_ARB);
	}

	void CoreEngine::DisableAntialiasing(void)
	{
		glDisable(GL_MULTISAMPLE_ARB);
	}

	bool CoreEngine::isAntiAliasingSupported(void)
	{
		return this->bAntiAliasSupport;
	}

	/*
	================================================================
		CG SHADER SUPPORT CODE
	================================================================
	*/

	bool CoreEngine::InitializeShaders(bool bSupportCGFX,
										bool debug,
										EnumVertexShaderProfile vsProfile,
										EnumFragmentShaderProfile psProfile, 
										bool bGLSL)
	{
		if(bGLSL)
		{
			// Make sure we support the GLSL shading language 1.0
			if(!strstr(this->glExtensions, "GL_ARB_shading_language_100"))
			{
				//MessageBox(g_hWnd, "GL_ARB_shading_language_100 extension not supported!", "Error", MB_OK);
				//return false;
				this->mlog.LogError(LString("Failed to initialize GLSL. Shaders not supported!"));

				return false;
			}

			// Now let's set all of our function pointers for our extension functions
			glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
			glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
			glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
			glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
			glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
			glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
			glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
			glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
			glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
			glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
			glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
			glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
			glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
			glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress("glDetachObjectARB");
			glDeleteObjectARB  = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
			glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
			glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
		}

		//Vertex profile mappings
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumVertexShaderProfile::ARBVP1, "ARBVP1"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumVertexShaderProfile::GLSLV, "GLSLV"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumVertexShaderProfile::VP20, "VP20"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumVertexShaderProfile::VP30, "VP30"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumVertexShaderProfile::VP40, "VP40"));

		//Fragment profile mappings
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumFragmentShaderProfile::ARBFP1, "ARBFP1"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumFragmentShaderProfile::GLSLF, "GLSLF"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumFragmentShaderProfile::FP20, "FP20"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumFragmentShaderProfile::FP30, "FP30"));
		mCGProfileMapping.insert(ShaderProfilePair((int)EnumFragmentShaderProfile::FP40, "FP40"));

		this->bShaderSupport = false;
		this->mlog.LogInfo(LString("Initializing Shader system..."));

		//
		// Search for a valid vertex shader profile in this order:
		//
		// CG_PROFILE_ARBVP1 - GL_ARB_vertex_program
		// CG_PROFILE_VP40   - GL_ARB_vertex_program + GL_NV_vertex_program3
		//
		
		if( cgGLIsProfileSupported((CGprofile)vsProfile) )
		{
			this->mlog.LogInfo(LString("Using Cg Vertex Program Profile %s", mCGProfileMapping[(int)vsProfile].c_str()));
			mCGprofile_vertex = (CGprofile)vsProfile;
		}else {
			this->mlog.LogError(LString("Failed to initialize vertex shader! Hardware doesn't support any of the required vertex shading extensions!"));

			return false;
		}

	//	cgGLSetOptimalOptions(mCGprofile_vertex);

		//
		// Search for a valid pixel shader profile in this order:
		//
		// CG_PROFILE_ARBFP1 - GL_ARB_fragment_program
		// CG_PROFILE_FP30   - GL_NV_fragment_program
		// CG_PROFILE_FP20   - NV_texture_shader & NV_register_combiners
		//
		if( cgGLIsProfileSupported((CGprofile)psProfile) ) 
		{
			this->mlog.LogInfo(LString("Using Cg Pixel Program Profile %s", mCGProfileMapping[(int)psProfile].c_str()));
			mCGprofile_pixel = (CGprofile)psProfile;
		} else {
			this->mlog.LogError(LString("Failed to initialize pixel shader! Hardware doesn't support any of the required pixel shading extensions!"));

			return false;
		}

		/*
		cgGLSetOptimalOptions(mCGprofile_vertex);
		checkForCgError("Setting optimal settings for vertex profile");

		cgGLSetOptimalOptions(mCGprofile_pixel);
		checkForCgError("Setting optimal settings for pixel profile");
		*/

		// Create the context...
		this->mCGcontext = cgCreateContext();

		if(!this->mCGcontext)
		{
			this->mlog.LogError(LString("Cg Shader context failed to create!"));
			return false;
		}

		if(bSupportCGFX)
		{
			this->bCGFX = true;
			cgGLRegisterStates(mCGcontext);
			this->mlog.LogInfo(LString("CgFX OpenGL state manager bound to Cg Context"));
		} else {
			this->bCGFX = false;
			this->mlog.LogInfo(LString("CgFX OpenGL state manager NOT bound to Cg Context"));
		}

		checkForCgError("Creating context...");

		cgGLSetDebugMode( (debug == true)? CG_TRUE : CG_FALSE );

		this->bShaderSupport = true;

		this->mlog.LogSuccessText(LString("Shaders successfully initialized"));

		return this->bShaderSupport;
	}

	bool CoreEngine::isVertexProfileSupported(CoreEngine::EnumVertexShaderProfile vsProfile)
	{
		std::string vsProfileString = mCGProfileMapping[(int)vsProfile]; 

		this->mlog.LogInfo(LString("Checking if vertex profile %s is supported...", vsProfileString.c_str()));

		if( cgGLIsProfileSupported((CGprofile)vsProfile) )
		{
			this->mlog.LogSuccessText(LString("Vertex profile %s is supported!", vsProfileString.c_str()));

			return true;
		}

		this->mlog.LogError(LString("Vertex profile %s is not supported!", vsProfileString.c_str()));

		return false;
	}

	bool CoreEngine::isFragmentProfileSupported(CoreEngine::EnumFragmentShaderProfile fsProfile)
	{
		std::string fsProfileString = mCGProfileMapping[(int)fsProfile]; 

		this->mlog.LogInfo(LString("Checking if fragment profile %s is supported...", fsProfileString.c_str()));

		if( cgGLIsProfileSupported((CGprofile)fsProfile) )
		{
			this->mlog.LogSuccessText(LString("Fragment profile %s is supported!", fsProfileString.c_str()));

			return true;
		}

		this->mlog.LogError(LString("Fragment profile %s is not supported!", fsProfileString.c_str()));

		return false;
	}

	void CoreEngine::DeInitializeShaders(void)
	{
		cgDestroyContext(this->mCGcontext);
		this->mlog.LogSuccessText(LString("Shaders successfully De-initialized"));
	}

	void CoreEngine::checkForCgError(const char *situation)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR)
		{
			this->mlog.LogError(LString("%s: %s: %s\n", this->getApp()->getWindowName(), situation, string));

			if (error == CG_COMPILER_ERROR)
			{
				this->mlog.LogError(LString("%s\n", cgGetLastListing(this->mCGcontext)));
			}

			//exit(1);
		}
	}

	CGprofile CoreEngine::getCGVertexProfile(void)
	{
		return this->mCGprofile_vertex;
	}

	CGprofile CoreEngine::getCGPixelProfile(void)
	{
		return this->mCGprofile_pixel;
	}

	CGcontext CoreEngine::getCGContext(void)
	{
		return this->mCGcontext;
	}

	__forceinline void CoreEngine::EnableVertexProfile(CGprofile vsProfile)
	{
		this->mCGprofile_vertex = vsProfile;
		cgGLEnableProfile( this->mCGprofile_vertex);
	}

	__forceinline void CoreEngine::EnablePixelProfile(CGprofile fsProfile)
	{
		this->mCGprofile_pixel = fsProfile;
		cgGLEnableProfile( this->mCGprofile_pixel);
	}

	__forceinline void CoreEngine::EnableShaderProfiles(void)
	{
		cgGLEnableProfile( this->mCGprofile_vertex);
		cgGLEnableProfile( this->mCGprofile_pixel);
	}

	__forceinline void CoreEngine::DisableVertexProfile(void)
	{
		cgGLDisableProfile( this->mCGprofile_vertex);
	}

	__forceinline void CoreEngine::DisablePixelProfile(void)
	{
		cgGLDisableProfile( this->mCGprofile_pixel);
	}

	__forceinline void CoreEngine::DisableShaderProfiles(void)
	{
		cgGLDisableProfile( this->mCGprofile_vertex);
		cgGLDisableProfile( this->mCGprofile_pixel);
	}

	__forceinline void CoreEngine::UnBindLastVertexProgram(void)
	{
		cgGLUnbindProgram( this->mCGprofile_vertex );
	}

	__forceinline void CoreEngine::UnBindLastPixelProgram(void)
	{
		cgGLUnbindProgram( this->mCGprofile_pixel );
	}

	__forceinline void CoreEngine::UnBindAllOfLastPrograms(void)
	{
		this->UnBindLastVertexProgram();
		this->UnBindLastPixelProgram();
	}

	bool CoreEngine::InitializeHarwareOcclusionQuery(void)
	{
		if( strstr( this->glExtensions, "GL_ARB_occlusion_query" ) == NULL )
		{
			this->bOcclusionQuery = false;
			this->mlog.LogError(LString("Hardware Occlusion queries not supported!"));
			return false;
		}
		else
		{
			glGenQueriesARB        = (PFNGLGENQUERIESARBPROC)wglGetProcAddress("glGenQueriesARB");
			glDeleteQueriesARB     = (PFNGLDELETEQUERIESARBPROC)wglGetProcAddress("glDeleteQueriesARB");
			glIsQueryARB           = (PFNGLISQUERYARBPROC)wglGetProcAddress("glIsQueryARB");
			glBeginQueryARB        = (PFNGLBEGINQUERYARBPROC)wglGetProcAddress("glBeginQueryARB");
			glEndQueryARB          = (PFNGLENDQUERYARBPROC)wglGetProcAddress("glEndQueryARB");
			glGetQueryivARB        = (PFNGLGETQUERYIVARBPROC)wglGetProcAddress("glGetQueryivARB");
			glGetQueryObjectivARB  = (PFNGLGETQUERYOBJECTIVARBPROC)wglGetProcAddress("glGetQueryObjectivARB");
			glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)wglGetProcAddress("glGetQueryObjectuivARB");

			if( !glGenQueriesARB  || !glDeleteQueriesARB || !glIsQueryARB || 
				!glBeginQueryARB || !glEndQueryARB || !glGetQueryivARB ||
				!glGetQueryObjectivARB || !glGetQueryObjectuivARB )
			{
				this->bOcclusionQuery = false;
				this->mlog.LogError(LString("One or more GL_ARB_occlusion_query functions were not created.. Hardware occlusion query may not function"));
				return false;
			}

		}

		this->bOcclusionQuery = true;
		this->mlog.LogSuccessText(LString("Hardware Occlusion Query is successfully initialized using GL_ARB_occlusion_query"));
		return true;
	}

	bool CoreEngine::isUsingCGFX(void)
	{
		return this->bCGFX;
	}

	/*	
	================================================================
			Harware occlusion query object helper
	================================================================
	*/

	void CoreEngine::HardwareQueryObject::CreateQueryObject(GLuint *pInt)
	{
		glGenQueriesARB( 1, pInt );
	}

	void CoreEngine::HardwareQueryObject::DeleteQueryObject(GLuint *pInt)
	{
		glDeleteQueriesARB( 1, pInt );
	}

	void CoreEngine::HardwareQueryObject::GetFragmentCount(GLuint* queryObject, GLuint* fragments)
	{
		glGetQueryObjectuivARB( (*queryObject), GL_QUERY_RESULT_ARB, fragments);
	}

	void CoreEngine::HardwareQueryObject::BeginOcclusionQuery(GLuint* queryObject)
	{
		glBeginQueryARB( GL_SAMPLES_PASSED_ARB, (*queryObject) );
	}

	void CoreEngine::HardwareQueryObject::EndOcclusionQuery(void)
	{
		glEndQueryARB( GL_SAMPLES_PASSED_ARB );
	}

	/*	
	================================================================
			TextureStateHandler object helper
	================================================================
	*/
	CoreEngine::TextureStateHandler::TextureStateHandler(void)
	{
		this->mTextureState1D.Init(GL_TEXTURE_1D);
		this->mTextureState2D.Init(GL_TEXTURE_2D);
		this->mTextureState3D.Init(GL_TEXTURE_3D);
		this->mTextureStateCubeMap.Init(GL_TEXTURE_CUBE_MAP);
	}

	CoreEngine::TextureStateHandler::~TextureStateHandler(void)
	{
	}

	void CoreEngine::TextureStateHandler::TextureSateBase::Init(GLint type)
	{
		this->mType = type;
	}

	void CoreEngine::TextureStateHandler::SetActiveTextureUnit(GLuint texUNIT)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + texUNIT);
		/*
		switch(texUNIT)
		{
		case 0:
		glActiveTextureARB(GL_TEXTURE0_ARB);
		break;
		case 1:
		glActiveTextureARB(GL_TEXTURE1_ARB);
		break;
		case 2:
		glActiveTextureARB(GL_TEXTURE2_ARB);
		break;
		case 3:
		glActiveTextureARB(GL_TEXTURE3_ARB);
		break;
		case 4:
		glActiveTextureARB(GL_TEXTURE4_ARB);
		break;
		}
		*/
	}

	void CoreEngine::TextureStateHandler::TextureSateBase::Enable(void)
	{
		glEnable(this->mType);
	}

	void CoreEngine::TextureStateHandler::TextureSateBase::Disable(void)
	{
		glDisable(this->mType);
	}

	void CoreEngine::UnBindLastBuffer(bool indexBuffer)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );

		if(indexBuffer)
		{
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		}
	}

	void CoreEngine::EnableClientVBOState(GLint textureUnits)
	{
		switch(textureUnits)
		{
		case 0:
			glClientActiveTextureARB(GL_TEXTURE0);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			break;
		case 1:
			glClientActiveTextureARB(GL_TEXTURE0);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			break;
		case 2:
			glClientActiveTextureARB(GL_TEXTURE0);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE2);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			break;
		case 3:
			glClientActiveTextureARB(GL_TEXTURE0);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE2);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE3);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			break;
		case 4:
			glClientActiveTextureARB(GL_TEXTURE0);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE2);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE3);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE4);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glEnableClientState( GL_NORMAL_ARRAY );
			glEnableClientState( GL_VERTEX_ARRAY );
			break;
		default:
			break;
		}
	}

	void CoreEngine::DisableClientVBOState(GLint textureUnits)
	{
		switch(textureUnits)
		{
		case 0:
			glClientActiveTextureARB(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			break;
		case 1:
			glClientActiveTextureARB(GL_TEXTURE1);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );		

			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			break;
		case 2:
			glClientActiveTextureARB(GL_TEXTURE2);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			break;
		case 3:
			glClientActiveTextureARB(GL_TEXTURE3);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE2);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );	

			glClientActiveTextureARB(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );		

			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			break;
		case 4:

			glClientActiveTextureARB(GL_TEXTURE4);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE3);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE2);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE1);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glClientActiveTextureARB(GL_TEXTURE0);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );		

			glDisableClientState( GL_NORMAL_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			break;
		default:
			break;
		}
	}	

	GLint CoreEngine::getMaxRenderTargets(void)
	{
		return this->iMaxRenderTargets;
	}

	GLint CoreEngine::getMaxSamplesFBO(void)
	{
		return this->iMaxSamples;
	}

	/*
	================================================================
		GL Depth test Helper objects
	================================================================
	*/
	void CoreEngine::DepthTest::Enable(void)
	{
		glEnable(GL_DEPTH_TEST);
	}

	void CoreEngine::DepthTest::Disable(void)
	{
		glDisable(GL_DEPTH_TEST);
	}

	RX_API __declspec(align(16)) CoreEngine* myGlobalGame;
}