/*
	Description: Main AbstractLight Base Object

	classes contained are:

	class AbstractLight

	NOTE: FOR USE WITH SHADERS ONLY
*/

#ifndef RX_LIGHT_H
#define RX_LIGHT_H

#pragma comment (lib, "opengl32.lib")

#include "Client.h"
#include <windows.h>
#include <gl/gl.h>
#include "GL_MultiTextureExterns.h"
#include "utils/3DMATH.H"

namespace DifferentialArts
{
	/*!	\class AbstractLight
		\brief An abstract AbstractLight class
		\author Rajesh Peter Douglas D'Monte	

		This class is a base class for all future light classes
	*/
	class RX_API AbstractLight
	{
	public:
		AbstractLight();	//!<Constructor		
		AbstractLight(int index);	//!<Constructor, with the light index passed as parameter
		~AbstractLight();//!< Destructor

		Math::Vector3 pos;//!< Position vector
		Math::Vector3 direction;//!< Direction vector
		float specularExponent;//!< Specular power

		Math::Vector3 diffuse; //!< Diffuse Color
		Math::Vector3 specular;//!< Specular Color
		Math::Vector3 ambient;//!< Ambient Color

		bool bEnabled;//!< AbstractLight enabled boolean
		bool bDeferred; //!< Sets if the lights are deferred

		void setAttenuation(Math::Vector3 &x); //!< Sets attenuation
		void setSpot(bool &x);	 //!< Sets if the light is a spot light or not

		Math::Vector3 getAttenuation(void); //!< Returns attenuation
		bool getSpot(void);	 //!< Returns true if the light is a spot light

		virtual void UpdateAPILight(bool passObjSpcViaMultiTexcoord) = 0;//!< Updates all light parameters through the Graphics API, the parameter its deprecated

		virtual void UpdateAPILight_pos_only(bool passObjSpcViaMultiTexcoord) = 0;//!< Updates all light position through the Graphics API. Ignore the parameter its deprecated
		virtual void UpdateAPILight_direction_only() = 0;//!< Updates all light direction through the Graphics API.

		virtual void UpdateAPILight_diffuse_only() = 0;//!< Updates all light diffuse through the Graphics API.
		virtual void UpdateAPILight_specular_only() = 0;//!< Updates all light specular through the Graphics API.
		virtual void UpdateAPILight_ambient_only() = 0;//!< Updates all light ambient through the Graphics API.

		virtual void UpdateAPILight_Enabled() = 0;//!< Updates all light enabled status through the Graphics API.

		virtual void UpdateAPILight_attenuation_only() = 0;//!< Updates all light attenuation through the Graphics API.

		void Assign(const AbstractLight &m);	//!<Method like copy constructor

		Math::Vector3 attenuation;
		bool spot;
		int mIndex;	

		float magSquared; //!< You can store some user data for sorting purpose here
	};

	
	/*!	\class EngineLight
		\brief An OpenGL light class
		\author Rajesh Peter Douglas D'Monte	

		The OpenGL light class. Manages the bare minimum for lighting.
	*/
	class RX_API EngineLight: public AbstractLight
	{
	public:
		EngineLight();	//!<Constructor
		EngineLight(int index);	//!<Constructor, with the light index passed as parameter

		~EngineLight();//!< Destructor

		virtual void UpdateAPILight(bool passObjSpcViaMultiTexcoord = false);

		virtual void UpdateAPILight_pos_only(bool passObjSpcViaMultiTexcoord = false);
		virtual void UpdateAPILight_direction_only();

		virtual void UpdateAPILight_diffuse_only();
		virtual void UpdateAPILight_specular_only();
		virtual void UpdateAPILight_ambient_only();

		virtual void UpdateAPILight_Enabled();

		virtual void UpdateAPILight_attenuation_only();
	};
}

#endif