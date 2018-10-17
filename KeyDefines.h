/*
	Key code defines
*/
#ifndef _KEY_DEFINES_H_
#define _KEY_DEFINES_H_

#define RX_USING_WIN_32_VK //Using win32 Virtual keys define

		#ifdef RX_USING_WIN_32_VK

		#include <windows.h>

		#define RX_TILDE VK_OEM_3

		#ifndef VK_CAPS
		#define VK_CAPS 20
		#endif

		#define RX_CAPS VK_CAPS

		#define RX_MINUS VK_OEM_MINUS 
		#define RX_PLUS VK_OEM_PLUS
		#define RX_COMMA VK_OEM_COMMA 
		#define RX_PERIOD VK_OEM_PERIOD
		#define RX_COLON VK_OEM_1 
		#define RX_QUESTION VK_OEM_2

		#define RX_BACKSLASH VK_OEM_102

		/* RIPPED FROM WINUSER.H */

		/*
		 * Virtual Keys, Standard Set
		 */
		#define RX_LBUTTON        0x01
		#define RX_RBUTTON        0x02
		#define RX_CANCEL         0x03
		#define RX_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

		#define RX_OPEN_BRACE    0xDB  //  '[{' for US
		#define RX_PIPE          0xDC  //  '\|' for US
		#define RX_CLOSE_BRACE   0xDD  //  ']}' for US
		#define RX_QUOTE         0xDE  //  ''"' for US

		/*
		 * 0x07 : unassigned
		 */

		#define RX_BACK           0x08
		#define RX_BACKSPACE	  RX_BACK
		#define RX_TAB			  0x09

		/*
		 * 0x0A - 0x0B : reserved
		 */

		#define RX_CLEAR          0x0C
		#define RX_RETURN         0x0D

		#define RX_SHIFT          0x10
		#define RX_CONTROL        0x11
		#define RX_MENU           0x12
		#define RX_PAUSE          0x13
		#define RX_CAPITAL        0x14

		#define RX_KANA           0x15
		#define RX_HANGEUL        0x15  /* old name - should be here for compatibility */
		#define RX_HANGUL         0x15
		#define RX_JUNJA          0x17
		#define RX_FINAL          0x18
		#define RX_HANJA          0x19
		#define RX_KANJI          0x19

		#define RX_ESCAPE         0x1B

		#define RX_CONVERT        0x1C
		#define RX_NONCONVERT     0x1D
		#define RX_ACCEPT         0x1E
		#define RX_MODECHANGE     0x1F

		#define RX_SPACE          0x20
		#define RX_PRIOR          0x21
		#define RX_NEXT           0x22
		#define RX_END            0x23
		#define RX_HOME           0x24
		#define RX_LEFT           0x25
		#define RX_UP             0x26
		#define RX_RIGHT          0x27
		#define RX_DOWN           0x28
		#define RX_SELECT         0x29
		#define RX_PRINT          0x2A
		#define RX_EXECUTE        0x2B
		#define RX_SNAPSHOT       0x2C
		#define RX_INSERT         0x2D
		#define RX_DELETE         0x2E
		#define RX_HELP           0x2F

		/*
		 * RX_0 - RX_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
		 * 0x40 : unassigned
		 * RX_A - RX_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		 */

		#define RX_LWIN           0x5B
		#define RX_RWIN           0x5C
		#define RX_APPS           0x5D

		/*
		 * 0x5E : reserved
		 */

		#define RX_SLEEP          0x5F

		#define RX_NUMPAD0        0x60
		#define RX_NUMPAD1        0x61
		#define RX_NUMPAD2        0x62
		#define RX_NUMPAD3        0x63
		#define RX_NUMPAD4        0x64
		#define RX_NUMPAD5        0x65
		#define RX_NUMPAD6        0x66
		#define RX_NUMPAD7        0x67
		#define RX_NUMPAD8        0x68
		#define RX_NUMPAD9        0x69
		#define RX_MULTIPLY       0x6A
		#define RX_ADD            0x6B
		#define RX_SEPARATOR      0x6C
		#define RX_SUBTRACT       0x6D
		#define RX_DECIMAL        0x6E
		#define RX_DIVIDE         0x6F
		#define RX_F1             0x70
		#define RX_F2             0x71
		#define RX_F3             0x72
		#define RX_F4             0x73
		#define RX_F5             0x74
		#define RX_F6             0x75
		#define RX_F7             0x76
		#define RX_F8             0x77
		#define RX_F9             0x78
		#define RX_F10            0x79
		#define RX_F11            0x7A
		#define RX_F12            0x7B
		#define RX_F13            0x7C
		#define RX_F14            0x7D
		#define RX_F15            0x7E
		#define RX_F16            0x7F
		#define RX_F17            0x80
		#define RX_F18            0x81
		#define RX_F19            0x82
		#define RX_F20            0x83
		#define RX_F21            0x84
		#define RX_F22            0x85
		#define RX_F23            0x86
		#define RX_F24            0x87

/*
		 * 0x88 - 0x8F : unassigned
		 */

		#define RX_NUMLOCK        0x90
		#define RX_SCROLL         0x91

		/*
		 * NEC PC-9800 kbd definitions
		 */
		#define RX_OEM_NEC_EQUAL  0x92   // '=' key on numpad

		#define RX_LSHIFT         0xA0
		#define RX_RSHIFT         0xA1
		#define RX_LCONTROL       0xA2
		#define RX_RCONTROL       0xA3
		#define RX_LMENU          0xA4
		#define RX_RMENU          0xA5		

		#endif

#endif