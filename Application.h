/*
==========================================================================
	contains win32 application class
	class Application
==========================================================================
*/
#ifndef APPLICATION_WIN32_H
#define APPLICATION_WIN32_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Client.h"
#include "Mouse.h"
#include "Console.h"
#include "KeyDefines.h"

namespace DifferentialArts
{
	#define WIN32_LEAN_AND_MEAN      // Trims down libraries.
	#define VC_LEANMEAN              // Trims even more libraries.

	#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 522
	#endif

	#define CLASSNAME "SOMECLASSNAME_FOR_THIS_PRGRM"

	/*
	============================================================
		Application class for win32
	============================================================
	*/
	/*!	\class Application
		\brief Application layer class
		\author Rajesh Peter Douglas D'Monte

		This class is for maintaining most communication with the
		operating system. 
	*/
	class RX_API Application
	{
	public:
		HWND createWindow(void); //!< Creates a window and returns a handle
		void MsgBox(LPCTSTR text, LPCTSTR title); //!< Displays a message box
		void MsgBoxAndQuit(LPCTSTR text, LPCTSTR title); //!< Displays a message box and quits
		void init_app(Console* pLog); //!< Initialize the application with a pointer to a log object
		void ChangeToFullScreen(); //!< Change to fullscreen \bug May not work if you started windowed
		void end(void); //!< Ends the application
		bool isRunning(); //!< Returns true if the application is running
		int GetFrameRate(void); //!< Returns the frame rate
		RECT getRect(void); //!< Returns the main rect
		RECT getWindowRect(void);//!< Returns the window rect
		bool isFullScreen(void); //!< Returns true if fullscreen

		unsigned int getRefreshRate(void); //!< Returns the refresh rate
		unsigned int getColorDepth(void); //!< Returns the color depth
		unsigned int getDepthBits(void); //!< Returns the depth bits
		unsigned int getStencilBits(void); //!< Returns the stencil bits
		unsigned int getAlphaBits(void); //!< Returns the alpha bits
		DIM getDimensions(); //!< Returns the dimensions of the application
		DIM getSystemResolution(void); //!< Returns the system resolution

		//Key Event handlers
		bool keyPressed(unsigned int key); //!< Check if key is pressed
		bool keyReleased_wParam(unsigned int key, WPARAM wParam); //!< \deprecated @see keyPressed
		bool keyReleased(unsigned int  key); //!< Check if key is released
		unsigned int getLastKeyReleased(void); //!< Returns the last key released
		void UpdateKeyReleased(void); //Not for the end user
		void SetMouseSensitivity(float m); //!< Sets mouse sensitivity
		void WindowRefresh(void); //!< Calls ShowWindow UpdateWindow and SetFocus

		void CenterMouse(void); //!< Centers the mouse position
		void ResetMouseRelease(void); //Not for the end user
		DIM getCenter(void); //!< Returns the center 
		DIM getLastCenter(void); //!< Returns the last value calculated by getCenter()
		float GetMouseSensitivity(void); //!< Returns the mouse sensitivity
		bool useMouse;	//!< For mouse status

		void setHINSTANCE(HINSTANCE hInstance); 
		HDC getDC(void); //!< Returns the device context
		HWND getHandle(void);	//!< Returns the window handle
		HINSTANCE getHInstance(void); //!< Returns the hInstance object
		void ToggleFullScreen(void); //!< \bug Not yet functional
		PIXELFORMATDESCRIPTOR getPixelFormatDescriptor(void); //! Returns the pixel format descriptor used by the engine
		LPCTSTR getWindowName(void); //!< Returns the window name
		HGLRC getGLRenderContext(void); //!< Returns the GL render context the engine is using

		bool UpdateGameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, float fov,
							   double zNear, double zFar); //Not for the end user
		bool isActivated(void); //!< Returns true if application is activated

		/*
			Multisample Functions
		*/	
		//WARNING: ONLY FOR SPECIAL EXTENSIONS ON WIGGLE
		bool WGLisExtensionSupported(const char *extension); //!< Check to see if WGL extension is supported
		void InitializeFromPixelFormatForMultiSample(const int pf); //Not for the end user
		void DestroyWindowObject(void);	 //!< Destroys the window object

		void OverrideDeviceSettings(int refreshRate = 60, unsigned char colorBits = 24,
									unsigned char depthBits = 24, unsigned char stencilBits = 0,
									unsigned char alphaBits = 0); //!< Overrides default device settings, call in your game object's constructor.

		DWORD m_FrameInterval;
		int m_FPS;
		int tFPS;
		float m_fFrameIntervalSeconds;
		float m_ftRatio;
		float m_fGameTime;
		float m_fActualTime;

		DWORD currentTick;
		DWORD lastTick;
		DWORD dwStyle;
		float frLastTime;
		//The log file Object	
		Mouse Mouse; //!< Mouse object. Access the mouse through this object.

		MousePointInt windowPosition;
		WPARAM mKeyWParam;
		LPARAM mKeyLParam;
		Console* mlog;	
		float volume;	//!< \deprecated FMOD controls volume instead

		bool mbCEGUIUpdateInputInjection;

	private:
		BOOL  m_bFullScreen;
		HWND  m_hWnd;											// This is the handle for the window
		RECT  m_rRect;											// This holds the window dimensions
		HDC   m_hDC;											// General HDC - (handle to device context)
		HGLRC m_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
		HINSTANCE m_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()	
		RECT  m_rWindowRect;	
		unsigned int width;
		unsigned int height;
		//Bit plane values
		unsigned int mColorBits;
		unsigned int mStencilBits;
		unsigned int mAlphaBits;
		unsigned int mDepthBits;
		unsigned int mRefreshRate;
		float mouseSensitivity;

		unsigned int characterReleased;	
		char getAlphaNumeric(void);
		bool bSetupPixelFormat(HDC hdc);
		void SizeOpenGLScreen(int w, int h, float fov, double zNear, double zFar);

		DEVMODE	m_dmOld;
		bool winMessage;	
		DIM systemResolution;	
		DIM centerScreen;
		LPCTSTR mWindowName;
		PIXELFORMATDESCRIPTOR mPFD;

		bool active;
		bool pause;

		//..Friends
		friend class CoreEngine;
	};
}

#endif