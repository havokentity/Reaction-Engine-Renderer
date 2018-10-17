/*
========================================================
	class Console
========================================================
*/

#ifndef CONSOLE_H
#define CONSOLE_H

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)

#include "Client.h"
#include "utils/BaseConsole.h"

namespace DifferentialArts
{
	/*
	==============================================
		Console Class 
	==============================================
	*/

	/*!	\class Console
		\brief Console class
		\author Rajesh Peter Douglas D'Monte	

		This console class is used inside the main engine class. Use this help
		as a reference on how to use the console methods.
	*/
	class RX_API Console : public BaseConsole
	{
	public:
		Console();//!< Constructor
		~Console();//!< Destructor

		virtual void draw(void); //!< Renders the console on screen
		virtual void operate(float dt); //!< This should be called in the Update method of the engine.
		virtual void init(void);//!< Initializes the console

	protected:
		unsigned int mTexture;		
	};
}

#endif