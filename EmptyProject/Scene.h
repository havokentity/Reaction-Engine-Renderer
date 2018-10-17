/*
	$Author: Rajesh Peter Douglas D'Monte		
*/

#ifndef MY_SCENE_H
#define MY_SCENE_H

#include "../SourceFiles/TypesClient.h"

#include "../SourceFiles/CoreEngine.h"
#include "../SourceFiles/camera.h"
#include "../SourceFiles/HDR.h"
#include "../SourceFiles/Shader.h"
#include "../SourceFiles/Font.h"
#include "../SourceFiles/3DS.h"
#include "../SourceFiles/ApplicationEntryPoint.h"
	
//#define COMPILE_MARIA_APP
//#define COMPILE_KARTHIK_APP
#define COMPILE_TEST_APP
//PEO

#ifdef COMPILE_MARIA_APP

class CMariaPaneer:public CoreEngine
{
public:
	CMariaPaneer::CMariaPaneer()
	{
		this->useEngineSound(false);
		this->set(90, 1, 10000, "Smoke weed!");
	}

	virtual bool Render(void)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		mFBO.BeginRender();
		glClear(GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		myCamera.Look();		
		this->mSkyBox.Render(myCamera.Position());

		/*
		myEffect.BindFirstPass();
		myTexture.bindTextureToTextureUnit(0);


		
		for(int x = -10; x < 10; x++)
		{
			cgGLSetParameter4f(time,
						   (1 + 1*(((this->spectrum[0 + x + 10] + this->spectrum[1 + x + 10] + this->spectrum[3 + x + 10])/3))*1),
						   1 + this->spectrum[32 + x + 10]*20,
						   1,
						   mTime);
			vel[x+10] += Abs((Abs(x)-10)) * (1*(((this->spectrum[0 + x + 10] + this->spectrum[1 + x + 10] + this->spectrum[3 + x + 10])/3))*100);
			vel[x+10] -=  vel[x+10]*0.1f;
			glPushMatrix();
			//glScalef(50, 1, 50);
			glTranslatef(x * 60, vel[x+10] , 0);
			gluSphere(pObject, 30, 6, 6);
			glPopMatrix();
		}

		myEffect.BindNextPass();
		myTexture.bindTextureToTextureUnit(0);
		cgGLSetParameter4f(time,
						   (1 + 1*(((this->spectrum[0] + this->spectrum[1] + this->spectrum[3])/3))*1),
						   1 + this->spectrum[32]*20,
						   1,
						   mTime);

		glPushMatrix();
		glTranslatef(0, 0 , 500);
		gluSphere(pObject, 360, 100, 100);
		glPopMatrix();

		//float dbz = 100;
		//lReadPixels(512, 512, 1, 1, GL_ALPHA, GL_FLOAT, &dbz); 	

		myEffect.ResetLastPass();	*/	

		mFBO.EndRender();		
		mHDR.Render(&mFBO);		

		Print(5, 5, 0, "FPS: %d Time: %f", myGlobalGame->getApp()->GetFrameRate(), mTime);

		this->mlog.mConsole.draw();	

		this->mGUISystem->renderGUI();

		return true;
	}

	virtual void Update(float dt)
	{
		this->groupMusic1->getSpectrum(this->spectrum, 128, 0, FMOD_DSP_FFT_WINDOW_RECT);

		if(!this->mlog.mConsole.isActivated())
		{
			myCamera.Update();
			myCamera.UpdateStrafe();
		}

		mTime += dt;

		this->mlog.mConsole.operate(dt);

		if(this->getApp()->keyReleased(RX_TILDE))
		{
			this->mlog.mConsole.toggleActivate();
		}	

		if(this->getApp()->keyReleased(RX_SPACE))
		{
			myEffect.Free();
			myEffect.LoadEffect("Data/shaders/prashanth.cgfx");
			myEffect.AssignParameterToEffect(time, "time");
			myEffect.obtainValidatedTechnique();
		}
	}

	bool testQuit(const CEGUI::EventArgs& pEventArgs)
	{
		this->getApp()->end();
		return true;
	}

	virtual bool InitRender(void)
	{
		this->mlog.mConsole.init();
		this->DisableVSYNC();

		this->InitializeShaders(true);

		this->mTextureStateSystem.mTextureState2D.Enable();
		myCamera.PositionCameraera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));

		CreateTextureFonts("Data/Textures/font.bmp");

		pObject = gluNewQuadric();

		gluQuadricNormals(pObject, GL_SMOOTH);
		gluQuadricTexture(pObject, GL_TRUE);	

		myTexture.LoadTextureImage_MipMaps("Data/cloud.bmp");
		myEffect.LoadEffect("Data/shaders/prashanth.cgfx");
		myEffect.AssignParameterToEffect(time, "time");
		myEffect.obtainValidatedTechnique();	

		CameraStatic.PositionCameraera( Vector3(0, 100, -20), Vector3(0, 0, 0), Vector3(0, 1, 0));

		mHDR.Initialize(false, 1024, 1024);
		mFBO.Initialize(1024, 1024, false, GL_RGBA, GL_RGBA16F_ARB, GL_FLOAT, 0, 0, GL_LINEAR, GL_LINEAR, GL_TEXTURE_2D, true, true);		
		mHDR.setLensExposure(0.8f);
		mHDR.setBlurAmount(0.0f);

		this->mSkyBox.CreateSkybox("Data/skybox/", Vector3(1000, 1000, 1000), true);

		LoadSoftwareStreamLoop("C:\\Documents and Settings\\DeMonte\\My Documents\\My Music\\06 Lost Souls Forever.mp3", &music);
		musicSource.setSound(music, this->groupMusic1);
		//musicSource.play();

		this->mLightHandler.light0.pos = Vector3(10, 10, 10);
		this->mLightHandler.light0.UpdateAPILight_pos_only();

		this->groupMusic1->setVolume(0.5f);

		CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");		
		CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");		
		CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
		CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font"); 


		CEGUI::WindowManager& wMgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window* main = wMgr.getWindow((CEGUI::utf8*)"Root"); 

		main->addChildWindow( CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"test.layout") ); 
		main->addChildWindow( CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"test2.layout") );

		//CEGUI::WindowManager::getSingleton()::
		
		try
		{
			CEGUI::WindowManager::getSingleton().getWindow("Root//")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CMariaPaneer::testQuit, this));
		} catch( CEGUI::Exception& e ) {
			this->mlog.LogError(LString("CEGUI error: %s\n", e.getMessage( ).c_str( )));
		} 
		mTime = 0;		

		memset(&vel, 0, sizeof(float)*20);

		return true;
	}

	virtual void Release(void)
	{
		mFBO.Free();
		mHDR.Free();
		myTexture.Free();
		myEffect.Free();
		gluDeleteQuadric(pObject);
		musicSource.Free();
		music->release();
	}

protected:
	float spectrum[128];
	float vel[20];
	HDR mHDR;
	RenderTarget mFBO;
	FMODSource musicSource;
	FMOD::Sound* music;
	Camera myCamera, CameraStatic;
	GLUquadricObj* pObject;
	Effect myEffect;
	Texture2D myTexture;
	float mTime;
	CGparameter time;
};

bool testQuit(const CEGUI::EventArgs& pEventArgs)
{
	myGlobalGame->getApp()->end();
	return true;
}

#endif

#ifdef COMPILE_KARTHIK_APP

#include <vector>

void UpdateModel(void);
void AddModel(void);
void TotalModels(void);
void ResetRot(void);
void ResetPos(void);

class CKarthikApp:public CoreEngine
{
public:
	CKarthikApp::CKarthikApp()
	{
		this->set(90, 1, 10000.0, "Surreal Sand Box (exclusive for Karthik), from Rajesh :D");
	}

	virtual bool Render(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

		glLoadIdentity();
		myCamera.Look();

		glColor3f(1, 1, 1);
		this->mTextureStateSystem.mTextureState2D.Disable();
		if(this->mLightHandler.light0.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light0.pos.x, this->mLightHandler.light0.pos.y, this->mLightHandler.light0.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light1.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light1.pos.x, this->mLightHandler.light1.pos.y, this->mLightHandler.light1.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light2.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light2.pos.x, this->mLightHandler.light2.pos.y, this->mLightHandler.light2.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light3.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light3.pos.x, this->mLightHandler.light3.pos.y, this->mLightHandler.light3.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light4.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light4.pos.x, this->mLightHandler.light4.pos.y, this->mLightHandler.light4.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light5.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light5.pos.x, this->mLightHandler.light5.pos.y, this->mLightHandler.light5.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light6.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light6.pos.x, this->mLightHandler.light6.pos.y, this->mLightHandler.light6.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}

		if(this->mLightHandler.light7.bEnabled)
		{
			glPushMatrix();
				glTranslatef(this->mLightHandler.light7.pos.x, this->mLightHandler.light7.pos.y, this->mLightHandler.light7.pos.z);
				gluSphere(this->pSphere, 1, 10, 10);
			glPopMatrix();
		}
		
		this->mTextureStateSystem.mTextureState2D.Enable();
		this->EnableLighting();

		this->mLightHandler.light0.UpdateAPILight();
		this->mLightHandler.light1.UpdateAPILight();
		this->mLightHandler.light2.UpdateAPILight();
		this->mLightHandler.light3.UpdateAPILight();
		this->mLightHandler.light4.UpdateAPILight();
		this->mLightHandler.light5.UpdateAPILight();
		this->mLightHandler.light6.UpdateAPILight();
		this->mLightHandler.light7.UpdateAPILight();		

		for(GLuint i = 0; i < this->mMdl.size(); i++)
		{
			glPushMatrix();
				glRotatef(this->rot[i].x, 0, 0, 1);
				glRotatef(this->rot[i].y, 0, 1, 0);
				glTranslatef(this->pos[i].x, this->pos[i].y, this->pos[i].z);
				this->mMdl[i].Render();
			glPopMatrix();
		}			
		
		this->DisableLighting();

		this->mTextureStateSystem.mTextureState2D.Enable();
		crate.bindTexture();

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glNormal3f(0, 1, 0); glVertex3f(-100, -15, -100);
			glTexCoord2f(0, 4); glNormal3f(0, 1, 0); glVertex3f(100, -15, -100);
			glTexCoord2f(4, 4); glNormal3f(0, 1, 0); glVertex3f(100, -15, 100);
			glTexCoord2f(4, 0); glNormal3f(0, 1, 0); glVertex3f(-100, -15, 100);
		glEnd();

		Print(5, 20, 0, "~-Console V-ScreenShot, G-UpdateModel, <SPACE>-UserControl, ArrowKeys-Move, IJKL-Rotate");
		Print(5, 5, 0, "FPS: %d pos(%f,%f,%f)", myGlobalGame->getApp()->GetFrameRate(), this->myCamera.Position().x, this->myCamera.Position().y, this->myCamera.Position().z);
		myGlobalGame->mlog.mConsole.draw();
		return true;
	}

	virtual void Update(float dt)
	{
		if(mouse)
		{
			myCamera.Update();
			myCamera.UpdateStrafe();
		}

		if(myCamera.Position().y < 0)
		{
			myCamera.PositionCameraera( Vector3(myCamera.Position().x, 0, myCamera.Position().z), myCamera.View(), myCamera.UpVector());
		}

		myGlobalGame->mlog.mConsole.operate(dt);	

		if(myGlobalGame->getApp()->keyReleased(RX_TILDE))
		{
			mouse = myGlobalGame->mlog.mConsole.isActivated();
			myGlobalGame->mlog.mConsole.toggleActivate();	
			this->getApp()->CenterMouse();
		} else if(myGlobalGame->getApp()->keyReleased('V'))
		{
			this->SaveScreenshot("Screen");
		}

		if(mouse)
		{
			if(myGlobalGame->getApp()->keyReleased('G'))
			{
				UpdateModel();
			}

			float tRot = dt*100;

			if(myGlobalGame->getApp()->keyPressed('J'))
			{
				rot[current].y -= tRot;
			} else if(myGlobalGame->getApp()->keyPressed('L'))
			{
				rot[current].y += tRot;
			}

			if(myGlobalGame->getApp()->keyPressed('I'))
			{
				rot[current].x -= tRot;
			} else if(myGlobalGame->getApp()->keyPressed('K'))
			{
				rot[current].x += tRot;
			}

			if(myGlobalGame->getApp()->keyPressed(RX_UP))
			{
				pos[current] -= Normalize(this->myCamera.View())* tRot;
			} else if(myGlobalGame->getApp()->keyPressed(RX_DOWN))
			{
				pos[current] +=  Normalize(this->myCamera.View())*tRot;
			}

			if(myGlobalGame->getApp()->keyPressed(RX_LEFT))
			{
				pos[current] -= Normalize(this->myCamera.Strafe())* tRot;
			} else if(myGlobalGame->getApp()->keyPressed(RX_RIGHT))
			{
				pos[current] +=  Normalize(this->myCamera.Strafe())*tRot;
			}
		}

		if(myGlobalGame->getApp()->keyReleased(RX_SPACE))
		{
			mouse = !mouse;
		}		
	}

	virtual bool InitRender(void)
	{
		this->InitializeAntialiasing(16);
		glClearColor(0, 0, 0, 1);
		glEnable(GL_DEPTH_TEST);

		CreateTextureFonts("Data/Textures/font.bmp");

		this->mlog.mConsole.init();	

		crate.LoadTextureImage_MipMaps("Data/Textures/tiles.bmp");

		myCamera.PositionCameraera( Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));

		current = 0;

		strSourceFile = new char[512];
		memset(strSourceFile, 0, sizeof(char)*512);
		strcpy(strSourceFile, "Data/test.3ds");
		this->mlog.mConsole.bindStringVariable("modelPath", this->strSourceFile);

		this->mlog.mConsole.bindIntegerVariable("currentModel", &this->current);

		this->mlog.mConsole.bindCommand("update", UpdateModel);
		this->mlog.mConsole.bindCommand("totalModels", TotalModels);
		this->mlog.mConsole.bindCommand("addModel", AddModel);
		this->mlog.mConsole.bindCommand("ResetRot", ResetRot);
		this->mlog.mConsole.bindCommand("ResetPos", ResetPos);

		glMatrixMode(GL_MODELVIEW);
		this->mLightHandler.light0.bEnabled = true;

		AddModel();

		this->pSphere = gluNewQuadric();
		gluQuadricNormals(this->pSphere, GL_SMOOTH);
		gluQuadricTexture(this->pSphere, GL_TRUE);

		this->mMdl[current].Load(this->strSourceFile);

		this->mTextureStateSystem.mTextureState2D.Enable();		

		mouse = true;

		return true;
	}

	virtual void Release(void)
	{
		for(GLuint i = 0; i < this->mMdl.size();i++)
		{
			mMdl[i].Free();			
		}

		gluDeleteQuadric(pSphere);

		delete[] strSourceFile;
	}

	protected:
		Camera myCamera;
		Texture2D crate;
		bool mouse;
		int current;
		std::vector<C3DS> mMdl;
		std::vector<Vector3> pos;
		std::vector<Vector2> rot;
		char* strSourceFile;
		GLUquadricObj* pSphere;

		friend void UpdateModel(void);
		friend void AddModel(void);
		friend void TotalModels(void);
		friend void ResetRot(void);
		friend void ResetPos(void);
};

void ResetRot(void)
{
	CKarthikApp* tempApp;

	tempApp = (CKarthikApp*)myGlobalGame;

	if((GLuint)tempApp->current >= tempApp->mMdl.size())
	{
		tempApp->current = tempApp->mMdl.size()-1;
	} else if(tempApp->current < 0)
	{
		tempApp->current = 0;
	}

	tempApp->rot[tempApp->current].x = 0;
	tempApp->rot[tempApp->current].y = 0;
}

void ResetPos(void)
{
	CKarthikApp* tempApp;

	tempApp = (CKarthikApp*)myGlobalGame;

	if((GLuint)tempApp->current >= tempApp->mMdl.size())
	{
		tempApp->current = tempApp->mMdl.size()-1;
	} else if(tempApp->current < 0)
	{
		tempApp->current = 0;
	}

	tempApp->pos[tempApp->current] = Vector3(0, 0, 0);
}

void AddModel(void)
{
	CKarthikApp* tempApp;

	tempApp = (CKarthikApp*)myGlobalGame;

	C3DS temp;

	Vector3 tempVec = Vector3(0, 0, 0);
	Vector2 tempVec2 = Vector2(0, 0);
	temp.Load(tempApp->strSourceFile);
	
	tempApp->mMdl.push_back(temp);
	tempApp->pos.push_back(tempVec);
	tempApp->rot.push_back(tempVec2);

	tempApp->current = tempApp->mMdl.size()-1;
}

void TotalModels()
{
	CKarthikApp* tempApp;

	tempApp = (CKarthikApp*)myGlobalGame;

	char temp[256];

	sprintf(temp, "Total models: %d", tempApp->mMdl.size());

	myGlobalGame->mlog.mConsole.addString(temp);
}

void UpdateModel(void)
{
	CKarthikApp* tempApp;

	tempApp = (CKarthikApp*)myGlobalGame;

	if((GLuint)tempApp->current >= tempApp->mMdl.size())
	{
		tempApp->current = tempApp->mMdl.size()-1;
	} else if(tempApp->current < 0)
	{
		tempApp->current = 0;
	}

	tempApp->mMdl[tempApp->current].Free();
	tempApp->mMdl[tempApp->current].Load(tempApp->strSourceFile);

	myGlobalGame->mlog.mConsole.addString("Updated!");
}

#endif

#ifdef COMPILE_TEST_APP

#ifdef NDEBUG
#pragma comment(lib, "PhysicsSimulator.lib")
#else
#pragma comment(lib, "PhysicsSimulatorD.lib")
#endif

#include "../SourceFiles/RxBezierPath.h"
#include "../SourceFiles/RxSpectrumDisplay.h"
#include "../SourceFiles/Skydome.h"
#include "../SourceFiles/ParticleSystem.h"
#include "PhysicsSimulator.h"
#include "../SourceFiles/Texture.h"
#include "../SourceFiles/Shader.h"
#include "../SourceFiles/VertexBuffer.h"
#include "../SourceFiles/RenderTarget.h"
#include "../SourceFiles/CelShader.h"
#include "../SourceFiles/HDR.h"

VertexSingleTextureCoord vertices[] = 
{
	{ 1, 0, 0, 1, 0, -100, -15, 100},
	{ 1, 1, 0, 1, 0, 100, -15, 100},
	{ 0, 1, 0, 1, 0, 100, -15, -100},
	{ 0, 0, 0, 1, 0, -100, -15, -100}
};

void UpdateLighting(void);

class CTestApp:public CoreEngine
{
public:
	CTestApp()
	{
		this->useEngineSound(false);
		this->set(90.0f, 1, 10000.0, "Rajesh's Sand Box");
	}

	void updateShadowMap(int shadowID)
	{
		this->mShadow[shadowID].BeginRender(0, 0);		
		
		
		for(int i=0;i<6;i++)
		{
			this->mShadow[shadowID].setFace(i);

			//glClearColor(0, 1, 1, 1);
			glClear(GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			this->SetupCubeMapView(i, this->mLightHandler.light[shadowID].pos);

			//this->mSkyBox.Render(Vector3(0, 0, 0));

			glPushMatrix();
				glTranslatef(20, -10, 20);	
				//testMesh->RenderAll(true);
				testModelShadow.Render();
			glPopMatrix();
			//this->mSkyBox.Render(Vector3(0, 0, 0));
			//this->DisableTextures_2D();
			
			//_3ds.Render_SLOW(0, 0, 0);
			//_3ds.Render();
			glPushMatrix();
			glScalef(1 + this->spectrum[32]*20, 1 + 1*(((this->spectrum[0] + this->spectrum[1] + this->spectrum[3])/3))*10, 1 + this->spectrum[48]*20);
			glTranslatef(-150, 0, 0);
			city.Render();
			glPopMatrix();
			
			this->mVBO.RenderTEX1(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);
					
			//glFlush();
		}

		//this->disEffect.ResetLastPass();
		this->mShadow[shadowID].EndRender();		
		
	}

	virtual bool Render(void)
	{	
		static int i = 0;
		i++;

		this->disEffect.BindFirstPass();
		if(bUpdateShadow)
		{			
			//this->updateShadowMap(0);		
			bUpdateShadow = false;
		}
	
		if(i % 60 == 0)
		{
			this->updateShadowMap(0);			
			this->updateShadowMap(1);	
			this->updateShadowMap(2);
		}		

		//this->disEffect.ResetLastPass();

		glClear(GL_DEPTH_BUFFER_BIT);		 
		
		mMultiSampleFBO.BeginRender(0, 0);
		//mFBO.BeginRender(0, 0);		
		glClear(GL_DEPTH_BUFFER_BIT);			

		glLoadIdentity();
		myCamera.Look();

		float mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		cgGLSetMatrixParameterfr(this->cameraMatrix, mat);

		if (move->mMouseSphere)
		{
				move->MoveActor();
		}
		if(myGlobalGame->getApp()->Mouse.RightButton.isDown())
		{
			if(move->mMouseSphere == NULL)
			{
				move->PickActor();
			} 
		}
		else 
		{
			move->LetGoActor();
		}	
					
		skyboxEffect.BindFirstPass();

		this->mSkyBox.Render();

		cgGLSetupSampler(this->shadowMap[0], this->mShadow[0].mTexture.getTexture());
		cgGLSetupSampler(this->shadowMap[1], this->mShadow[1].mTexture.getTexture());
		cgGLSetupSampler(this->shadowMap[2], this->mShadow[2].mTexture.getTexture());

		perPixelLightingEffect.BindNamedPass("ppl_D");
		
		//this->mLightHandler.light0.pos = Vector3(pos[0], pos[1], pos[2]);
		this->mLightHandler.light[0].UpdateAPILight_pos_only();

		this->mLightHandler.light[1].pos = flamePos + Vector3(((rand()%1024)/256)*0.2f, ((rand()%1024)/256)*0.2f, ((rand()%1024)/256)*0.2f);
		this->mLightHandler.light[1].UpdateAPILight_pos_only();

		this->mLightHandler.light[1].UpdateAPILight_attenuation_only();

		this->mLightHandler.light[2].UpdateAPILight_pos_only();		

		//cgGLSetMatrixParameterfr(
		//cgGLSetStateMatrixParameter(this->prevMV, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
		//cgGLSetStateMatrixParameter(this->prevMVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);			
		
		glPushMatrix();
		glScalef(1 + this->spectrum[32]*20, 1 + 1*(((this->spectrum[0] + this->spectrum[1] + this->spectrum[3])/3))*10, 1 + this->spectrum[48]*20);
		//glScalef(1, 1 + this->spectrum[5]*100, 1);
		glTranslatef(-150, 0, 0);	
		city.Render();
		glPopMatrix();		
		
		brass.bindTexture();
		
		this->mVBO.RenderTEX1(GL_QUADS, VertexBuffer::BEGIN_DEFAULT, VertexBuffer::TOTAL_COUNT);			

		perPixelLightingEffect.BindNamedPass("ppl_DGNP");
		
		this->rock.bindTextureToTextureUnit(0);		
		this->rockNormal.bindTextureToTextureUnit(1);	
		
		glColor4f(1, 0, 0, 1);
		glPushMatrix();
			glTranslatef(20, -10, 20);			
			//testMesh->RenderAll(false);
			testModel.Render();
		glPopMatrix();

		this->mTextureStateSystem.mTextureState2D.Disable();
		this->mTextureStateSystem.SetActiveTextureUnit(0);

		perPixelLightingEffect.BindNamedPass("ps_D");
		pSystem->Render();

		perPixelLightingEffect.ResetLastPass();	

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glDisable(GL_LIGHTING);
		//perPixelLightingEffect.BindNamedPass("ps_D");
		//pSystem->Render(&this->myCamera);
		glPushMatrix();
		glTranslatef(100, 0, 0);
		pSystem->RenderPointSprites();
		glPopMatrix();
		glDisable(GL_BLEND);
		//this->EndParticleRender();	

		//perPixelLightingEffect.ResetLastPass();	
			
		glColor3f(1, 1, 0.5f);

		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(this->mLightHandler.light[0].pos.x, this->mLightHandler.light[0].pos.y, this->mLightHandler.light[0].pos.z);
		gluSphere(this->myQuadric, 2, 8, 8);
		glPopMatrix();	

		if(myGlobalGame->getApp()->keyPressed(RX_TAB))
		{
			glPushMatrix();
			glTranslatef(this->myCamera.View().x, this->myCamera.View().y, this->myCamera.View().z);
			gluSphere(this->myQuadric, 1, 8, 8);
			glPopMatrix();
		}

		mCurveEditor.debugDraw();	

		//this->mSkyDome.Render(this->myCamera.Position().x, this->myCamera.Position().y - 1000, this->myCamera.Position().z);		
		
		mMultiSampleFBO.blitMultisampleBufferToStandardFBO(this->mFBO);
		mMultiSampleFBO.EndRender();
		
		//float dbz = 100;
		//glReadPixels(512, 512, 1, 1, GL_ALPHA, GL_FLOAT, &dbz);
		this->mHDR.setDOF(dbz);
		//mFBO.EndRender();		
		
		mHDR.Render(&mFBO, bFishEye);	

		if(bFishEye)
		{
			this->ChangeFOV(120.0f);
		} else {
			this->ChangeFOV(90.0f);
		}
		
		this->mTextureStateSystem.mTextureState2D.Enable();

		glColor4f(1, 1, 1, 1);
		Print(5, 5, 0, "FPS: %d pos: %f %f %f Mouse(%d, %d), Depth: %f", myGlobalGame->getApp()->GetFrameRate(),
																this->CameraStatic.Position().x, this->CameraStatic.Position().y,
																this->CameraStatic.Position().z,
																myGlobalGame->getApp()->Mouse.x,
																myGlobalGame->getApp()->Mouse.y,
																dbz);
		myGlobalGame->mlog.mConsole.draw();				

		spectrumDisplay.Render(this->spectrum, 128);	
		
		this->mGUISystem->renderGUI();		

		return true;
	}	

	virtual void Update(float dt)
	{			
		myGlobalGame->mLuaScriptEngine->Update(dt);

		this->mSoundSystem->groupMusic1->getSpectrum(this->spectrum, 128, 0, FMOD_DSP_FFT_WINDOW_RECT);

		if(myGlobalGame->getApp()->keyPressed('U'))
		{
			this->dbz += 1;
		} else if(myGlobalGame->getApp()->keyPressed('J'))
		{
			this->dbz -= 1;
		} else if(myGlobalGame->getApp()->keyReleased('Z'))
		{
			bFishEye = !bFishEye;
		}

		if(myGlobalGame->getApp()->keyReleased('G'))
		{
			bUpdateShadow = true;
			this->mLightHandler.light[0].pos = this->mCurveEditor.mCurvePath.PlayTillEnd(dt);
		} else if(myGlobalGame->getApp()->keyReleased('Y'))
		{			
			flamePos = this->mCurveEditor.mCurvePath.PlayTillEnd(dt);
		} else if(myGlobalGame->getApp()->keyReleased('T'))
		{			
			this->mLightHandler.light[2].pos = this->mCurveEditor.mCurvePath.PlayTillEnd(dt);
		}else if(myGlobalGame->getApp()->keyReleased('B'))
		{
			Vector3 temp, temp2, ptemp;

			temp = this->mCurveEditor.mCurvePath.PlayTillEnd(dt);
			temp2 = this->mCurveEditor.mCurvePath.PlayTillEnd(dt+0.001f);

			ptemp = this->myCamera.Position();
			ptemp -= (ptemp - temp2)*0.01f;
			
			this->myCamera.PositionCameraera(temp, ptemp, this->myCamera.UpVector());
		} else if(myGlobalGame->getApp()->keyReleased('H'))
		{
			this->mCurveEditor.mCurvePath.SetHead(0);
		} else if(myGlobalGame->getApp()->keyReleased(RX_SPACE))
		{
			this->bMouseEnable = !this->bMouseEnable;
			this->getApp()->Mouse.CenterScreen(this->getApp()->windowPosition, this->getApp()->getDimensions(),
											   this->getApp()->getSystemResolution());
		}

		if(bMouseEnable)
		{
			myCamera.SetViewByMouse();
			myCamera.GetKeyInput();
		}

		myCamera.UpdateStrafe();
		myGlobalGame->mlog.mConsole.operate(dt);	

		mCurveEditor.Update(this->myCamera.View(), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0));

		physicsSystem->updateSimulation(dt);		

		this->getApp()->m_fGameTime += dt;

		if(myGlobalGame->getApp()->keyReleased(RX_TILDE))
		{
			myGlobalGame->mlog.mConsole.toggleActivate();				
		} else if(myGlobalGame->getApp()->keyReleased(RX_ADD))
		{
			this->mHDR.IncreaseExposureBySteps();
		} else if(myGlobalGame->getApp()->keyReleased(RX_SUBTRACT))
		{
			this->mHDR.DecreaseExposureBySteps();
		} else if(myGlobalGame->getApp()->keyReleased('V'))
		{
			this->SaveScreenshot("Screen", "jpg");
		} else if(myGlobalGame->getApp()->keyReleased('P'))
		{
			this->mFBO.mTexture[0].saveImageToDisk("c:\\FBO.tga");
		} else if(myGlobalGame->getApp()->keyReleased('F'))
		{			
			skyboxEffect.LoadEffect("Data/Shaders/skybox.cgfx");
			//skyboxEffect.obtainValidatedTechnique();

			this->mHDR.ReloadHDREffect();
			mHDR.setLensExposure(0.8f);
			mHDR.setBlurAmount(0.25f);
			mHDR.setGamma(1.5f);			
			mHDR.setRadialBlurAmount(1);
			mHDR.setRadialScaleDown(0.95f);
			mHDR.SetRadialCardinalOrigin(0, 1);	
			mHDR.setFocalFallOff(200.0f, 300.0f);
		}

		this->mLightHandler.light[1].setAttenuation(Vector3(((rand()%1024)/256)*0.05f, ((rand()%1024)/256)*0.007f, 0.0009f));

		pSystem->operate(dt);
		this->fireSoundSource.set(this->mLightHandler.light[1].pos, Vector3(0, 0, 0));
		pSystem->pos = this->mLightHandler.light[1].pos;

		this->mSoundSystem->SetListenerPositionAndVelocity(this->myCamera.Position(), Vector3(0, 0, 0));
		this->mSoundSystem->SetListenerOrientation(Normalize(this->myCamera.View() - this->myCamera.Position()), Vector3(0, 1, 0));
		//this->mHDR.UpdateOverlayTime(this->getApp()->fGameTime*10);
		this->mSkyDome.UpdateTime(this->getApp()->m_fGameTime*0.01f);
    }

	virtual bool InitRender(void)
	{
		//this->InitializeAntialiasing(16);
		this->InitializeShaders(true, true);
		this->InitializeMSAA();
		this->InitializeCSAA();
		this->Initialize3DTextures();		

		bUpdateShadow = false;

		this->DisableVSYNC();	
		
		glEnable(GL_DEPTH_TEST);				

		glClearColor(0, 0, 0, 1);	
		glClear(GL_COLOR_BUFFER_BIT);

		//this->mlog.mConsole.init();				

		physicsSystem = new CPhysicsSimulator();
		
		physicsSystem->Init();

		myCamera.PositionCameraera( Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
		CameraStatic.PositionCameraera( Vector3(512, 384, -382), Vector3(512, 384, 0), Vector3(0, -1, 0));
		
		this->myQuadric = gluNewQuadric();
		gluQuadricNormals(this->myQuadric, GL_SMOOTH);
		gluQuadricTexture(this->myQuadric, GL_TRUE);

		glMatrixMode(GL_MODELVIEW);

		this->mSkyBox.CreateSkybox("Data/skybox/", Vector3(5000, 5000, 5000), true);
		//this->mSkyBox.CreateSkyboxFromCrossCubeMap("Data/grace_new_cross.hdr", Vector3(5000, 5000, 5000), false);		
		
		this->mSkyDome.Load("Data/skydome.3ds", "Data/Cloud.bmp");

		this->mVBO.InitializeTEX1(4, vertices);		

		mHDR.Initialize(0, 768, 768, true, true);
		//mHDR.overlayTexture.LoadTextureImage("Data/noiseOverlay.bmp", true);
		mFBO.Initialize(768, 768, false, GL_RGBA, GL_RGBA16F_ARB, GL_FLOAT, 0, 0,
					    GL_LINEAR, GL_LINEAR, GL_TEXTURE_2D, false, false, 2, true, true, true);		
		mMultiSampleFBO.Initialize(768, 768, false, GL_RGBA, GL_RGBA16F_ARB, GL_FLOAT, 8, 8,
								   GL_LINEAR, GL_LINEAR, GL_TEXTURE_2D, true, true, 2, true, true, false, false);

		/*
		mMultiSampleFBO.PopDrawBufferForMRT();
		mMultiSampleFBO.UpdateMRTEnumBuffer();
		mMultiSampleFBO.UnBind();*/
		
		mHDR.setLensExposure(0.8f);
		mHDR.setBlurAmount(0.25f);
		mHDR.setGamma(1.5f);			
		mHDR.setRadialBlurAmount(1);
		mHDR.setRadialScaleDown(0.95f);
		mHDR.SetRadialCardinalOrigin(0, 1);	
		mHDR.setFocalFallOff(200.0f, 300.0f);

		mShadow[0].Initialize(1024, 1024);
		mShadow[1].Initialize(1024, 1024);
		mShadow[2].Initialize(1024, 1024);
		mShadow[2].UnBind();

		skyboxEffect.LoadEffect("Data/Shaders/skybox.cgfx");
		//skyboxEffect.obtainValidatedTechnique();
 
		this->normalizationMapCubeMap.GenerateNormalizationCubeMap(512);

		this->mlog.mConsole.bindCommand("ule", UpdateLighting);

		//rock.LoadTextureImage("Data/16tile01.tga", true);		
		//rockNormal.LoadTextureImage("Data/16tile01_nrm.tga", true);
		//rock.LoadTextureImage("Data/STATUE.jpg", true);		
		//rockNormal.LoadTextureImage("Data/STATUED.tga", true);
		rock.LoadTextureImage("Data/16tile01.tga", true);		
		rockNormal.LoadTextureImage("Data/16tile01_nrm.tga", true);

		afterburn.LoadTextureImage("Data/Afterburn.bmp", true);
		brass.LoadTextureImage("Data/Textures/plain.bmp", true);	

		this->mSoundSystem->mFMODHelper.LoadStreamLoop("Data/Sounds/fire.wav", &fireSnd);	
		
		fireSnd->set3DMinMaxDistance(2, 100000);		
		fireSoundSource.setSound(this->fireSnd, this->mSoundSystem->groupSounds1);
		fireSoundSource.play();

		this->mSoundSystem->mFMODHelper.LoadSoftwareStreamLoop("Data/Sounds/_music.mp3", &music);
		musicSource.setSound(this->music, this->mSoundSystem->groupMusic1);
		musicSource.play();		

		this->mSoundSystem->groupMusic1->setVolume(0.5f);

		pSystem = new ParticleSystem();
		pParticle = new Particle();

		//this->mHDR.setBlurAmount(0.5f);
		
		pParticle->Initialize_from_file("Data/SparkyFire.pt");
		pSystem->Initialize_from_file_SETBASE("Data/Explosion1.psy", afterburn.getTexture(), pParticle);

		this->mLightHandler.light[0].setAttenuation(Vector3(0.000001f, 0.00001f, 0.00002f));
		//this->mLightHandler.light[0].setAttenuation(Vector3(0.0001f, 0.001f, 0.002f));
		//this->mLightHandler.light[0].diffuse = Vector3(0.8f, 0.6f, 0.5f);
		this->mLightHandler.light[0].specular = Vector3(0, 0, 0);
		this->mLightHandler.light[0].UpdateAPILight();

		this->mLightHandler.light[1].diffuse = Vector3(pParticle->color);
		this->mLightHandler.light[1].specular = Vector3(pParticle->color* 1);
		this->mLightHandler.light[1].ambient = Vector3(0, 0, 0);
		this->mLightHandler.light[1].setAttenuation(Vector3(0.002f, 0.007f, 0.0009f));

		this->mLightHandler.light[1].UpdateAPILight();

		this->mLightHandler.light[2].diffuse = Vector3(0.6f, 0.1f, 0.5f);
		this->mLightHandler.light[2].specular = Vector3(0.8f, 0.8f, 0.8f);
		this->mLightHandler.light[2].ambient = Vector3(0, 0, 0);
		this->mLightHandler.light[2].setAttenuation(Vector3(0.000002f, 0.007f, 0.000009f));

		this->mLightHandler.light[2].UpdateAPILight();

		city.Load("Data/city.3ds");			

		dbz = 100;
		this->mHDR.setFocalFallOff(100.0f);

		move = new CActorMouseControl();
		move->Init(1.0, 10, 100, 10, physicsSystem);	

		testMesh.Init("c:\\sph.rxs", 0);		

		testModel.Init(testMesh, VBO_BUMP_MAP);
		testModelShadow.Init(testMesh, VBO_SHADOW);

		CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");		
		CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");	
		CEGUI::SchemeManager::getSingleton().loadScheme("VanillaSkin.scheme");
		CEGUI::System::getSingleton().setDefaultMouseCursor("Vanilla-Images", "MouseArrow");
		CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font"); 
		CEGUI::FontManager::getSingleton().createFont("DejaVuSans-10.font"); 
		CEGUI::FontManager::getSingleton().createFont("fkp-16.font"); 

		testScript = myGlobalGame->mLuaScriptEngine->NewScript();
		testScript->LoadLuaScriptFile("Data/test.lua");

		this->mlog.LogWarning(LString("A dumb ass warning"));
		this->mlog.LogError(LString("A dumb ass error"));
		this->mlog.LogCritical(LString("A dumb ass critical error"));

		UpdateLighting();

		mCurveEditor.Initialize();

		this->getApp()->Mouse.Hide();
		bMouseEnable = true;
		bFishEye = false;			

		return true;
	}	

	virtual void Release(void)
	{	
		gluDeleteQuadric(myQuadric);
		
		perPixelLightingEffect.Free();	
		mHDR.Free();
		mFBO.Free();
		mMultiSampleFBO.Free();

		mVBO.Free();

		mShadow[0].Free();
		mShadow[1].Free();
		mShadow[2].Free();

		physicsSystem->Free();
		
		disEffect.Free();
		city.Free();
		this->mSkyDome.Free();
		rock.Free();
		rockNormal.Free();
		afterburn.Free();
		fireSoundSource.Free();
		musicSource.Free();
		skyboxEffect.Free();
		fireSnd->release();
		music->release();
		brass.Free();
		normalizationMapCubeMap.Free();	

		pSystem->Free();
		FREE(pSystem);
		FREE(pParticle);
	}

	Effect perPixelLightingEffect, disEffect, skyboxEffect;
	CGparameter environmentMap, normalizationMap, shadowMap[3], lightWorld, time, cameraMatrix;

	CSkin testMesh;
	CModelVBO testModel, testModelShadow;

	bool bUpdateShadow, bFishEye;

	//Mouse control
	CActorMouseControl *move;
	bool bMouseEnable;

	LuaScript* testScript;

protected:
	//NXU::NxuPhysicsCollection *collada;
	float spectrum[128], dbz;
	Vector3 flamePos;

	ParticleSystem* pSystem;
	Particle* pParticle;

	FMODSource fireSoundSource;
	FMODSource musicSource;
	FMOD::Sound* fireSnd;
	FMOD::Sound* music;

	HDR mHDR;
	C3DS city;
	VertexBuffer mVBO;
	RenderTarget mFBO, mMultiSampleFBO;
	
	RenderTarget_ShadowCubeMap mShadow[3];

	Camera myCamera, CameraStatic;
	GLUquadricObj* myQuadric;
	
	Vector3 mouseSave;
	Texture2D rock, rockNormal, afterburn, brass;
	SkyDome mSkyDome;
	TextureCube normalizationMapCubeMap;
	CPhysicsSimulator *physicsSystem;

	CurveEditorUI mCurveEditor;
	SpectrumDisplayUI spectrumDisplay;
};

void UpdateLighting(void)
{
	CTestApp* tempApp;

	tempApp = (CTestApp*)myGlobalGame;

	tempApp->perPixelLightingEffect.Free();
	tempApp->perPixelLightingEffect.LoadEffect("Data/Shaders/perpixelLighting.cgfx");
	//tempApp->perPixelLightingEffect.obtainValidatedTechnique();
	tempApp->perPixelLightingEffect.AssignParameterToEffect(tempApp->shadowMap[0], "shadowMap[0]");
	tempApp->perPixelLightingEffect.AssignParameterToEffect(tempApp->shadowMap[1], "shadowMap[1]");
	tempApp->perPixelLightingEffect.AssignParameterToEffect(tempApp->shadowMap[2], "shadowMap[2]");	
	tempApp->perPixelLightingEffect.AssignParameterToEffect(tempApp->cameraMatrix, "cameraMatrix");	

	tempApp->disEffect.Free();
	tempApp->disEffect.LoadEffect("Data/Shaders/shadowmap.cgfx");
	//tempApp->disEffect.obtainValidatedTechnique();

	myGlobalGame->mlog.mConsole.addString("Lighting Effect updated");
}

#endif

#endif