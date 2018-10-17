/*
	Date: Long ago

	  NOTE:

	  WIN32 SPECIFIC MOUSE HANDLER CLASS

	  class MouseButton, Mouse

	  Mouse uses WINAPI functions
*/

#ifndef MOUSE_ENG_H
#define MOUSE_ENG_H

#include <Windows.h>
#include <stdio.h>
#include "Client.h"

namespace DifferentialArts
{
	#define MOUSE_UP   0
	#define MOUSE_DOWN 1

	//Structures
	/*!	\struct MousePoint
		\brief Handles floating point mouse positions
		\author Rajesh Peter Douglas D'Monte	
	*/
	struct MousePoint
	{
		float x;//!< x is the x coord on screen
		float y;//!< y is the y coord on screen	
	};

	/*!	\struct MousePointInt
		\brief Handles integer point mouse positions
		\author Rajesh Peter Douglas D'Monte	
	*/
	struct MousePointInt
	{
		int x;//!< x is the x coord on screen
		int y;//!< y is the y coord on screen
	};

	/*!	\struct DIMENSIONS
		\brief Handles unsigned integer point coords
		\author Rajesh Peter Douglas D'Monte	
		
		You can also use it as DIM

		Code:\n
		\code
		typedef struct tDIMENSIONS
		{
			unsigned int x;
			unsigned int y;
		} DIMENSIONS, DIM;
		\endcode
	*/
	typedef struct tDIMENSIONS
	{
		unsigned int x;//!< x is the x coord
		unsigned int y;//!< y is the y coord
	} DIMENSIONS, DIM;

	/*!	\class MouseButton
		\brief Handles button presses
		\author Rajesh Peter Douglas D'Monte

		Doesn't work independantly. This is a helper class for engine internals
	*/
	class RX_API MouseButton
	{
	public:
		MouseButton();//!<Constructor
		~MouseButton();//!<Destructor

		bool isDown();//!< Check if the Button is down
		bool isReleased();//!< Check if the Button is released
		void SetState(bool s);//!< Sets the button down state
		void SetRelease(bool s);//!< Sets the button release state

	private:	

		bool state;
		bool release;

		friend class Mouse;
	};

	/*!	\class Mouse
		\brief Handles Mouse functions
		\author Rajesh Peter Douglas D'Monte

		Doesn't work independantly. This is a helper class for engine internals
	*/
	class RX_API Mouse
	{
	public:
		Mouse();//!<Constructor
		~Mouse();//!<Destructor

		//Windows specific code
		void Hide(void);//!< Hides the mouse cursor
		void Show(void);//!< Shows the mouse cursor
		void SetPosition(int xpos, int ypos);//!< Sets mouse position
		void CenterScreen(MousePointInt windowPosition, DIM dimensions, DIM sysResolution);//!< Centers the mouse
		//End of windows specific code
		DIM getCenter(MousePointInt windowPosition, DIM dimensions, DIM sysResolution);//!< Returns the center of the screen
		
		MousePointInt GetImmediateCursorPosition(void);//!< Returns the mouse position

		MouseButton LeftButton;//!< Left mouse button
		MouseButton RightButton;//!< Right mouse button
		MouseButton MiddleButton;//!< Middle mouse button

		int x;//!< x is the x coord on screen
		int y;//!< y is the y coord on screen
		short mouseWheel;//!<Mouse wheel change
	private:		
	};
}

#endif