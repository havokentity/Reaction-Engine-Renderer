/*
  NOTE;
  REFER MOUSE.H
*/
#include "Mouse.h"

namespace DifferentialArts
{
	MouseButton::~MouseButton()
	{
	}

	MouseButton::MouseButton()
	{
	}

	bool MouseButton::isDown()
	{
		return this->state;
	}

	bool MouseButton::isReleased()
	{
		return this->release;
	}

	void MouseButton::SetState(bool s)
	{
		this->state = s;
	}

	void MouseButton::SetRelease(bool s)
	{
		this->release = s;
	}

	Mouse::~Mouse()
	{
	}

	Mouse::Mouse()
	{
		this->LeftButton.state = false;
		this->RightButton.state = false;
		this->MiddleButton.state = false;
	}

	void Mouse::Hide(void)
	{
		int level = ShowCursor(FALSE);
		if(level > -1)
			this->Hide();
	}

	void Mouse::Show(void)
	{
		int level = ShowCursor(TRUE);
		if(level < 0)
			this->Show();
	}

	void Mouse::SetPosition(int xpos, int ypos)
	{
		SetCursorPos(xpos, ypos);
	}


	/* This function will center the mouse on screen */
	void Mouse::CenterScreen(MousePointInt windowPosition, DIM dimensions, DIM sysResolution)
	{	
		unsigned int middleX, middleY;
		middleX = windowPosition.x>10000?1:windowPosition.x + (int(dimensions.x)>>1);
		middleY = windowPosition.y>10000?1:windowPosition.y + (int(dimensions.y)>>1);
		middleX = (middleX>sysResolution.x)?sysResolution.x-5:middleX;
		middleY = (middleY>sysResolution.y)?sysResolution.y-5:middleY;

		/*	
			char str[128];
			sprintf(str, "x: %d   y: %d", int(dimensions.x)>>1, int(dimensions.y)>>1);
			MessageBox(NULL, str, "", MB_OK);
		*/

		SetCursorPos(middleX, middleY);
	}

	DIM Mouse::getCenter(MousePointInt windowPosition, DIM dimensions, DIM sysResolution)
	{
		DIM middle;
		unsigned int middleX, middleY;
		middleX = windowPosition.x>10000?1:windowPosition.x + (int(dimensions.x)>>1);
		middleY = windowPosition.y>10000?1:windowPosition.y + (int(dimensions.y)>>1);
		middleX = (middleX>sysResolution.x)?sysResolution.x-5:middleX;
		middleY = (middleY>sysResolution.y)?sysResolution.y-5:middleY;

		middle.x = middleX;
		middle.y = middleY;

		return middle;
	}

	MousePointInt Mouse::GetImmediateCursorPosition(void)
	{
		MousePointInt ret;
		POINT tempRet;
		GetCursorPos(&tempRet);

		ret.x = tempRet.x;
		ret.y = tempRet.y;

		return ret;
	}
}
