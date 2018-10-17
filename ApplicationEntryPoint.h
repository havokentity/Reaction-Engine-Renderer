/*
	WINMAIN exists here.. for other os's add respective entry points and
	change appropriate defines
*/

#ifndef WIN32_APPLICATION_ENGINE_ENTRY_POINT
#define WIN32_APPLICATION_ENGINE_ENTRY_POINT

/*! \file ApplicationEntryPoint.h
    \brief The application entry point
    
    This header should be included in all your projects if you want to use the Reaction Engine.
	Modification of this header is possible, but not recommended. Only expert users may modify
	this header.
*/


#include "CoreEngine.h"
#include "Font.h"

//Function for application entry point (WIN32)
/* Client WIN32 Entry Point */
	#ifndef EXPORT_DLL
		long __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{	
			if(!DifferentialArts::myGlobalGame->getApp()->UpdateGameWndProc(hWnd, msg, wParam, lParam,
														  DifferentialArts::myGlobalGame->getFOV(),
														  DifferentialArts::myGlobalGame->getZNear(),
														  DifferentialArts::myGlobalGame->getZFar()))
			{
				return 0;
			}

			return (long)DefWindowProc(hWnd, msg, wParam, lParam);
		}

		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
		{			
			using namespace DifferentialArts;
			// Synchronize Local And Global Game objects and create engine 
			myGlobalGame = createEngine();
			bool renderSuccess = false;

			MSG msg;
			HWND hWnd;
			WNDCLASS wc;
			memset(&msg, 0, sizeof(MSG));	

			wc.cbClsExtra		= 0;
			wc.cbWndExtra		= 0;
			wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
			wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
			wc.hInstance		= hInstance;
			wc.lpfnWndProc		= WndProc;
			wc.lpszClassName	= CLASSNAME;
			wc.lpszMenuName		= NULL;
			wc.style			= CS_HREDRAW|CS_VREDRAW;

			gConsole = &myGlobalGame->mlog;
			gConsole->init();			

			if(!myGlobalGame->mlog.log.Init (LOG_OUTPUT))
			{		
				myGlobalGame->getApp()->MsgBoxAndQuit("\"Output.txt\" failed to create!", "Severe");	
			} else {
				myGlobalGame->getApp()->init_app(&myGlobalGame->mlog);
			}
			
			if(!myGlobalGame->SetupFromConfigurationFile("Data/Config.ini"))
			{
				myGlobalGame->mlog.LogError(LString("SetupFromConfigurationFile is FAILURE! \"Config.ini\" is missing!"));
				myGlobalGame->getApp()->MsgBoxAndQuit("Missing: config.ini", "Severe");	
				PostQuitMessage(0);
			}	

			RegisterClass(&wc);

			myGlobalGame->getApp()->setHINSTANCE(hInstance);	

			hWnd = myGlobalGame->getApp()->createWindow();	

			myGlobalGame->mlog.LogSuccessText(LString("CreateWindow is SUCCESS!"));	

			if(!hWnd)
			{
				myGlobalGame->mlog.LogError(LString("CreateWindow is FAILURE!"));
				myGlobalGame->getApp()->end();
			} else {
			}

			myGlobalGame->getApp()->lastTick = timeGetTime();

			ShowWindow(myGlobalGame->getApp()->getHandle(), SW_SHOWNORMAL);					// Show the window
			UpdateWindow(myGlobalGame->getApp()->getHandle());									// Draw the window
			SetFocus(myGlobalGame->getApp()->getHandle());	

			myGlobalGame->InitGLGAME(hWnd);	
			myGlobalGame->OutputSystemInformation();	
			

			//Initialize SOUND system
			CreateTextureFonts("Data/Textures/font.bmp");
			myGlobalGame->mlog.init();			
			
			if(!myGlobalGame->mSoundSystem->InitSoundSystem(!myGlobalGame->isUsingOpenAL(), 32))
			{
				myGlobalGame->getApp()->MsgBoxAndQuit("Couldn't initialize Sound System", "Severe");	
			} else {

				ShowWindow(myGlobalGame->getApp()->getHandle(), SW_SHOWNORMAL);					// Show the window
				UpdateWindow(myGlobalGame->getApp()->getHandle());									// Draw the window
				//SetFocus(myGlobalGame->getApp()->getHandle());

				//	Put Your initialization of render function here		
				if(myGlobalGame->InitRender())
				{
					myGlobalGame->renderInitialized();			
				} else {
					myGlobalGame->mlog.LogError(LString("CoreEngine error, InitRender Failed"));
					myGlobalGame->getApp()->end();
				}
			}					
			
			while(myGlobalGame->getApp()->isRunning())
			{
				if(PeekMessage(&msg, myGlobalGame->getApp()->getHandle(), 0, 0, PM_REMOVE))
				{
					if(msg.message == WM_QUIT)
					{
						myGlobalGame->getApp()->end();
						break;
					} else {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				} else {
					myGlobalGame->getApp()->currentTick = timeGetTime();	
					myGlobalGame->getApp()->m_FrameInterval = myGlobalGame->getApp()->currentTick - myGlobalGame->getApp()->lastTick;
					myGlobalGame->getApp()->m_fFrameIntervalSeconds = myGlobalGame->getApp()->m_FrameInterval * 0.001f;
						
					if(!myGlobalGame->mbEnginePaused)
					{
						
						//	Put Your update physics function here eg:
						
						myGlobalGame->Update(myGlobalGame->getApp()->m_fFrameIntervalSeconds);
					}

					myGlobalGame->getApp()->lastTick = myGlobalGame->getApp()->currentTick;						
						
					//	Put Your render function here				 
					if(!myGlobalGame->mbEnginePaused)
					{
						renderSuccess = myGlobalGame->Render();	
						
						myGlobalGame->mSoundSystem->mFMODSystem->update();
					}

					if (!renderSuccess)	// Active?  Was There A Quit Received?
					{
						myGlobalGame->getApp()->end();							// ESC or Render Signalled A Quit
					} else {
						myGlobalGame->getApp()->UpdateKeyReleased();
						myGlobalGame->getApp()->Mouse.mouseWheel = 0;
						myGlobalGame->getApp()->ResetMouseRelease();
						SwapBuffers(myGlobalGame->getApp()->getDC());			
					}			

					myGlobalGame->getApp()->tFPS++;			
					if(myGlobalGame->getApp()->currentTick - myGlobalGame->getApp()->frLastTime > 1000)
					{
						myGlobalGame->getApp()->frLastTime = (float)myGlobalGame->getApp()->currentTick;
						myGlobalGame->getApp()->m_FPS = myGlobalGame->getApp()->tFPS;
						myGlobalGame->getApp()->tFPS = 0;
					}	
				}
			}

			myGlobalGame->Release();

			FreeTextures();	

			// Shutdown Sound system (common for OpenAL and FMOD)
			
			myGlobalGame->mSoundSystem->DeInitSoundSystem();

			myGlobalGame->DeInit();					

			return 0;
		}
	#endif
#endif

