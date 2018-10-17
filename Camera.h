/*
	Description: Consists of Camera class, and Math::Quaternion Camera class

	The classes contained here are,
	class Camera
	class quatCamera
	
*/
#ifndef _Camera_H__T_H_
#define _Camera_H__T_H_

#include <windows.h>
#include <stdio.h>
#include "Client.h"
#include "utils/3DMATH.H"
#include "CoreEngine.h"

namespace DifferentialArts
{
	/*!	\class Camera
		\brief Camera class
		\author Rajesh Peter Douglas D'Monte

		This class is a generic Camera class for using in a 3D
		world.
	*/
	class RX_API Camera
	{
	public:
		LensFlareTexObj myLens; //!< Lens flare object

		//Constructors
		Camera(void); //!< Constructor
		Camera(Camera &Camera); //!< Copy constructor

		Math::Vector3 Position() {	return m_vPosition;		}
		//!< Returns the Camera position
		Math::Vector3 View()		{	return m_vView;			}
		//!< Returns the Camera view
		Math::Vector3 UpVector() {	return m_vUpVector;		}
		//!< Returns the Camera up vector
		Math::Vector3 Strafe()	{	return m_vStrafe;		}
		//!< Returns the Camera strafe vector

		void SetPosition(Math::Vector3 &v) {		this->m_vPosition	= v;	} //!< Sets the position
		void SetView(Math::Vector3 &v)	  {		this->m_vView		= v;	} //!< Sets the view point
		void SetUpVector(Math::Vector3 &v) {		this->m_vUpVector	= v;	} //!< Sets the up vector

		Math::Vector3* pPosition()   {	return &m_vPosition;	}
		//!< Returns a pointer to the Camera position
		Math::Vector3* pView()		{	return &m_vView;		}
		//!< Returns a pointer to the Camera view
		Math::Vector3* pUpVector()   {	return &m_vUpVector;	}
		//!< Returns a pointer to the Camera up vector
		Math::Vector3* pStrafe()	    {	return &m_vStrafe;		}
		//!< Returns a pointer to the Camera strafe vector
		
		//positions my Camera
		void PositionCamera(const Math::Vector3 &pos, const Math::Vector3 &view,
							const Math::Vector3 &up); //!< Position the Camera

		void EasePositionTowardsPoint(const Math::Vector3 &pt, float easing); //easing is a 0 to 1 floating point value
		//!<Math::Vector3 &pt is the destination point. It eases the position vector towards that point.

		void EaseViewTowardsPoint(const Math::Vector3 &pt, float easing); //easing is a 0 to 1 floating point value
		//!<Math::Vector3 &pt is the destination point. It eases the view vector towards that point.

		//rotates my Camera
		void RotateCamera(float angle, float x, float y, float z);
		//!< Rotates the Camera

		//rotates the Camera around a point
		void RotateAroundAPoint(const Math::Vector3 &vCenter, float angle, float x, float y, float z);
		//!< Rotates the Camera around a point. 

		//moves the Camera
		void MoveCamera(float velocity);
		//!< Moves the Camera in the direction of the view vector.

		//strafes the Camera
		void StrafeCamera(float velocity);
		//!< Moves the Camera in the direction of the strafe vector.
		
		//set's the view of a Camera by mouse
		void SetViewByMouse();
		//!< Updates Camera view with respect to mouse movement change.

		//invokes the gluLookAt and attributes it to a Camera
		void Look();	
		//!< Builds the appropriate Look matrix for the model view transform.

		//WASD/arrow keys controls
		void GetKeyInput();	
		//!< Moves the Camera with WASD keys or arrow keys

		void UpdateStrafe();
		//!< Updates the strafe vector

		void setCamera(Camera &Camera); //Set the Camera to another Camera
		//!< Sets the Camera properites to another Camera's properties

		void SetSpeed(float speed);	//!< Sets the Camera movement speed.

		bool isOccluded(const Math::Vector3 &p); //!< Checks whether a point in 3D is occluded, be it scene geometry or not in frustum

		bool isOccludedInfinite(const Math::Vector3 &p); //!< Checks whether an infinitely far away point is occluded or not, good for sun light sources

		void RenderLensFlare(const Math::Vector3 &lightPos); //!< This function renders the lens flare in an ortho projection

		void RenderLensFlareObject(GLfloat r, GLfloat g,
								   GLfloat b, GLfloat a,
								   Math::Vector3 &p, GLfloat scale,
								   GLuint &texture);
		//!<This function renders a single lens flare object that makes up a complete lens flare

		void ShakeCamera(float time, float mag);
		//!< Turns on shake Camera mode
		/*!< 
			This method shakes the Camera for a period of time and by a given magnitude

			\param time The total time the Camera shakes
			\param mag The magnitude of the radius of the shake circle around the position of the Camera

			Don't forget to call UpdateShakeCamera, to actually apply the shaking process. 
		*/

		void UpdateShakeCamera(float dt);
		//!< Updates the Camera shake properties
		/*!< 
			This method updates the shake Camera by interval dt

			\param dt A small interval dt
		*/

		void SetCameraShakeIncreaseMode(void);
		//!< This method makes the shake Camera option increase shake with time

		void SetCameraShakeDecreaseMode(void);
		//!< This method makes the shake Camera option decrease shake with time

		static int mTotalCameras;

		void BindCameraVariables(void);

	private:
		bool bInit;
		float mfTime;
		bool mbIncrease;
		float mfTotalTime;
		float mfMag;
		Math::Vector3 m_vPosition;
		Math::Vector3 m_vView;
		Math::Vector3 m_vUpVector;
		Math::Vector3 m_vStrafe;
		float m_fSpeed; // Free Camera speed
	};

	/*
	===============================
		Quartenion Camera class
	===============================
	*/

	/*!	\class quatCamera
		\brief Quaternion Camera class
		\author Rajesh Peter Douglas D'Monte

		This class is a generic Camera class for using in a 3D
		world which uses quaternions.

		\deprecated
		@see Camera
	*/
	class quatCamera  
	{
	public:
		float m_MaxPitchRate;
		float m_MaxHeadingRate;
		float m_HeadingDegrees;
		float m_PitchDegrees;
		float m_MaxForwardVelocity;
		float m_ForwardVelocity;
		Math::Quaternion m_qHeading;
		Math::Quaternion m_qPitch;
		Math::Vector3 m_Position;
		Math::Vector3 m_DirectionVector;

		void ChangeVelocity(float vel);
		void ChangeHeading(float deg);
		void ChangePitch(float deg);
		void SetPrespective(void);
		void SetViewByMouse(void);
		quatCamera();
		virtual ~quatCamera();

		//set CameraGameGL
		void setGame(CoreEngine &game);

		void Update(void);

	private:
		CoreEngine* CameraGameGL;
	};
}

#endif
