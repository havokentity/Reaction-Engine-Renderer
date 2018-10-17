/*
	Description: Consists of Camera class, and Math::Quaternion Camera class

	The classes contained here are,
	class Camera
	class quatCamera
	
*/

/*
	NOTE:
	----
	Look at Camera.h for further comments
*/

#define NOMINMAX

#include "Camera.h"

namespace DifferentialArts
{
	int Camera::mTotalCameras = 0;

	/*======================================================================================
	//																	
	//	Camera Constructor											    
	//																
	*======================================================================================*/

	Camera::Camera(void)
	{
		m_fSpeed = 100.0f;
		Math::Vector3 vZero(0, 0, 0);
		Math::Vector3 vView(0, 0, 0);
		Math::Vector3 vUp(0, 1, 0);

		this->mfTime = 0.0f;
		this->mfMag = 0.0f;
		this->mfTotalTime = 0.0f;
		this->mbIncrease = false;

		m_vPosition = vZero;
		m_vView = vView;
		m_vUpVector = vUp;	

		bInit = false;
	}

	void Camera::BindCameraVariables(void)
	{
		if(!bInit)
		{
			char strTempStr[256];

			sprintf(strTempStr, "Camera[%d].speed", Camera::mTotalCameras);
			myGlobalGame->mlog.bindFloatVariable(strTempStr, &this->m_fSpeed);

			sprintf(strTempStr, "Camera[%d].pos", Camera::mTotalCameras);
			myGlobalGame->mlog.bindFloatVariable(strTempStr, &this->m_vPosition.x, &this->m_vPosition.y,
				&this->m_vPosition.z);

			Camera::mTotalCameras++;

			bInit = true;
		} else {
			myGlobalGame->mlog.LogError(LString("Camera can only be bound once!"));
		}
	}
	Camera::Camera(Camera &Camera)
	{		
		this->m_vPosition = Camera.Position();
		this->m_vView = Camera.View();
		this->m_vUpVector = Camera.UpVector();	
	}

	RX_API void Camera::SetSpeed(float speed)
	{
		this->m_fSpeed = speed;
	}

	/*======================================================================================
	//																	
	//	This positions our Camera in 3D space   					    
	//																
	*======================================================================================*/

	void Camera::PositionCamera(const Math::Vector3 &pos, const Math::Vector3 &view, 
								const Math::Vector3 &up)
	{
		m_vPosition = pos;
		m_vView = view;
		m_vUpVector = up;
	}

	/*======================================================================================
	//																	
	//	This Rotate's the view of our Camera in 3D space   				
	//																	
	*======================================================================================*/

	void Camera::RotateCamera(float angle, float x, float y, float z)
	{
		Math::Vector3 vView, vNewView;

		vView = m_vView - m_vPosition;

		float sinTheta = Math::sin(angle);
		float cosTheta = Math::cos(angle);

		vNewView.x =  (cosTheta + (1 - cosTheta) * x * x)     * vView.x;
		vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vView.y;
		vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vView.z;

		vNewView.y =  ((1 - cosTheta) * x * y + z * sinTheta) * vView.x;
		vNewView.y += (cosTheta + (1 - cosTheta) * y * y)     * vView.y;
		vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vView.z;

		vNewView.z =  ((1 - cosTheta) * x * z - y * sinTheta) * vView.x;
		vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vView.y;
		vNewView.z += (cosTheta + (1 - cosTheta) * z * z)     * vView.z;	

		m_vView = m_vPosition + vNewView;
	}

	/*======================================================================================
	//																
	//	This Rotate's our Camera in 3D space about a point			  
	//																
	*======================================================================================*/

	void Camera::RotateAroundAPoint(const Math::Vector3 &vCenter, float angle, float x, float y, float z)
	{
		Math::Vector3 vNew, vNewView;

		vNew = m_vPosition - vCenter;
		
		float sinTheta = Math::sin(angle);
		float cosTheta = Math::cos(angle);

		vNewView.x =  (cosTheta + (1 - cosTheta) * x * x)     * vNew.x;
		vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vNew.y;
		vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vNew.z;

		vNewView.y =  ((1 - cosTheta) * x * y + z * sinTheta) * vNew.x;
		vNewView.y += (cosTheta + (1 - cosTheta) * y * y)	  * vNew.y;
		vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vNew.z;

		vNewView.z =  ((1 - cosTheta) * x * z - y * sinTheta) * vNew.x;
		vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vNew.y;
		vNewView.z += (cosTheta + (1 - cosTheta) * z * z)	  * vNew.z;

		m_vPosition = vCenter + vNewView;
	}

	/*======================================================================================/
	//																	
	//	Get's user Input											   
	//																
	*======================================================================================*/

	void Camera::GetKeyInput()
	{	
		if(myGlobalGame->getApp()->keyPressed('W'))
		{
			MoveCamera(m_fSpeed * myGlobalGame->getApp()->m_fFrameIntervalSeconds);
		} else if(myGlobalGame->getApp()->keyPressed('S'))
		{
			MoveCamera(-m_fSpeed * myGlobalGame->getApp()->m_fFrameIntervalSeconds);
		}

		if(myGlobalGame->getApp()->keyPressed('A'))
		{
			StrafeCamera(-m_fSpeed * myGlobalGame->getApp()->m_fFrameIntervalSeconds);
		} else if(myGlobalGame->getApp()->keyPressed('D'))
		{
			StrafeCamera(m_fSpeed * myGlobalGame->getApp()->m_fFrameIntervalSeconds);		
		}		
	}


	/*======================================================================================
	//																
	//	This set's our Camera's view by the mouse       			   
	//																
	*======================================================================================*/

	void Camera::SetViewByMouse()
	{
		MousePointInt mousePos;
		int middleX = myGlobalGame->getApp()->windowPosition.x>10000?1:myGlobalGame->getApp()->windowPosition.x + (myGlobalGame->getApp()->getDimensions().x>>1);
		int middleY = myGlobalGame->getApp()->windowPosition.y>10000?1:myGlobalGame->getApp()->windowPosition.y + (myGlobalGame->getApp()->getDimensions().y>>1);
		middleX = middleX>(int)myGlobalGame->getApp()->getSystemResolution().x?(int)myGlobalGame->getApp()->getSystemResolution().x-5:middleX;
		middleY = middleY>(int)myGlobalGame->getApp()->getSystemResolution().y?(int)myGlobalGame->getApp()->getSystemResolution().y-5:middleY;
		float angleY;
		float angleZ;
		static float currentRotX = 0.0f;
		static float lastRotX = 0.0f;

		//GetCursorPos(&mousePos);
		//mousePos.x = myGlobalGame->getApp()->Mouse.x;
		//mousePos.y = myGlobalGame->getApp()->Mouse.y;

		mousePos = myGlobalGame->getApp()->Mouse.GetImmediateCursorPosition();

		if(middleX == mousePos.x && middleY == mousePos.y)
			return;

		myGlobalGame->getApp()->CenterMouse();

		angleY = (float) (middleX - mousePos.x) * myGlobalGame->getApp()->GetMouseSensitivity();
		angleZ = (float) (middleY - mousePos.y) * myGlobalGame->getApp()->GetMouseSensitivity();

		lastRotX = currentRotX;
		currentRotX += angleZ;	
		
		if(currentRotX > 1.56f)
		{
			currentRotX = 1.56f;

			if(lastRotX != 1.56f)
			{
				Math::Vector3 vAxis = (m_vView - m_vPosition).Cross(m_vUpVector).Normalize();

				RotateCamera(1.56f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
			}
		} else if(currentRotX < -1.56f)
		{
			currentRotX = -1.56f;

			if(lastRotX != 1.56f)
			{
				Math::Vector3 vAxis = (m_vView - m_vPosition).Cross(m_vUpVector).Normalize();

				RotateCamera(-1.56f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
			}
		} else {
				Math::Vector3 vAxis = (m_vView - m_vPosition).Cross(m_vUpVector).Normalize();

				RotateCamera(angleZ, vAxis.x, vAxis.y, vAxis.z);
		}	

		RotateCamera(angleY, 0, 1, 0);
	}

	/*======================================================================================
	//																	
	//	Strafe our Camera											   
	//																
	*======================================================================================*/

	void Camera::StrafeCamera(float velocity)
	{
		m_vPosition.x += m_vStrafe.x * velocity;
		m_vPosition.z += m_vStrafe.z * velocity;

		m_vView.x += m_vStrafe.x * velocity;
		m_vView.z += m_vStrafe.z * velocity;
	}

	/*======================================================================================
	//																
	//	This move's our Camera in 3D space 							
	//																
	*======================================================================================*/

	void Camera::MoveCamera(float velocity)
	{
		Math::Vector3 vVector(0, 0, 0);

		vVector = (m_vView - m_vPosition).Normalize() * velocity;

		m_vPosition += vVector;

		m_vView += vVector;
	}

	/*======================================================================================
	//																
	//	Invoke's gluLookAt using a Camera							    
	//																	
	*======================================================================================*/

	void Camera::Look()
	{			
		gluLookAt(m_vPosition.x,    m_vPosition.y,   m_vPosition.z,
				  m_vView.x,		m_vView.y,		 m_vView.z,
				  m_vUpVector.x,	m_vUpVector.y,	 m_vUpVector.z);
		
		//THIS WORKS AS WELL, DONE MANUALLY
		/*
		float mat[16];

		memset(mat, 0, sizeof(float)*16);

		Math::Vector3 xAxis;
		Math::Vector3 yAxis;
		Math::Vector3 zAxis;

		zAxis = Normalize(m_vView - m_vPosition);
		xAxis = Normalize(Cross(zAxis, m_vUpVector));
		yAxis = Normalize(Cross(xAxis, zAxis));

		mat[0] = xAxis.x;
		mat[4] = xAxis.y;
		mat[8] = xAxis.z;

		mat[1] = yAxis.x;
		mat[5] = yAxis.y;
		mat[9] = yAxis.z;

		mat[2] = -zAxis.x;
		mat[6] = -zAxis.y;
		mat[10] = -zAxis.z;

		mat[15] = 1;

		glMultMatrixf(mat);

		memset(mat, 0, sizeof(float)*16);
		
		mat[0] = 1;
		mat[5] = 1;
		mat[10] = 1;
		mat[15] = 1;

		mat[12] = -m_vPosition.x;
		mat[13] = -m_vPosition.y;
		mat[14] = -m_vPosition.z;

		glMultMatrixf(mat);	
		*/
	}

	/*======================================================================================////
	//																	//
	//	Ease position towards a given point
	//																	//
	*======================================================================================*/////

	void Camera::EasePositionTowardsPoint(const Math::Vector3 &pt, float easing)
	{	
		this->m_vPosition += (pt - this->m_vPosition) * easing;
	}

	/*======================================================================================////
	//																	//
	//	Ease view towards a given point
	//																	//
	*======================================================================================*/////

	void Camera::EaseViewTowardsPoint(const Math::Vector3 &pt, float easing)
	{	
		this->m_vView += (pt - this->m_vView) * easing;
	}



	/*======================================================================================////
	//																	//
	//	Update the strafe vector direction			    //
	//																	//
	*======================================================================================*/////

	void Camera::UpdateStrafe()
	{	
		m_vStrafe = (m_vView - m_vPosition).Cross(m_vUpVector).Normalize();
	}

	void Camera::setCamera(Camera &Camera)
	{
		this->m_vPosition = Camera.Position();
		this->m_vView = Camera.View();
		this->m_vUpVector = Camera.UpVector();
	}

	/*
		Code used in lens flare, tanx to NEHE GREAT NEHE
		http://nehe.gamedev.net/
		-Lens flare tutorial
	*/

	bool Camera::isOccluded(const Math::Vector3 &p)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLdouble wx, wy, wz;
		GLfloat dbuf;

		gluProject(p.x, p.y, p.z, mvmat, projmat, viewport, &wx, &wy, &wz);

		glReadPixels((GLint)wx, (GLint)wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dbuf);

		if(dbuf < wz)
		{
			return true;
		}

		return false;
	}

	bool Camera::isOccludedInfinite(const Math::Vector3 &p)
	{
		GLint viewport[4];
		GLdouble mvmat[16];
		GLdouble projmat[16];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_PROJECTION_MATRIX, projmat);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

		GLdouble wx, wy, wz;
		GLfloat dbuf;

		gluProject(p.x, p.y, p.z, mvmat, projmat, viewport, &wx, &wy, &wz);

		wz = 0.999999;

		glReadPixels((GLint)wx, (GLint)wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dbuf);

		if(dbuf < wz)
		{
			return true;
		}

		return false;
	}

	void Camera::RenderLensFlareObject(GLfloat r, GLfloat g, GLfloat b, GLfloat a, Math::Vector3 &p, GLfloat scale, GLuint &texture)
	{
		Math::Vector3 q[4];
		GLfloat m[16];

		scale *= myLens.scale;

		glGetFloatv( GL_MODELVIEW_MATRIX, m );

		Math::Vector3 vRight = Math::Vector3( m[0], m[4], m[8] );
		Math::Vector3 vUp = Math::Vector3( m[1], m[5], m[9] );

		Math::Vector3 vPoint0 = p + ((-vRight - vUp) * scale);
		Math::Vector3 vPoint1 = p + (( vRight - vUp) * scale);
		Math::Vector3 vPoint2 = p + (( vRight + vUp) * scale);
		Math::Vector3 vPoint3 = p + ((-vRight + vUp) * scale);

		glBindTexture(GL_TEXTURE_2D, texture);					// Bind to the Big Glow texture
		glColor4f(r, g, b, a);		
				glBegin(GL_QUADS);	
					/*
						glTexCoord2f(0.0f, 1.0f);
						glVertex3f(-1, 1, 0);

						glTexCoord2f(0.0f, 0.0f);
						glVertex3f(-1, -1, 0);

						glTexCoord2f(1.0f, 0.0f);
						glVertex3f(1, -1, 0);

						glTexCoord2f(1.0f, 1.0f);
						glVertex3f(1, 1, 0);
					*/

					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(vPoint0.x,vPoint0.y,vPoint0.z);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(vPoint1.x, vPoint1.y,vPoint1.z);

					glTexCoord2f(1.0f, 1.0f);
					glVertex3f(vPoint2.x,vPoint2.y,vPoint2.z);

					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(vPoint3.x,vPoint3.y,vPoint3.z);
				glEnd();
		glPopMatrix();											
	}

	void Camera::RenderLensFlare(const Math::Vector3 &lightPos)
	{
		Math::Vector3 newLightPos = lightPos + this->myLens.pos;
		GLfloat Length = 0.0f;	

		glEnable(GL_BLEND);										
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);						
		glDisable(GL_DEPTH_TEST);								
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);	

		Math::Vector3 vLightSourceToCamera = m_vPosition - newLightPos;																	

		Length = 1/vLightSourceToCamera.InverseMagnitude();	

		Math::Vector3 lightDir = -vLightSourceToCamera/Length;

		if(Length > myLens.maxDistance && myLens.infinite)
		{
			newLightPos = m_vPosition + lightDir * myLens.maxDistance;
			Length = myLens.maxDistance;
		}

		Math::Vector3 directionVector = (m_vView - m_vPosition).Normalize();
		bool occluded = (myLens.infinite == true)?!this->isOccludedInfinite(newLightPos):!this->isOccluded(newLightPos);
		bool inFrustum = (myLens.infinite == true)?myGlobalGame->frustum.PointInFrustumInfinite(newLightPos.x, newLightPos.y, newLightPos.z):myGlobalGame->frustum.PointInFrustum(newLightPos.x, newLightPos.y, newLightPos.z);

		RenderLensFlareObject(0.60f, 0.60f, 0.8f, Math::abs(directionVector.Dot(lightDir))*myLens.sunLumino*1.25f, newLightPos, myLens.srcScale*myLens.scale, myLens.bigGlowTexture);	

		// Draw the flare only If the light source is in our line of sight
		if(inFrustum)
		{	
			/*
				GLuint bigGlowTexture;
				GLuint starTexture;
				GLuint glowTexture;
				GLuint haloTexture;
			*/

			if (occluded)	//Check if the center of the flare is occluded
			{				

				/*
				Math::Vector3 vLightSourceToCamera = m_vPosition - newLightPos;		// Lets compute the vector that points to the Camera from
																		// the light source.

				Length = Magnitude(vLightSourceToCamera);					// Save the length we will need it in a minute

				Math::Vector3 lightDir = -vLightSourceToCamera/Length;

				*/
				/* 
					Now here's something we need to do too, since our world is real huge,
					and light is infinitely away (really far away), we need to pretend it was
					bigger keeping it same size and really far away at the same time - hope
					you get what i mean
				*/


				/*
				if(Length > myLens.maxDistance && myLens.infinite)
				{
					newLightPos = m_vPosition + lightDir * myLens.maxDistance;
					Length = myLens.maxDistance;
				}


				Math::Vector3 directionVector = Normalize(m_vView - m_vPosition);
				*/

				Math::Vector3 ptIntersect = directionVector * Length;					// Now lets find an point along the Cameras direction
																			// vector that we can use as an intersection point. 
																			// Lets translate down this vector the same distance
																			// that the Camera is away from the light source.
				ptIntersect += m_vPosition;

				Math::Vector3 vLightSourceToIntersect = ptIntersect - newLightPos;	// Lets compute the vector that points to the Intersect
																			// point from the light source
				float invLength = vLightSourceToIntersect.InverseMagnitude();
				Length = 1/invLength;		// Save the length we will need it later.
				//vLightSourceToIntersect = Normalize(vLightSourceToIntersect);	// Normalize the vector so its unit length								
				vLightSourceToIntersect *= invLength;

				// Render the large hazy glow
				RenderLensFlareObject(0.60f, 0.60f, 0.8f, Math::abs(directionVector.Dot(lightDir))*myLens.touchedLumino, newLightPos, myLens.scale, myLens.bigGlowTexture);

				RenderLensFlareObject(0.60f, 0.60f, 0.8f, 1.0f, newLightPos, myLens.srcScale, myLens.bigGlowTexture);
				// Render the streaks
				RenderLensFlareObject(0.60f, 0.60f, 0.8f, 1.0f, newLightPos, myLens.srcScale, myLens.starTexture);
				// Render the small Glow
				RenderLensFlareObject(0.8f, 0.8f, 1.0f, 0.5f, newLightPos, 3.5f, myLens.glowTexture);

				Math::Vector3 pt = vLightSourceToIntersect * (Length * 0.2f);				// Lets compute a point that is 20%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.9f, 0.6f, 0.4f, 0.5f, pt, 0.6f, myLens.glowTexture);				// Render the small Glow

				pt = vLightSourceToIntersect * (Length * 0.15f);			// Lets compute a point that is 30%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.		
			
				RenderLensFlareObject(0.8f, 0.5f, 0.6f, 0.5f, pt, 1.9f, myLens.haloTexture);				// Render the a Halo
			
				pt = vLightSourceToIntersect * (Length * 0.175f);			// Lets compute a point that is 35%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.9f, 0.2f, 0.1f, 0.5f, pt, 1.03f, myLens.haloTexture);				// Render the a Halo

				pt = vLightSourceToIntersect * (Length * 0.285f);			// Lets compute a point that is 57%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.7f, 0.7f, 0.4f, 0.5f, pt, 1.8f, myLens.haloTexture);				// Render the a Halo
			
				pt = vLightSourceToIntersect * (Length * 0.2755f);			// Lets compute a point that is 55.1%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.9f, 0.9f, 0.2f, 0.5f, pt, 1.0f, myLens.glowTexture);				// Render the small Glow

				pt = vLightSourceToIntersect * (Length * 0.4775f);			// Lets compute a point that is 95.5%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.93f, 0.82f, 0.73f, 0.5f, pt, 1.2f, myLens.glowTexture);			// Render the small Glow
			
				pt = vLightSourceToIntersect * (Length * 0.49f);			// Lets compute a point that is 98%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.7f, 0.6f, 0.5f, 0.5f, pt, 1.6f, myLens.haloTexture);				// Render the a Halo

				pt = vLightSourceToIntersect * (Length * 0.65f);			// Lets compute a point that is 130%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.7f, 0.8f, 0.3f, 0.5f, pt, 3.0f, myLens.glowTexture);				// Render the small Glow
			
				pt = vLightSourceToIntersect * (Length * 0.63f);			// Lets compute a point that is 126%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.4f, 0.3f, 0.2f, 0.5f, pt, 3.6f, myLens.glowTexture);				// Render the small Glow

				pt = vLightSourceToIntersect * (Length * 0.8f);				// Lets compute a point that is 160%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.7f, 0.5f, 0.5f, 0.5f, pt, 3.6f, myLens.haloTexture);				// Render the a Halo
			
				pt = vLightSourceToIntersect * (Length * 0.7825f);			// Lets compute a point that is 156.5%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.8f, 0.5f, 0.1f, 0.5f, pt, 4.8f, myLens.glowTexture);				// Render the small Glow

				pt = vLightSourceToIntersect * (Length * 1.0f);				// Lets compute a point that is 200%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.5f, 0.5f, 0.7f, 0.5f, pt, 4.9f, myLens.haloTexture);				// Render the a Halo
			
				pt = vLightSourceToIntersect * (Length * 0.975f);			// Lets compute a point that is 195%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.4f, 0.1f, 0.9f, 0.5f, pt, 5.2f, myLens.glowTexture);				// Render the small Glow

				pt = vLightSourceToIntersect * (Length * 1.395f);				// Lets compute a point that is 200%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.5f, 0.5f, 0.7f, 0.5f, pt, 5.9f, myLens.haloTexture);

				pt = vLightSourceToIntersect * (Length * 1.295f);			// Lets compute a point that is 195%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.		

				RenderLensFlareObject(0.4f, 0.1f, 0.8f, 0.5f, pt, 7.7f, myLens.glowTexture);	
				
				pt = vLightSourceToIntersect * (Length * 1.695f);				// Lets compute a point that is 200%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.6f, 0.5f, 0.7f, 0.5f, pt, 7.9f, myLens.haloTexture);

				pt = vLightSourceToIntersect * (Length * 1.595f);			// Lets compute a point that is 195%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.		

				RenderLensFlareObject(0.5f, 0.1f, 0.8f, 0.5f, pt, 8.7f, myLens.glowTexture);

				pt = vLightSourceToIntersect * (Length * 1.995f);				// Lets compute a point that is 200%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.
			
				RenderLensFlareObject(0.8f, 0.5f, 0.7f, 0.5f, pt, 8.9f, myLens.haloTexture);

				pt = vLightSourceToIntersect * (Length * 1.795f);			// Lets compute a point that is 195%
				pt += newLightPos;										// away from the light source in the
																		// direction of the intersection point.		

				RenderLensFlareObject(0.7f, 0.1f, 0.8f, 0.5f, pt, 11.7f, myLens.glowTexture);
			}							
		}

		glDisable(GL_BLEND);									
		glEnable(GL_DEPTH_TEST);
	}

	void Camera::ShakeCamera(float time, float mag)
	{
		this->mfTime = time;
		this->mfMag = mag;
		this->mfTotalTime = this->mfTime;
	}

	void Camera::UpdateShakeCamera(float dt)
	{
		if(this->mfTime <= 0)
		{
			return;
		}

		this->mfTime -= dt;

		if(this->mfTime <= 0)
		{
			return;
		}

		this->m_vPosition += (RANDOM_VECTOR) * this->mfMag * ((mbIncrease == false) ? lerp(this->mfTime/this->mfTotalTime, 0.0f, 1.0f) : lerp(this->mfTime/this->mfTotalTime, 1.0f, 0.0f));
	}

	void Camera::SetCameraShakeIncreaseMode(void)
	{
		this->mbIncrease = true;
	}

	void Camera::SetCameraShakeDecreaseMode(void)
	{
		this->mbIncrease = false;
	}

	/*
	===============================
		Quartenion Camera class
	===============================
	*/

	quatCamera::quatCamera()
	{
		// Initalize all our member varibles.
		m_MaxPitchRate			= 0.0f;
		m_MaxHeadingRate		= 0.0f;
		m_HeadingDegrees		= 0.0f;
		m_PitchDegrees			= 0.0f;
		m_MaxForwardVelocity	= 0.0f;
		m_ForwardVelocity		= 0.0f;
	}

	quatCamera::~quatCamera()
	{

	}

	void quatCamera::SetPrespective()
	{
		GLfloat Matrix[16];
		Math::Quaternion q;

		// Make the quats that will represent our rotations
		m_qPitch.CreateFromAxisAngle(1.0f, 0.0f, 0.0f, m_PitchDegrees);
		m_qHeading.CreateFromAxisAngle(0.0f, 1.0f, 0.0f, m_HeadingDegrees);
		
		// Combine the pitch and heading rotations and store the results in q
		q = m_qPitch * m_qHeading;
		q.returnMatrix(Matrix);

		// Let OpenGL set our new prespective on the world!
		glMultMatrixf(Matrix);
		
		// Create a matrix from the pitch Math::Quaternion and get the j vector 
		// for our direction.
		m_qPitch.returnMatrix(Matrix);
		m_DirectionVector.y = Matrix[9];

		// Combine the heading and pitch rotations and make a matrix to get
		// the i and j vectors for our direction.
		q = m_qHeading * m_qPitch;
		q.returnMatrix(Matrix);
		m_DirectionVector.x = Matrix[8];
		m_DirectionVector.z = Matrix[10];

		// Scale the direction by our speed.
		m_DirectionVector *= m_ForwardVelocity;

		// Increment our position by the vector
		m_Position.x += m_DirectionVector.x;
		m_Position.y += m_DirectionVector.y;
		m_Position.z += m_DirectionVector.z;

		// Translate to our new position.
		glTranslatef(-m_Position.x, -m_Position.y, m_Position.z);
	}

	void quatCamera::ChangePitch(float deg)
	{
		if(Math::abs(deg) < Math::abs(m_MaxPitchRate))
		{
			// Our pitch is less than the max pitch rate that we 
			// defined so lets increment it.
			m_PitchDegrees += deg;
		}
		else
		{
			// Our pitch is greater than the max pitch rate that
			// we defined so we can only increment our pitch by the 
			// maximum allowed value.
			if(deg < 0)
			{
				// We are pitching down so decrement
				m_PitchDegrees -= m_MaxPitchRate;
			}
			else
			{
				// We are pitching up so increment
				m_PitchDegrees += m_MaxPitchRate;
			}
		}

		if(m_PitchDegrees > 360.0f)
		{
			m_PitchDegrees -= 360.0f;
		}
		else if(m_PitchDegrees < -360.0f)
		{
			m_PitchDegrees += 360.0f;
		}
	}

	void quatCamera::ChangeHeading(float deg)
	{
		if(Math::abs(deg) < Math::abs(m_MaxHeadingRate))
		{
			// Our Heading is less than the max heading rate that we 
			// defined so lets increment it but first we must check
			// to see if we are inverted so that our heading will not
			// become inverted.
			if(m_PitchDegrees > 90 && m_PitchDegrees < 270 || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
			{
				m_HeadingDegrees -= deg;
			}
			else
			{
				m_HeadingDegrees += deg;
			}
		}
		else
		{
			// Our heading is greater than the max heading rate that
			// we defined so we can only increment our heading by the 
			// maximum allowed value.
			if(deg < 0)
			{
				// Check to see if we are upside down.
				if((m_PitchDegrees > 90 && m_PitchDegrees < 270) || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
				{
					// Ok we would normally decrement here but since we are upside
					// down then we need to increment our heading
					m_HeadingDegrees += m_MaxHeadingRate;
				}
				else
				{
					// We are not upside down so decrement as usual
					m_HeadingDegrees -= m_MaxHeadingRate;
				}
			}
			else
			{
				// Check to see if we are upside down.
				if(m_PitchDegrees > 90 && m_PitchDegrees < 270 || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
				{
					// Ok we would normally increment here but since we are upside
					// down then we need to decrement our heading.
					m_HeadingDegrees -= m_MaxHeadingRate;
				}
				else
				{
					// We are not upside down so increment as usual.
					m_HeadingDegrees += m_MaxHeadingRate;
				}
			}
		}
		
		// We don't want our heading to run away from us either. Although it
		// really doesn't matter I prefer to have my heading degrees
		// within the range of -360.0f to 360.0f
		if(m_HeadingDegrees > 360.0f)
		{
			m_HeadingDegrees -= 360.0f;
		}
		else if(m_HeadingDegrees < -360.0f)
		{
			m_HeadingDegrees += 360.0f;
		}
	}

	void quatCamera::ChangeVelocity(float vel)
	{
		if(Math::abs(vel) < Math::abs(m_MaxForwardVelocity))
		{
			// Our velocity is less than the max velocity increment that we 
			// defined so lets increment it.
			m_ForwardVelocity += vel;
		}
		else
		{
			// Our velocity is greater than the max velocity increment that
			// we defined so we can only increment our velocity by the 
			// maximum allowed value.
			if(vel < 0)
			{
				// We are slowing down so decrement
				m_ForwardVelocity -= -m_MaxForwardVelocity;
			}
			else
			{
				// We are speeding up so increment
				m_ForwardVelocity += m_MaxForwardVelocity;
			}
		}
	}

	void quatCamera::SetViewByMouse(void)
	{
		float DeltaMouse;
		POINT pt;

		int CenterX = CameraGameGL->getApp()->windowPosition.x>10000?1:CameraGameGL->getApp()->windowPosition.x + (CameraGameGL->getApp()->getDimensions().x>>1);
		int CenterY = CameraGameGL->getApp()->windowPosition.y>10000?1:CameraGameGL->getApp()->windowPosition.y + (CameraGameGL->getApp()->getDimensions().y>>1);
		CenterX = CenterX>(int)CameraGameGL->getApp()->getSystemResolution().x?(int)CameraGameGL->getApp()->getSystemResolution().x-5:CenterX;
		CenterY = CenterY>(int)CameraGameGL->getApp()->getSystemResolution().y?(int)CameraGameGL->getApp()->getSystemResolution().y-5:CenterY;

		GetCursorPos(&pt);
		
		int MouseX = pt.x;
		int MouseY = pt.y;

		if(MouseX == CenterX || MouseY == CenterY)
		{
			return;
		}

		DeltaMouse = float(CenterX - MouseX) * CameraGameGL->getApp()->GetMouseSensitivity();
		this->ChangeHeading(DeltaMouse);

		DeltaMouse = float(CenterY - MouseY) * CameraGameGL->getApp()->GetMouseSensitivity();
		this->ChangePitch(DeltaMouse);

		SetCursorPos(CenterX, CenterY);
	}

	void quatCamera::setGame(CoreEngine &game)
	{
		this->CameraGameGL = &game;
	}

	void quatCamera::Update(void)
	{

	}

}