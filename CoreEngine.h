/*
=========================================================================================
	Description: Main Engine/Wrapper class

	classes contained are:

	class CoreEngine, TextureStateHandler, TextureSateBase, HardwareQueryObject
=========================================================================================
*/

#ifndef GL_GAME_CLASS_H_MY
#define GL_GAME_CLASS_H_MY

#define FORCE_NO_VERTEX_ARRAYS false
#define NOMINMAX

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ilut.lib")

/* CG LIBRARIES */
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib ")

/* Crazy Ed's GUI System LIBRARIES */
#ifdef NDEBUG
	#pragma comment(lib, "tolua++.lib")
	#pragma comment(lib, "DruidUtilities.lib")
#else
	#pragma comment(lib, "tolua++_d.lib")
	#pragma comment(lib, "DruidUtilities_d.lib")
#endif

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)
#pragma warning(disable:4482) //Non standard extension used for enum's warning disable

#include "LuaScriptEngine.h"
#include <windows.h>
#include <fstream>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <il/il.h>
#include <il/ilut.h>

/* CG HEADERS */
#include <Cg/Cg.h>
#include <Cg/cgGL.h>

#include <map>
#include <string>

#include "SoundInterface.h"
#include "utils/3DMATH.H"
#include "Texture.h"
#include "SkyBox.h"
#include "Frustum.h"
#include "Application.h"
#include "Vertex.h"

#include "LightSystem.h"

#include "Console.h"

#include <gl/glext.h> //gl extensions header

DifferentialArts::CoreEngine* createEngine(void); //Dont export this function into the dll

/*
==========================================================
 multitexturing capabilities extension capabilities
 =========================================================
 */
#include "GL_MultiTextureExterns.h"

namespace DifferentialArts
{
	/*
		FOR ANTI ALIASING DEFINES (MULTI SAMPLING)
	*/
	#define WGL_SAMPLE_BUFFERS_ARB		 0x2041
	#define WGL_SAMPLES_ARB			     0x2042

	/* Extensions */

	/*
	WINDOWS SPECIFIC VSYNC CONTROL.. USING WGL
	*/
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
	extern RX_API PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;

	/*WGL function used for AntiAliasing*/
	#include "WGL_MULTISAMPLE_DEFINES.h"

	typedef BOOL (APIENTRY *PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC, const int *, const FLOAT *, UINT, int *, UINT *);
	extern RX_API PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;	

	/*
	 more opengl defines
	 */
	#include "GLdefines.h"

	/*
	 ARB_vertex_buffer_object
	*/
	extern RX_API PFNGLBINDBUFFERARBPROC           glBindBufferARB;
	extern RX_API PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB;
	extern RX_API PFNGLGENBUFFERSARBPROC           glGenBuffersARB;
	extern RX_API PFNGLISBUFFERARBPROC             glIsBufferARB;
	extern RX_API PFNGLBUFFERDATAARBPROC           glBufferDataARB;
	extern RX_API PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB;
	extern RX_API PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB;
	extern RX_API PFNGLMAPBUFFERARBPROC            glMapBufferARB;
	extern RX_API PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB;
	extern RX_API PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
	extern RX_API PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB;

	extern RX_API PFNGLMULTIDRAWELEMENTSPROC	   glMultiDrawElements;  //For glMultiDrawElements

	/*
		OpenGL Hardware occlusion query extensions
	*/
	extern RX_API PFNGLGENQUERIESARBPROC        glGenQueriesARB;
	extern RX_API PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB;
	extern RX_API PFNGLISQUERYARBPROC           glIsQueryARB;
	extern RX_API PFNGLBEGINQUERYARBPROC        glBeginQueryARB;
	extern RX_API PFNGLENDQUERYARBPROC          glEndQueryARB;
	extern RX_API PFNGLGETQUERYIVARBPROC        glGetQueryivARB;
	extern RX_API PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB;
	extern RX_API PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB;

	/*
		OpenGL Frame buffer objects extensions
	*/
	extern RX_API PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
	extern RX_API PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
	extern RX_API PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
	extern RX_API PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
	extern RX_API PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
	extern RX_API PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
	extern RX_API PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
	extern RX_API PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
	extern RX_API PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
	extern RX_API PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
	extern RX_API PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
	extern RX_API PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
	extern RX_API PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
	extern RX_API PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
	extern RX_API PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
	extern RX_API PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
	extern RX_API PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
	extern RX_API PFNGLDRAWBUFFERSPROC glDrawBuffers;

	/* FOR MSAA in frame buffers */
	extern RX_API PFNGLBLITFRAMEBUFFEREXTPROC glBlitFramebufferEXT;
	extern RX_API PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT;
	extern RX_API PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV; //NVIDIA SPECIFIC FOR CSAA

	/* For 3D Textures*/
	extern RX_API PFNGLTEXIMAGE3DPROC glTexImage3D;

	// GL_ARB_point_parameters
	extern RX_API PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB;
	extern RX_API PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;

	//OpenGL Shader functions
	extern RX_API PFNGLPOINTPARAMETERFARBPROC		glPointParameterfARB;
	extern RX_API PFNGLPOINTPARAMETERFVARBPROC		glPointParameterfvARB;
	extern RX_API PFNGLCREATESHADEROBJECTARBPROC	glCreateShaderObjectARB;
	extern RX_API PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
	extern RX_API PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
	extern RX_API PFNGLCREATEPROGRAMOBJECTARBPROC	glCreateProgramObjectARB;
	extern RX_API PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
	extern RX_API PFNGLLINKPROGRAMARBPROC			glLinkProgramARB;
	extern RX_API PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
	extern RX_API PFNGLUNIFORM1IARBPROC				glUniform1iARB;
	extern RX_API PFNGLUNIFORM1FARBPROC				glUniform1fARB;
	extern RX_API PFNGLUNIFORM2FARBPROC				glUniform2fARB;
	extern RX_API PFNGLUNIFORM3FARBPROC				glUniform3fARB;
	extern RX_API PFNGLUNIFORM4FARBPROC				glUniform4fARB;
	extern RX_API PFNGLGETUNIFORMLOCATIONARBPROC	glGetUniformLocationARB;
	extern RX_API PFNGLDETACHOBJECTARBPROC			glDetachObjectARB;
	extern RX_API PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
	extern RX_API PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;
	extern RX_API PFNGLGETINFOLOGARBPROC			glGetInfoLogARB;

	#define BUFFER_OFFSET(i) ((char *)NULL + (i)) //Used for buffer offsetting in VBO's

	#ifndef LOG_OUTPUT
		#define LOG_OUTPUT "Output.html"
	#endif

	/*
		Variable name to text	
	*/
	#define VSTRING(text) #text	

	//Shader Profile mapping
	typedef std::map<int, std::string> ShaderProfileMap;
	typedef std::pair<int, std::string> ShaderProfilePair;

	//====================================

	/*
		\struct TypeSystemInformation
		\brief Holds some graphic device information
	*/
	typedef struct tTypeSystemInformation
	{
		char* strVendor;//!< The device vendor name
		char* strRenderer;//!< The device renderer
		char* strGLVersion;//!< The OpenGL version the device supports
		char *glslVersion; //!< The OpenGL GLSL version the device supports
	} TypeSystemInformation;

	struct vertex
	{
		GLfloat u, v;
		GLfloat nx, ny, nz;
		GLfloat x, y, z;
	};

	struct SString
	{
		char str[64];
	};

	/*
		Display list typedef, just GLuint
	*/
	/*! \typedef DisplayList
		\brief DisplayList object
	    
		This is a GLuint used to represent Display lists
	*/
	typedef GLuint DisplayList;

	/* Motion blur texture quality define */
	#define LOWEST_QUALITY		64
	#define LOW_QUALITY			128
	#define MEDIUM_QUALITY		256
	#define HIGH_QUALITY		512
	#define VERY_HIGH_QUALITY	1024

	#define AUTO_BLUR_RATE -1 //Fall back for blur rate if no blur rate was inputed

	/*! \file CoreEngine.h
		\brief Main header for the Reaction Engine
	    
		This is where the primary OpenGL class for the Reaction Engine is
		declared
	*/	
	/*! \fn CoreEngine* createEngine(void)
		\brief This function is for creating a new engine instance

		You have to create your own definition of this function. This is just a prototype
		and is NOT exported in the engine dll. After you create a game class inherited
		from CoreEngine, just return a new instance from this function.

		Eg: \n
		\code
		CoreEngine* createEngine(void)
		{
			return new CYourGame;
		}
		\endcode

		\n Where CYourGame is inherited from CoreEngine

		@return A pointer to your game object
	*/

	typedef  void (CoreEngine::*MemberFunction(void))(void);
	#define CALL_MEMBER_FN(classObject, ptrToMember)  ((classObject).*(ptrToMember))

	class Camera;

	/*!	\class CoreEngine
		\brief Main Engine class
		\author Rajesh Peter Douglas D'Monte

		This is the main class for using the Reaction engine. Inherit from this class to make use of the
		Reaction Engine.

		Example of a simple application using the Reaction Engine: \n

		\code

		class CSampleApp:public CoreEngine
		{
		public:
			CSampleApp::CSampleApp()
			{
				this->useEngineSound(false);
				this->set(90, 1, 10000, "Sample App!");
			}

			virtual bool Render(void)
			{
				glClear(GL_DEPTH_BUFFER_BIT);		

				this->mlog.mConsole.draw();	

				//this->mGUISystem->renderGUI();

				return true;
			}

			virtual void Update(float dt)
			{
				this->mTime += dt;

				this->mlog.mConsole.operate(dt);

				if(this->getApp()->keyReleased(RX_TILDE))
				{
					this->mlog.mConsole.toggleActivate();
				}	
			}


			virtual bool InitRender(void)
			{

				return true;
			}

			virtual void Release(void)
			{
			}
			
		protected:
		};

		\endcode

		@see
		CoreEngine* createEngine(void)
	*/
	class RX_API CoreEngine;

	/* Global GL Game object and pointer to GL Game object*/
	extern RX_API __declspec(align(16)) CoreEngine* myGlobalGame;

	class RX_API CoreEngine : public BaseMemoryInterface
	{
	public:	
		CoreEngine(void);//!<Default Constructor
		void Default_Constructor(void);

		~CoreEngine(void);//!<Destructor

		//static CoreEngine* createEngine(void); //Forcefully Implement in derived class

		virtual bool Render(void) = 0; //!<Implement in derived class
		virtual void Update(float dt) = 0;  //!<Implement in derived class
		virtual bool InitRender(void) = 0; //!<Implement in derived class
		virtual void Release(void) = 0;  //!<Implement in derived class

		void SizeOpenGLScreen(int w, int h);	//!<Sets the OpenGL screen size and aspect ratio
		void InitGLGAME(HWND hWnd);	//!< Initializes the Engine	
		void DeInit();//!< Free's engine data. This does NOT call Release. Release is called in ApplicationEntryPoint.h

		/* Atleast 1 set function should be called by your classes constructor */
		void set(float fov, double zNear, double zFar, LPCTSTR programTitle);//!< Sets FOV and other parameters
		/*!<
			This method MUST be called in the Constructor of your application.

			\param fov The field of view of your 3D application
			\param zNear The near clipping plane of your 3D application
			\param zFar The far clipping plane of your 3D application
			\param programTitle Your window title
		*/
		void set(float fov, double zNear, double zFar, LPCTSTR programTitle, int x, int y);//!< Sets FOV and other parameters
		/*!<
			This method MUST be called in the Constructor of your application.

			\param fov The field of view of your 3D application
			\param zNear The near clipping plane of your 3D application
			\param zFar The far clipping plane of your 3D application
			\param programTitle Your window title
		*/

		void ChangeFOV(float newFOV); //!< Changes the Field of view
		void IncreaseFOV(float df); //!< Increases the field of view by the given value
		void DecreaseFOV(float df);//!< Decreases the field of view by the given value
		void SetFullScreen(bool value);		//!< Sets fullscreen mode \bug Doesn't work yet. Remind me to fix it
		
		void OutputSystemInformation(void);//!< Outputs system information to console and log
		/* For the end programmer!, if you want GL_EXTENSIONS, use char* GetExtensions(void) */
		TypeSystemInformation getSystemInfo(void); //!< Returns a point to a TypeSystemInformation object containing system information
		
		void Setup3DScreen(float fov, double zNear, double zFar); //!< Sets up a 3D screen
		void SetupAppropriate3DScreen(void); //!<Sets up a perspective with the internal width, height, znear, zfar, fov
		void Setup3DScreenView(float fov, float width,
							   float height,
							   double zNear, double zFar); //!<same as Setup3DScreen but allows you to specify width, height
		void Setup2DScreen(int width, int height); //!< Setup a 2D screen in ORTHO mode
		void SetupCubeMapView(int cubeFace, const Math::Vector3 &objPos); //!< To be used to take the view of 6 sided reflection map or shadow map
		void SaveAndSetup2DScreen(int width, int height);//!< Pushes the current screen onto the stack and uses the new screen
		void RestoreOldScreen(void); //!<Restores the last pushed screen
		void SetupFogGame(float start, float end, int mode /* GL_LINEAR, GL_EXP, GL_EXP2 */,
						  float* fogColor, float density);//!< Setup fixed function OpenGL fog. Mode can be GL_LINEAR, GL_EXP, GL_EXP2
		void RenderMotionBlurTexture(void); //!< \deprecated Renders a screen motion blur texture for fixed function

		void PrepareForParticleRender(void); //!< Prepares the screen for rendering particle system using fixed function pipeline
		void EndParticleRender(void);//!< Ends a fixed function particle render

		/*	
		=================================================
			LUA Scripting engine object helper
		=================================================
		*/
		LuaScriptEngine* mLuaScriptEngine; //!< The lua scripting engine. All lua operations are to performed with this alone.

		/*	
		=================================================
			TextureStateHandler object helper
		=================================================
		*/

		/*!	\class TextureStateHandler
			\brief Texture state handler class
			\author Rajesh Peter Douglas D'Monte

			Used to handle texture states.
		*/
		class RX_API TextureStateHandler
		{
		public:
			TextureStateHandler(void); //!< Constructor
			~TextureStateHandler(void); //!< Deconstructor

			void SetActiveTextureUnit(GLuint texUNIT); //!< Sets the active texture unit

			/*!	\class TextureSateBase
				\brief Texture state base class
				\author Rajesh Peter Douglas D'Monte

				Used for manipulating various texture objects such as 2D textures etc
			*/
			class RX_API TextureSateBase
			{
			public:
				void Enable(void); //!< Enables texturing for the texture object
				void Disable(void);//!< Disables texturing for the texture object

			private:
				GLint mType;

				void Init(GLint type);

				friend class TextureStateHandler;
			};// mTextureState1D, mTextureState2D, mTextureState3D, mTextureStateCubeMap;

			TextureSateBase mTextureState1D;//!< 1D texture object state handler
			TextureSateBase mTextureState2D;//!< 2D texture object state handler
			TextureSateBase mTextureState3D;//!< 3D texture object state handler
			TextureSateBase mTextureStateCubeMap;//!< Cube map texture object state handler

		} mTextureStateSystem; //!< The texture state handler instance of the engine

		/*
			ALL DEPRECATED
			===============================
			void EnableTextures_1D(void);
			void DisableTextures_1D(void);
			void EnableTextures_2D(void);
			void DisableTextures_2D(void);
			void EnableTextures_CubeMap(void);
			void DisableTextures_CubeMap(void);
		*/

		virtual bool SetupFromConfigurationFile(char* fileName); //!< Initializes the engine from a configuration file
		/*!<
			This method initializes the engine from a text configuration file

			\param fileName The file path of the configuration file

			Eg: \n
			\verbatim

			[FULLSCREEN] 0
			[WIDTH] 1280
			[HEIGHT] 1024
			[FREQUENCY] 60
			[COLOR_DEPTH] 32
			[MOUSE_SENSITIVITY] 0.01
			[AUDIO_VOLUME] 0.54
			[USE_MOUSE] 1

			\endverbatim
			
			@return true if the method succeeds
		*/

		bool isPointSpriteSupported(void)  const; //!< Returns true if ARB point sprites are supported
		bool SaveScreenshot(const char* fileName, const char* format = ".jpg")  const; //!< Saves a screenshot to disk
		
		char* GetExtensions(void); //!< Returns a pointer to the OpenGL device extension list
		bool supportsMultiTexture(void); //!< Returns true if the device supports multitexturing
		void tempSetViewport(int w, int h); //!< Sets the view port to the specified dimensions without affecting engine internals
		void resetViewport(void); //!< Resets the viewport to the original dimensions
		void PrepareBlurTexture(void); //!< \deprecated Cooks a fixed function blur texture
		void RenderMotionBlurToTexture(void sceneDraw()); //!< \deprecated Renders the scene function to a blur texture
		void RenderMotionBlurToScene(void sceneDraw());	 //!< \deprecated Renders the blur texture to the screen

		/*
			Shaders support for nVIDIA Cg Shader Language
		*/

		/*! \enum EnumVertexShaderProfile
			\brief NVIDIA Cg Vertex Shader profiles
		*/
		enum RX_API EnumVertexShaderProfile
		{
			GLSLV = CG_PROFILE_GLSLV,//!< GLSL Vertex Shader profile
			ARBVP1 = CG_PROFILE_ARBVP1,//!< ARB Vertex Shader profile 1
			VP40 = CG_PROFILE_VP40,//!< NVIDIA Vertex Program 4 profile
			VP30 = CG_PROFILE_VP30,//!< NVIDIA Vertex Program 3 profile
			VP20 = CG_PROFILE_VP20,//!< NVIDIA Vertex Program 2 profile
		};

		/*! \enum EnumFragmentShaderProfile
			\brief NVIDIA Cg Fragment Shader profiles
		*/
		enum RX_API EnumFragmentShaderProfile
		{
			GLSLF = CG_PROFILE_GLSLF,//!< GLSL Fragment Shader profile
			ARBFP1 = CG_PROFILE_ARBFP1,//!< ARB Fragment Shader profile 1
			FP40 = CG_PROFILE_FP40,//!< NVIDIA Fragment Program 4 profile
			FP30 = CG_PROFILE_FP30,//!< NVIDIA Fragment Program 3 profile
			FP20 = CG_PROFILE_FP20,//!< NVIDIA Fragment Program 2 profile
		};

		bool InitializeShaders(bool bSupportCGFX = false,
							   bool debug = false,
							   EnumVertexShaderProfile vsProfile = EnumVertexShaderProfile::VP40,
							   EnumFragmentShaderProfile psProfile = EnumFragmentShaderProfile::FP40,
							   bool bGLSL = true);
		//!<Initializes shader support in the engine.

		bool isVertexProfileSupported(EnumVertexShaderProfile vsProfile);
		//!< Checks if the given vertex program profile is supported by the graphic card

		bool isFragmentProfileSupported(EnumFragmentShaderProfile fsProfile);
		//!< Checks if the given fragment program profile is supported by the graphic card

		void DeInitializeShaders(void); //!< Disables shader support in the engine
		void checkForCgError(const char *situation); //!< Used internally by the engine to check Cg Error strings
		CGprofile getCGVertexProfile(void); //!< Returns the current vertex shader profile
		CGprofile getCGPixelProfile(void);//!< Returns the current fragment shader profile
		CGcontext getCGContext(void);//!< Returns the NVIDIA Cg shader context
		__forceinline void EnableShaderProfiles(void); //!< Enables shader profiles for shader rendering. Not to be used with Effects.
		__forceinline void DisableShaderProfiles(void); //!< Disable shader profiles for shader rendering. Not to be used with Effects.	
		void UnBindAllOfLastPrograms(void);//!< Unbind all the last bound shaders. Doesn't work with effects.

		/* For binding only vertex or pixel programs */
		__forceinline void EnableVertexProfile(CGprofile vsProfile = myGlobalGame->mCGprofile_vertex);//!<Enables the vertex shader profile alone
		__forceinline void EnablePixelProfile(CGprofile fsProfile = myGlobalGame->mCGprofile_pixel);//!<Enables the fragment shader profile alone
		__forceinline void DisableVertexProfile(void);//!<Disables the vertex shader profile alone
		__forceinline void DisablePixelProfile(void);//!<Disables the fragment shader profile alone
		__forceinline void UnBindLastVertexProgram(void);//!<Unbind last vertex program
		__forceinline void UnBindLastPixelProgram(void);//!<Unbind last fragment program

		/*
			Lighting Helpers (FOR FIXED FUNCTION PIPELINE ONLY)
		*/
		void EnableLighting(void); //!< Enable OpenGL fixed function pipeline lighting
		void DisableLighting(void); //!< Disable OpenGL fixed function pipeline lighting

		/*
			HDR Helpers
		*/
		/*! \enum HDRI_Texture_formats
			\brief HDR format helper enumeration
		*/
		enum HDRI_Texture_formats
		{
			FLOAT_16_RGBA = 0x881A, //!< GL_RGBA16F_ARB
			FLOAT_16_RGB  = 0x881B, //!< GL_RGB16F_ARB
			FLOAT_32_RGBA = 0x8814, //!< GL_RGBA32F_ARB
			FLOAT_32_RGB  = 0x8815, //!< GL_RGB32F_ARB
		};

		void ForceSwapBuffers(void); //!< Forces device swap buffers
		void SetHDRITextureFormat(HDRI_Texture_formats format); //!< Sets the texture format for all HDR images to be loaded in.
		bool isHDRSupported(void); //!< Returns true if device supports HDR rendering
		GLuint getHDRITextureFormat(void); //!< Returns the current HDR format

		/*
			Multisample Functions (WARNING: WINDOWS SPECIFIC) and other extensions
		*/	
		bool InitializeAntialiasing(int samples); //!< Initializes AA. Not useful when drawing fullscreen FBO's.
		bool isAntiAliasingSupported(void); //!< Returns true if AA is supported.
		void EnableAntialiasing();//!< Enables anti aliasing. This is not for FBO's
		void DisableAntialiasing();//!< Disables anti aliasing. This is not for FBO's
		bool InitializeMSAA(void); //Checks if FBO supports MSAA, extension: GL_EXT_framebuffer_blit, GL_EXT_framebuffer_multisample
		bool InitializeCSAA(void); //Checks if FBO supports MSAA, extension: GL_EXT_framebuffer_blit, GL_EXT_framebuffer_multisample	

		bool isMSAASupported(void); //!< Returns true if the device supports FBO Multi sampling anti aliasing
		bool isCSAASupported(void); //!< Returns true if the device supports FBO Coverage sampling anti aliasing
		bool is3DTexturesSupported(void);//!< Returns true if 3D textures are supported by the device

		GLint getMaxSamplesFBO(void);//!< Returns maximum samples supported by MSAA on the device

		/*
			Display list creation helpers
		*/
		/*
			IMPORTANT: Make sure DisplayList parameter is declared externally.. NOT AS A POINTER
		*/
		//Execute as true compiles and executes the display list atleast once
		//Otherwise only compile option is enabled
		//Begin your drawing after calling this function.. and then call endDisplayList();
		void createDisplayList(bool execute, DisplayList* display); //!< Creates a display list. Specify true for execute if you wish to execute it immediately.
		void endDisplayList(void);//!< End the display list.
		void callDisplayList(DisplayList* display);//!< Calls a display list for rendering
		void deleteDisplayList(DisplayList* display);//!< Deletes a display list
		void deleteDisplayLists(DisplayList* display, GLuint range); //!< Deletes many display lists

		//Multiple render target stuff
		GLint getMaxRenderTargets(void); //!< Returns the total render targets the device supports

		/*
			[==============================]
		*/

		/*
		================================================================
			Vsynce control (WARNING: WINDOWS SPECIFIC)
		================================================================
		*/
		void EnableVSYNC(void); //!< Enables Vertical Synchronization with the monitor refresh rate
		void DisableVSYNC(void);//!< Disables Vertical Synchronization

		/*	
		================================================================
			Harware occlusion query object helper
		================================================================
		*/
		/*!	\class HardwareQueryObject
			\brief Hardware occlusion query object
			\author Rajesh Peter Douglas D'Monte

			Used to handle hardware occlusion queries \n
			NOTE:\n
			For hardware occlusion query, make sure you render your scene once atleast normally, 
			you need not do this if you render all objects back to front consistently
		*/
		class RX_API HardwareQueryObject
		{
		public:
			void CreateQueryObject(GLuint* pInt); //!< Creates a query object
			void DeleteQueryObject(GLuint* pInt); //!< Deletes a query object
			void GetFragmentCount(GLuint* queryObject, GLuint* fragments); //!<This returns the unoccluded pixels, The fragment count is returned in the 'fragments' parameter

			void BeginOcclusionQuery(GLuint* queryObject); //!< Call this before render starts for hardware occlusion queries.
			void EndOcclusionQuery(void); //!<Call this after render ends for hardware occlusion queries		
		} mHardWareQuery; //!< Hardware occlusion query object helper

		/*
		================================================================
			GL Matrix MODE Helper objects
		================================================================
		*/
		/*!	\class MatrixModeHelper
			\brief OpenGL matrix helpers
			\author Rajesh Peter Douglas D'Monte

			Used to Matrix modes in OpenGL
		*/
		class RX_API MatrixModeHelper
		{
		public:
			void useModelViewMatrix()
			{
				glMatrixMode(GL_MODELVIEW);
			}
			//!< Sets the current matrix mode to MODEL VIEW

			void useProjectionMatrix()
			{
				glMatrixMode(GL_PROJECTION);
			}				
			//!< Sets the current matrix mode to PROJECTION

			void useTextureMatrix()
			{
				glMatrixMode(GL_TEXTURE);
			}
			//!< Sets the current matrix mode to TEXTURE

			void LoadIdentity()
			{
				glLoadIdentity();
			}//!< Loads the identity matrix onto the matrix stack
			 
			void pushMatrix()
			{
				glPushMatrix();
			}
			//!< Pushes a matrix onto the matrix stack

			void popMatrix()
			{
				glPopMatrix();
			}
			//!< Pops a matrix off the matrix stack
		} mMatrixMode; //!< OpenGL Matrix Mode helper object

		/*
		================================================================
			GL Depth test Helper objects
		================================================================
		*/
		/*!	\class DepthTest
			\brief DepthTest helper
			\author Rajesh Peter Douglas D'Monte

			Depth Test state helper object
		*/
		class RX_API DepthTest
		{
		public:
			void Enable(); //!< Enables depth testing
			void Disable(); //!< Disables depth testing
		} mDepthTest;//!< Depth text OpenGL state helper object

		/*
		================================================================
			More Helpers
		================================================================
		*/

		/*
			Collective primitive drawing functions,
			support for vertex arrays, index buffers and vbo's
		*/	

		/* VBO helper functions */
		bool isVBOSupported(void);//!< Returns true if VBO's are supported
		/*
			 Set indexBuffer parameter to true, if the last VBO you used
			 was an indexed VBO, if your unsure, just set to "true" and
			 don't leave with the default parameter "false"
		*/
		void UnBindLastBuffer(bool indexBuffer = false); //!< Unbinds the last Vertex buffer. Supply true if your using an index buffer
		void EnableClientVBOState(GLint textureUnits = 0); //!<Enables client VBO state. TextureUnits can be 0, 1, 2, 3 only
		void DisableClientVBOState(GLint textureUnits = 0); //!<Disables client VBO state. TextureUnits can be 0, 1, 2, 3 only

		/* FBO helper functions */
		bool isFBOSupported(void); //!< Returns true if Frame buffer objects are supported

		/* Is using CGFX */
		bool isUsingCGFX(void); //!< Returns true if engine is using CgFX

		/*
			[==============================]
		*/

		Math::Vector3 projectPoint(const Math::Vector3 &point); //!< Projects a point to screen space
		Math::Vector3 unProjectPoint(const Math::Vector3 &point); //!< UnProjects a screen point to view space
		Math::Vector3 unProjectPointFlipY(const Math::Vector3 &point); //!<UnProjects a screen point to view space with Y axis flipped

		void InitMotionBlurSystem(GLuint quality, float alpha, float blurRate);//!< \deprecated Initializes the motion blur system
		void SetMotionBlurSystem(GLuint quality, float alpha, float blurRate);	//!< \deprecated Sets motion blur system properties

		void PrepareForCursorRender(void); //!< \deprecated CEGUI is now used for this
		void EndCursorRender(void);//!< \deprecated CEGUI is now used for this
		void DrawCursor(GLuint texture, float x, float y, float sx, float sy);//!< \deprecated CEGUI is now used for this
		void DrawCursorDirect(GLuint texture, float x, float y, float sx, float sy);	//!< \deprecated CEGUI is now used for this
		//bool UpdateGameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/* 
		=====================================
			For Engine sound system
		=====================================
		*/

		//Sound Object
		SoundSystem* mSoundSystem; //!< The engine sound system

		/* 
		=====================================
			End of Engine sound system
		=====================================
		*/

		//friend void SetListenerPositionAndVelocity(Math::Vector3 pos, Math::Vector3 vel);
		//friend void SetListenerOrientation(Math::Vector3 heading, Math::Vector3 up);
		friend class SoundSystem;
		
		bool isOccluded(const Math::Vector3 &p, GLdouble* wx, GLdouble* wy, GLdouble* wz);//!< Returns true if the given point is occluded
		bool isScreenPointOccluded(GLdouble wx, GLdouble wy, GLdouble wz);//!< Returns true if the given 2D point is occluded against \b wz

		double getZNear(void); //!< Returns the near clipping plane distance
		double getZFar(void);//!< Returns the far clipping plane distance
		float getFOV(void);//!< Returns the field of view

		inline Application* getApp(void);//!< Returns the application object
		
		Skybox mSkyBox; //!< A handy skybox object
		Frustum frustum;//!< The current view frustum		
		bool mbEnginePaused; //!< This tells if the engine is paused or not. For example this is true when the application is minimized. 
		Console mlog;	//!< The logger object
		void useEngineSound(bool bOpenAL); //!< Should be called in constructor. This determines if the engine uses its sound capabilities.
		bool usingEngineSound(void) const;//!< Returns true if the engine is using its sound capabilities
		bool isUsingOpenAL(void) const; //!< Returns true if using OpenAL

		//Adds an object for use with glPushName
		GLuint createObject(void);//Not yet functional
		void renderInitialized(void); //!< This is called in ApplicationEntryPoint.h when the Init is successfull.
		bool isRenderInitialized(void);//!< Returns true if Init was successfull.
		//GLuint getSelectedObject(int x, int y, int w, int h);

		//AbstractLight Handler
		LightSystem mLightHandler; //!< The Main light handler object.

		//Deprecated stuff
		int currentMenu;

	protected:
		bool initRender;
		double zNear;
		double zFar;			
		
		float fov;	
		bool bMultiTexture;
		bool bDrawVertexArrayWithNormals;
		char* glExtensions;		
		TypeSystemInformation mySystem;	
		Application mAppObject;

		/*For motion blur */
		GLuint mBlurTexture;
		GLuint mBlurQuality;
		float lastBlurTime;
		bool bUseEngineSound;
		bool bUseOpenAL;

		//For vsync
		bool bVSYNCsupport;

		//For VBO's
		bool bVBO;

		//For FBO's
		bool bFBO;

		//For Hardware Occlusion
		bool bOcclusionQuery;

		//For anti alias
		bool bAntiAliasSupport;

		//For MSAA
		bool bMSAA;

		//Max samples for MSAA/CSAA
		GLint iMaxSamples;

		//For CSAA
		bool bCSAA;

		//For 3D textures
		bool b3Dtexture;

		//Boolean to store if Engine instance is using CGFX with OpenGL States or not
		bool bCGFX;

		//For ARB point sprites
		bool bPointSprites;

		//Initializes 3D texture extension
		bool Initialize3DTextures(void);

		//Initializes ARB point sprite extension
		bool InitializePointSprite(void);

		//Internal HDR Image texture format
		GLuint miHDRITextureFormat;

		//HDR support state
		bool mbHDR;

		//For shader support
		bool bShaderSupport;
		CGprofile   mCGprofile_vertex;
		CGprofile   mCGprofile_pixel;
		CGcontext	mCGcontext;	

		//For the mapping of PROFILE ID NUMBER TO PROFILE NAME STRING
		ShaderProfileMap mCGProfileMapping;

		//For object ID's
		GLuint mPresentID;

		//Multiple render target stuff
		GLint iMaxRenderTargets;

		ILuint GenerateSingleImage(void) const;
		void DeleteSingleImage(ILuint ImageName) const;

		//Deprecated stuff
		float mfBlurAlpha;
		float mfBlurRate;

		/* OpenGL stuff */
		void InitializeOpenGL(int width, int height);
		void QuerySystemInformation(void);	//Should be called only after QuerySystemInformation is called

		/* VBO helper functions */
		bool InitializeVertexBufferObjects(void);

		/* FBO helper functions */
		bool InitializeFrameBufferObjects(void);

		/* HDR Helper Functions */
		bool InitializeHDRExtensions(void);

		/*
			Vsynce control (WARNING: WINDOWS SPECIFIC)
		*/
		void InitializeVSYNC(void);	
		
		/*	
			Harware occlusion query setup
		*/
		bool InitializeHarwareOcclusionQuery(void);

		bool mbMainWindow;	

		friend class Camera;	
	};

}

#endif

