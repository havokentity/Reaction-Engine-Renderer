/*
	Description: Consists of FONT functions, FontObject

	The classes contained here are,
	class FontObject
	
*/
#ifndef FONT_H_
#define FONT_H_

/*! \file Font.h
    \brief A basic font library
    
	This header has many font functions that the end user shouldn't touch.
	Except for the one's that are documented. The rest of the functions are for
	internal engine use alone.
*/

#include <fstream>

#include "CoreEngine.h"
#include "Texture.h"
#include "Client.h"

namespace DifferentialArts
{
	extern GLuint fontBase0;

	const int iMaxChars = 256;

	typedef struct tFontColor
	{
		float r, g, b;
	} FontColor;

	typedef struct tFontColor4
	{
		float r, g, b, a;
	} FontColor4;

	RX_API void CreateTextureFonts(char* fileName);
	RX_API void Print(int x, int y, int set, const char *fmt, ...); //Print GL textured font text
	/*! \fn void Print(int x, int y, int set, const char *fmt, ...)
		\brief Prints text to screen

		\param x The x coord
		\param y The y coord
		\param set Font set, Can be 0 or 1
		\param fmt Your formatted text you wish to print
	*/
	RX_API void sPrint(int x, int y, int set, const char *fmt, ...);
	/*! \fn void Print(int x, int y, int set, const char *fmt, ...)
		\brief Prints text to screen

		This function is the same as Print() except that it doesn't render in an 
		ORTHO screen. You have to do that manually.

		\param x The x coord
		\param y The y coord
		\param set Font set, Can be 0 or 1
		\param fmt Your formatted text you wish to print
	*/

	/*sglprint: Same as Print, except, convert	to ortho routine is not called*/

	RX_API void jPrint(int x, int y, int font, FontColor color, const char* fmt, ...);/*Same as Print, except, convert
																to font colour can be sent */
	RX_API void FreeTextureFont(void);
	RX_API FontColor Color(float r, float g, float b);
	RX_API FontColor4 Color4(float r, float g, float b, float a);

	/* Font defines */
	#define FONT_CENTER  9999
	#define FONT_MIN    10000
	#define FONT_MAX    10001

	/* OOP Bitmap Font object */

	/*!	\class FontObject
		\brief Font class
		\author Rajesh Peter Douglas D'Monte

		A class for managing fonts. This class doesn't use textures. It
		uses real font metrics and real fonts.
	*/
	class RX_API FontObject
	{
	public:
		FontObject(); //!< Constructor
		~FontObject(); //!< Destructor

		bool Create(char* fontName, int height, /* 0 - 9*/int fontWeight); //!< Creates a font
		void Free(void); //!< Releases resources used by the FontObject
		void Render(int x, int y, FontColor4 color, const char* fmt, ...); //!< Renders the font
		void stripRender(int x, int y, FontColor4 color, const char* fmt, ...); //!< Strip renders the font without ORTHO conversion
		/* This is for rendering loading text (render loop independant) */ 
		void lRender(int x, int y, FontColor4 color, const char* fmt, ...); //!< Render loop independant font rendering
		/* This is for rendering loading text (render loop independant) and also clears
		   the back buffer first
		*/
		void lcRender(int x, int y, FontColor4 color, const char* fmt, ...); //!< Render loop independant font rendering and clears the screen
		int getHeight(void); //!< Returns the font height
		void lclear(void); //!< Clears the color buffer

	private:
		GLuint fontBase;
		int h, h_into_2, actualHeight;
		char fontName[128];
	};
}

#endif