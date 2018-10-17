/*
	Description: Main Engine/Wrapper class

	classes contained are:

	class LightSystem
*/


#ifndef RX_LIGHT_HANDLER_H_
#define RX_LIGHT_HANDLER_H_

#include "Light.h"
#include "Client.h"

namespace DifferentialArts
{
	#define RX_CUSTOM_MAX_LIGHTS 8
	/*! \def RX_CUSTOM_MAX_LIGHTS
		\brief Max engine lights. Note there can be any number of virtual lights
	*/


	/*!	\class LightSystem
		\brief The reaction engine light handler class
		\author Rajesh Peter Douglas D'Monte	

		Don't use this class unless you have a good reason. Its specifically
		for Reaction Engine internals, such as the scene manager etc.
	*/
	class RX_API LightSystem
	{
	public:
		LightSystem();//!< Constructor
		~LightSystem();//!< Deconstructor

		bool Initialize(void); //!< Initializes engine lights.

		EngineLight light[RX_CUSTOM_MAX_LIGHTS]; //!< AbstractLight objects
	};
}

#endif