/*
==========================================================================
	contains application class
	class Application
==========================================================================
*/
#define NOMINMAX

#include "Application.h"
#include "CoreEngine.h"

namespace DifferentialArts
{
	/*
	============================================================
		Application class for win32
	============================================================
	*/

	void Application::MsgBox(LPCTSTR text, LPCTSTR title)
	{
		MessageBox(NULL, text, title, MB_OK);
	}

	void Application::MsgBoxAndQuit(LPCTSTR text, LPCTSTR title)
	{
		MessageBox(NULL, text, title, MB_OK);
		this->end();
	}

	void Application::end(void)
	{
		this->mlog->LogInfo(LString("Application::end() was called"));
		this->winMessage = false;
		PostQuitMessage(0);
	}

	bool Application::isRunning()
	{
		return this->winMessage;
	}

	float Application::GetMouseSensitivity(void)
	{	
		return this->mouseSensitivity;
	}

	void Application::WindowRefresh(void)
	{
		ShowWindow(this->m_hWnd, SW_SHOWNORMAL);					// Show the window
		UpdateWindow(this->m_hWnd);									// Draw the window
		SetFocus(this->m_hWnd);
	}

	void Application::SetMouseSensitivity(float m)
	{
		if(m > 0.1f)
		{
			m = 0.1f;
		} else if(m<=0.001f)
		{
			m = 0.001f;
		}
		this->mouseSensitivity = m;
	}


	bool Application::isActivated(void)
	{
		return this->active;
	}

	bool Application::UpdateGameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, float fov,
										 double zNear, double zFar)
	{
		if(mbCEGUIUpdateInputInjection)
		{
			switch(msg)
			{
				case WM_ACTIVATE:							// Watch For Window Activate Message
				{
					if (!HIWORD(wParam))					// Check Minimization State
					{
						this->active=true;						// Program Is Active

						if(this->pause)
						{
							this->pause = false;
							myGlobalGame->mSoundSystem->ResumeAllSounds();
						}
					}
					else
					{
						this->active=false;						// Program Is No Longer Active

						if(!this->pause)
						{
							this->pause = true;
							this->Mouse.Show();
							myGlobalGame->mSoundSystem->StopAllSounds();
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
					if(!this->m_bFullScreen)
					{
						//this->Mouse.x = LOWORD(lParam);
						//this->Mouse.y = HIWORD(lParam);
						this->Mouse.x = int((float(LOWORD(lParam))/float(this->m_rRect.right)) * this->m_rWindowRect.right);
						this->Mouse.y = int((float(HIWORD(lParam))/float(this->m_rRect.bottom)) * this->m_rWindowRect.bottom);		
					} else {
						this->Mouse.x = LOWORD(lParam);
						this->Mouse.y = HIWORD(lParam);
					}
					//POINT pt;
					//GetCursorPos(&pt);
					//this->Mouse.x = pt.x;
					//this->Mouse.y = pt.y;					
					break;
				case WM_KEYDOWN:			
					this->mKeyWParam = wParam;
					this->mKeyLParam = lParam;		

					/*
					switch(wParam)
					{
					case RX_LSHIFT:
						CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::LeftShift);
						break;
					case RX_RSHIFT:
						CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::RightShift);				
						break;
					}*/			

					break;	
				case WM_CHAR:
					this->characterReleased = wParam;
					
					break;	
				case WM_SIZE:										// If the window is resized
					if(!this->isFullScreen())
					{
						this->SizeOpenGLScreen(this->width = LOWORD(lParam), this->height = HIWORD(lParam), fov, zNear, zFar);// LoWord=Width, HiWord=Height
						GetClientRect(hWnd, &this->getRect());				// Get the window rectangle			
					}

					if(wParam == SIZE_MINIMIZED)
					{
						myGlobalGame->mbEnginePaused = true;
					} else if(wParam == SIZE_RESTORED)
					{
						myGlobalGame->mbEnginePaused = false;
					}
					break; 
				case WM_MOVE:
					this->windowPosition.x = (int)LOWORD(lParam);
					this->windowPosition.y = (int)HIWORD(lParam);
					break;
				case WM_LBUTTONDOWN:	
					this->Mouse.LeftButton.SetState(MOUSE_DOWN);						
					break;
				case WM_LBUTTONUP:
					this->Mouse.LeftButton.SetState(MOUSE_UP);	
					this->Mouse.LeftButton.SetRelease(true);
					break;
				case WM_RBUTTONDOWN:
					this->Mouse.RightButton.SetState(MOUSE_DOWN);
					break;
				case WM_RBUTTONUP:
					this->Mouse.RightButton.SetState(MOUSE_UP);
					this->Mouse.RightButton.SetRelease(true);
					break;
				case WM_MBUTTONDOWN:
					this->Mouse.MiddleButton.SetState(MOUSE_DOWN);
					break;
				case WM_MBUTTONUP:
					this->Mouse.MiddleButton.SetState(MOUSE_UP);
					this->Mouse.MiddleButton.SetRelease(true);
					break;
				case WM_MOUSEWHEEL:
					this->Mouse.mouseWheel = (short)HIWORD(wParam);
					break;
				/*
				case WM_DESTROY:
					this->mlog.LogSuccessText(LString("WM_DESTROY case confirmed"));
					this->end();
					return false;
					break;
				*/
				case WM_CLOSE:
					this->mlog->LogSuccessText(LString("WM_CLOSE case confirmed"));
					this->end();
					return false;
					break;
				case WM_QUIT:
					this->mlog->LogSuccessText(LString("WM_QUIT case confirmed"));
					this->end();
					return false;
					break;
				default:	
					/*
					if(wParam == RX_LSHIFT)
					{
						CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::LeftShift);
					} else {
						if(CEGUI::System::getSingletonPtr() != 0)
						{
							CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::LeftShift);
						}
					}

					if(wParam == RX_RSHIFT)
					{
						CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::RightShift);
					} else {
						if(CEGUI::System::getSingletonPtr() != 0)
						{
							CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::RightShift);
						}
					} */
					break;
			}
		} else {
			switch(msg)
			{
				case WM_ACTIVATE:							// Watch For Window Activate Message
				{
					if (!HIWORD(wParam))					// Check Minimization State
					{
						this->active=true;						// Program Is Active

						if(this->pause)
						{
							this->pause = false;
							myGlobalGame->mSoundSystem->ResumeAllSounds();
						}
					}
					else
					{
						this->active=false;						// Program Is No Longer Active

						if(!this->pause)
						{
							this->pause = true;
							this->Mouse.Show();
							myGlobalGame->mSoundSystem->StopAllSounds();
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
					if(!this->m_bFullScreen)
					{
						//this->Mouse.x = LOWORD(lParam);
						//this->Mouse.y = HIWORD(lParam);
						this->Mouse.x = int((float(LOWORD(lParam))/float(this->m_rRect.right)) * this->m_rWindowRect.right);
						this->Mouse.y = int((float(HIWORD(lParam))/float(this->m_rRect.bottom)) * this->m_rWindowRect.bottom);		
					} else {
						this->Mouse.x = LOWORD(lParam);
						this->Mouse.y = HIWORD(lParam);
					}
					//POINT pt;
					//GetCursorPos(&pt);
					//this->Mouse.x = pt.x;
					//this->Mouse.y = pt.y;				
					break;
				case WM_KEYDOWN:			
					this->mKeyWParam = wParam;
					this->mKeyLParam = lParam;	
					
					break;	
				case WM_CHAR:
					this->characterReleased = wParam;
					break;	
				case WM_SIZE:										// If the window is resized
					if(!this->isFullScreen())
					{
						this->SizeOpenGLScreen(this->width = LOWORD(lParam), this->height = HIWORD(lParam), fov, zNear, zFar);// LoWord=Width, HiWord=Height
						GetClientRect(hWnd, &this->getRect());				// Get the window rectangle			
					}

					if(wParam == SIZE_MINIMIZED)
					{
						myGlobalGame->mbEnginePaused = true;
					} else if(wParam == SIZE_RESTORED)
					{
						myGlobalGame->mbEnginePaused = false;
					}
					break; 
				case WM_MOVE:
					this->windowPosition.x = (int)LOWORD(lParam);
					this->windowPosition.y = (int)HIWORD(lParam);
					break;
				case WM_LBUTTONDOWN:	
					this->Mouse.LeftButton.SetState(MOUSE_DOWN);			
					break;
				case WM_LBUTTONUP:
					this->Mouse.LeftButton.SetState(MOUSE_UP);	
					this->Mouse.LeftButton.SetRelease(true);
					break;
				case WM_RBUTTONDOWN:
					this->Mouse.RightButton.SetState(MOUSE_DOWN);
					break;
				case WM_RBUTTONUP:
					this->Mouse.RightButton.SetState(MOUSE_UP);
					this->Mouse.RightButton.SetRelease(true);
					break;
				case WM_MBUTTONDOWN:
					this->Mouse.MiddleButton.SetState(MOUSE_DOWN);
					break;
				case WM_MBUTTONUP:
					this->Mouse.MiddleButton.SetState(MOUSE_UP);
					this->Mouse.MiddleButton.SetRelease(true);
					break;
				case WM_MOUSEWHEEL:
					this->Mouse.mouseWheel = (short)HIWORD(wParam);
					break;
				/*
				case WM_DESTROY:
					this->mlog.LogSuccessText(LString("WM_DESTROY case confirmed"));
					this->end();
					return false;
					break;
				*/
				case WM_CLOSE:
					this->mlog->LogSuccessText(LString("WM_CLOSE case confirmed"));
					this->end();
					return false;
					break;
				case WM_QUIT:
					this->mlog->LogSuccessText(LString("WM_QUIT case confirmed"));
					this->end();
					return false;
					break;
				default:	
					break;
			}
		}

		return true;
	}

	void Application::SizeOpenGLScreen(int w, int h, float fov, double zNear, double zFar)
	{
		if(h == 0)
			h = 1;

		glViewport(0, 0, w, h);	

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(fov, (float)w/(float)h, zNear, zFar);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	bool Application::bSetupPixelFormat(HDC hdc)
	{
		mlog->LogInfo(LString("Setup of pixel format started..."));
		PIXELFORMATDESCRIPTOR pfd = {0}; 
		int pixelformat; 
	 
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);		
		pfd.nVersion = 1;									
															
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
		pfd.dwLayerMask = PFD_MAIN_PLANE;					
		pfd.iPixelType = PFD_TYPE_RGBA;						
		pfd.cColorBits = this->mColorBits;						
		pfd.cDepthBits = this->mDepthBits;	 //i think if i set this to zero, openGL will use the correct value					
		pfd.cAccumBits = 0;						
		pfd.cStencilBits = this->mStencilBits;	
		pfd.cAlphaBits = this->mAlphaBits;
	 
		mlog->LogInfo(LString("Choose pixel format started... "));
		// This gets us a pixel format that best matches the one passed in from the device
		if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
		{ 
			MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
			mlog->LogError(LString("ChoosePixelFormat has failed"));
			return false; 
		} 

		mlog->LogSuccessText(LString("Choose pixel format is successful"));
	 
		mlog->LogInfo(LString("SetPixel format started... "));
		// This sets the pixel format that we extracted from above
		if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
		{ 
			MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
			mlog->LogError(LString("SetPixelFormat has failed"));
			return false; 
		} 
		mlog->LogSuccessText(LString("SetPixel format is successful"));

		this->mPFD = pfd;

		mlog->LogSuccessText(LString("Setup of pixel format has succeeded!"));
	 
		return true;
	}

	void Application::OverrideDeviceSettings(int refreshRate, unsigned char colorBits,
									unsigned char depthBits, unsigned char stencilBits,
									unsigned char alphaBits)
	{
		this->mColorBits = colorBits;
		this->mRefreshRate = refreshRate;
		this->mDepthBits = depthBits;
		this->mStencilBits = stencilBits;
		this->mAlphaBits = alphaBits;
	}

	void Application::ChangeToFullScreen()
	{
		mlog->LogInfo(LString("Change to fullscreen has started..."));
		DEVMODE dmSettings;	
		memset(&dmSettings,0,sizeof(DEVMODE));

		if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
		{
			MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
			return;
		}	
		
		dmSettings.dmSize			= sizeof (DEVMODE);
		dmSettings.dmPelsWidth		= this->width;
		dmSettings.dmPelsHeight		= this->height;
		dmSettings.dmBitsPerPel		= this->mColorBits;
		dmSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		dmSettings.dmDisplayFrequency = this->mRefreshRate;

		int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	
		if(result != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
			mlog->LogError(LString("Display mode not compatible for fullscreen"));
			PostQuitMessage(0);
		}

		mlog->LogSuccessText(LString("Change to Fullscreen has succeeded!"));	
	}

	void Application::init_app(Console* pLog)
	{
		this->mlog = pLog;
	}

	void Application::setHINSTANCE(HINSTANCE hInstance)
	{
		this->m_hInstance = hInstance;
	}

	HINSTANCE Application::getHInstance(void)
	{
		return this->m_hInstance;
	}

	HDC Application::getDC(void)
	{
		return this->m_hDC;
	}

	HWND Application::getHandle(void)
	{
		return this->m_hWnd;
	}

	int Application::GetFrameRate(void)
	{
		return this->m_FPS;
	}

	RECT Application::getRect(void)
	{
		return this->m_rRect;
	}

	RECT Application::getWindowRect(void)
	{
		return this->m_rWindowRect;
	}

	void Application::ResetMouseRelease(void)
	{
		this->Mouse.LeftButton.SetRelease(false);
		this->Mouse.RightButton.SetRelease(false);
		this->Mouse.MiddleButton.SetRelease(false);
	}



	unsigned int Application::getRefreshRate(void)
	{
		return this->mRefreshRate;
	}

	unsigned int Application::getColorDepth(void)
	{
		return this->mColorBits;
	}

	unsigned int Application::getDepthBits(void)
	{
		return this->mDepthBits;
	}

	unsigned int Application::getStencilBits(void)
	{
		return this->mStencilBits;
	}

	unsigned int Application::getAlphaBits(void)
	{
		return this->mAlphaBits;
	}

	bool Application::keyReleased_wParam(unsigned int  key, WPARAM wParam)
	{
		if(wParam == key)
		{		
			return true;
		}

		return false;
	}

	bool Application::keyReleased(unsigned int key)
	{
		if(this->mKeyWParam == key)
		{	
			this->mKeyWParam = 0;
			return true;
		}

		return false;
	}

	bool Application::keyPressed(unsigned int key)
	{
		if(GetKeyState(key) & 0x80)
		{
			return true;
		}

		/*
		if(this->mKeyWParam == key)
		{	
			return true;
		}*/

		return false;
	}

	void Application::CenterMouse(void)
	{
		this->Mouse.CenterScreen(this->windowPosition, this->getDimensions() , this->systemResolution);
	}

	DIM Application::getDimensions()
	{
		DIM myDim;
		myDim.x = width;
		myDim.y = height;
		return myDim;
	}

	DIM Application::getSystemResolution(void)
	{
		return this->systemResolution;
	}

	DIM Application::getCenter(void)
	{
		DIM ret;
		ret = this->Mouse.getCenter(this->windowPosition, this->getDimensions() , this->systemResolution);
		this->centerScreen = ret;
		return ret;
	}

	DIM Application::getLastCenter(void)
	{
		return this->centerScreen;
	}

	RX_API void Application::ToggleFullScreen(void)
	{
		PostMessage(this->m_hWnd, (WM_USER+1), 0, 0);
		//this->DestroyWindowObject();
	}

	HWND Application::createWindow(void)
	{
		HWND rhWnd = CreateWindow(	CLASSNAME, 
									this->mWindowName,
									this->dwStyle,
									0,
									0,
									this->getDimensions().x,
									this->getDimensions().y,
									NULL,
									NULL,
									this->m_hInstance,
									NULL);

		return rhWnd;
	}

	LPCTSTR Application::getWindowName(void)
	{
		return this->mWindowName;
	}

	bool Application::isFullScreen(void)
	{
		bool ret;

		if(this->m_bFullScreen <= 0)
		{
			ret = false;
		} else {
			ret = true;
		}
		return ret;
	}

	char Application::getAlphaNumeric(void)
	{
		int i;	

		for( i = 65; i <= 90; i++)
		{
			if(this->keyReleased(i))
			{
				return i;
			}
		}

		for(i = 48; i <= 57; i++)
		{
			if(this->keyReleased(i))
			{
				return i;
			}
		}

		if(this->keyReleased(32))
		{
			return ' ';
		}

		if(this->keyReleased(47))
		{
			return '/';
		}

		return 0;
	}

	void Application::InitializeFromPixelFormatForMultiSample(const int pf)
	{
		PIXELFORMATDESCRIPTOR pfd = {0};
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = this->mColorBits;
		pfd.cDepthBits = this->mDepthBits;

		this->m_hDC = GetDC(this->m_hWnd);
		SetPixelFormat(this->m_hDC, pf, &pfd);
		this->m_hRC = wglCreateContext(this->m_hDC);
		wglMakeCurrent(this->m_hDC, this->m_hRC);
	}

	HGLRC Application::getGLRenderContext(void)
	{
		return this->m_hRC;
	}

	void Application::DestroyWindowObject(void)
	{
		if(this->m_hWnd)
		{
			if (this->m_hDC) 
			{
				wglMakeCurrent(m_hDC, NULL);	
				if (m_hRC)											
				{				
					wglDeleteContext(m_hRC);
				}
				ReleaseDC(m_hWnd, m_hDC);
				m_hDC = 0;
			}
			DestroyWindow(m_hWnd);
			m_hWnd = 0;
		}

		if(m_bFullScreen)
		{
			ChangeDisplaySettings(NULL, 0);
			Mouse.Show();
		}
	}

	void Application::UpdateKeyReleased(void)
	{
		this->characterReleased = 0;
	}

	unsigned int Application::getLastKeyReleased(void)
	{
		return this->characterReleased;
	}

	PIXELFORMATDESCRIPTOR Application::getPixelFormatDescriptor(void)
	{
		return this->mPFD;
	}

	bool Application::WGLisExtensionSupported(const char *extension)
	{
		const size_t extlen = strlen(extension);
		const char *supported = NULL;

		// Try To Use wglGetExtensionStringARB On Current DC, If Possible
		PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

		if (wglGetExtString)
			supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

		// If That Failed, Try Standard Opengl Extensions String
		//if (supported == NULL)
		//	supported = (char*)glGetString(GL_EXTENSIONS);

		// If That Failed Too, Must Be No Extensions Supported
		if (supported == NULL)
			return false;

		// Begin Examination At Start Of String, Increment By 1 On False Match
		for (const char* p = supported; ; p++)
		{
			// Advance p Up To The Next Possible Match
			p = strstr(p, extension);

			if (p == NULL)
				return false;															// No Match

			// Make Sure That Match Is At The Start Of The String Or That
			// The Previous Char Is A Space, Or Else We Could Accidentally
			// Match "wglFunkywglExtension" With "wglExtension"

			// Also, Make Sure That The Following Character Is Space Or NULL
			// Or Else "wglExtensionTwo" Might Match "wglExtension"
			if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
				return true;															// Match
		}
	}

}