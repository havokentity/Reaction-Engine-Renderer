/*
	Description: Consists of FONT functions, FontObject

	The classes contained here are,
	class FontObject
	
	Further: refer, Font.h
*/

#include "Font.h"

namespace DifferentialArts
{
	GLuint fontBase0;

	RX_API void CreateTextureFonts(char* fileName)								
	{
		float	cx;											
		float	cy;			

		GLuint width, height;

		LoadTexture(fileName, g_Textures, FONT_TEXTURE, true, width, height, true, true);
		fontBase0=glGenLists(256);								
		glBindTexture(GL_TEXTURE_2D, g_Textures[FONT_TEXTURE]);			
		for (int loop=0; loop<256; loop++)						
		{
			cx=float(loop%16)/16.0f;						
			cy=float(loop/16)/16.0f;						

			glNewList(fontBase0+loop,GL_COMPILE);				
				glBegin(GL_QUADS);							
					glTexCoord2f(cx,1.0f-cy-0.0625f);		
					glVertex2i(0,0);						
					glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);
					glVertex2i(16,0);						
					glTexCoord2f(cx+0.0625f,1.0f-cy);		
					glVertex2i(16,16);						
					glTexCoord2f(cx,1.0f-cy);				
					glVertex2i(0,16);						
				glEnd();									
				glTranslatef(11,0,0);	
			glEndList();						
		}
		
		myGlobalGame->mlog.LogSuccessText(LString("Texture Font Engine succesfully Loaded!"));
	}

	RX_API void jPrint(int x, int y, int font, FontColor color, const char* fmt, ...)
	{
		va_list ap;
		char text[255];

		if(font>1)
			font = 1;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);	
		
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glColor3f(color.r, color.g, color.b);
		glBindTexture(GL_TEXTURE_2D,g_Textures[FONT_TEXTURE]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho(0, 1024, 768, 0, 0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glPushAttrib(GL_LIST_BIT);
		glListBase(fontBase0-32+(128*font));
		glTranslated(x, y, 0);
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	
		glPopAttrib();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPopAttrib();
	}

	RX_API void FreeTextureFont(void)
	{	
		glDeleteTextures(1, &g_Textures[FONT_TEXTURE]);
		glDeleteLists(fontBase0, 256);
		myGlobalGame->mlog.LogSuccessText(LString("Textured Font succesfully released!"));
	}

	RX_API FontColor Color(float r, float g, float b)
	{
		FontColor color;

		color.r = r, color.g = g, color.b = b;

		return color;
	}

	RX_API FontColor4 Color4(float r, float g, float b, float a)
	{
		FontColor4 color;

		color.r = r, color.g = g, color.b = b, color.a = a;

		return color;
	}

	/* OOP Bitmap font object class */
	bool FontObject::Create(char* fontName, int height, /* 0 - 9*/int fontWeight)
	{
		myGlobalGame->mlog.LogInfo(LString("Bitmap Font engine init started..."));

		this->fontBase = glGenLists(iMaxChars);
		strcpy(this->fontName, fontName);
		
		this->actualHeight = height;
		this->h_into_2 = height>>1;
		this->h = this->h_into_2>>1;

		fontWeight = fontWeight<0?0:fontWeight;
		fontWeight = fontWeight>9?900:fontWeight*100;

		HFONT hFont;
		HDC hDC = myGlobalGame->getApp()->getDC();

		if(stricmp(fontName, "symbol") == 0)
		{
			  hFont = CreateFont(height, 0, 0, 0, fontWeight, FALSE, FALSE, FALSE,
								 SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
								 ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
								 fontName);
		} else {
			  hFont = CreateFont(height, 0, 0, 0, fontWeight, FALSE, FALSE, FALSE,
								 ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
								 ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
								 fontName);
		}

		if(!hFont)
		{
			myGlobalGame->mlog.LogError(LString("%s font using Bitmap Font Engine failed to start, hFont is NULL", this->fontName));
			return false;
		}

		SelectObject(hDC, hFont);

		wglUseFontBitmaps(hDC, 0, iMaxChars-1, this->fontBase);
		
		myGlobalGame->mlog.LogSuccessText(LString("%s Font using Bitmap Font Engine succesfully Loaded!", this->fontName));

		return true;
	}

	void FontObject::Render(int x, int y, FontColor4 color, const char* fmt, ...)
	{
		glColor4f(color.r, color.g, color.b, color.a);

		va_list ap;
		char text[1024];

		if(!fmt)
			return;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);	
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, 1024, 768, 0, 0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		int len = strlen(text);

		if(x == FONT_CENTER)
		{
			x = 512 - len*this->h;
		} else if(x == FONT_MIN)
		{
			x = 0;
		} else if(x == FONT_MAX)
		{
			x = 1024 - len*this->h_into_2;
		}

		if(y == FONT_CENTER)
		{
			y = 384;
		} else if(y == FONT_MIN)
		{
			y = this->h*4;
		} else if(y == FONT_MAX)
		{
			y = 768 - this->h_into_2;
		}

		glRasterPos2i(x, y);
		glPushAttrib(GL_LIST_BIT);
			glListBase(this->fontBase);		
			glCallLists((GLsizei)len, GL_UNSIGNED_BYTE, text);
		glPopAttrib();
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();		
	}

	void FontObject::stripRender(int x, int y, FontColor4 color, const char* fmt, ...)
	{
		glColor4f(color.r, color.g, color.b, color.a);

		va_list ap;
		char text[1024];

		if(!fmt)
			return;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);	

		glRasterPos2i(x, y);
		glListBase(this->fontBase);		
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	}

	FontObject::~FontObject()
	{
		this->Free();
	}

	void FontObject::Free()
	{
		if(this->fontBase)
		{
			glDeleteLists(this->fontBase, iMaxChars);
			myGlobalGame->mlog.LogSuccessText(LString("%s font for Bitmap Font Engine succesfully Released!", this->fontName));
		}
	}

	void FontObject::lclear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void FontObject::lRender(int x, int y, FontColor4 color, const char* fmt, ...)
	{
		glColor4f(color.r, color.g, color.b, color.a);

		va_list ap;
		char text[1024];

		if(!fmt)
			return;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);	
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, 1024, 768, 0, 0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		int len = strlen(text);

		if(x == FONT_CENTER)
		{
			x = 512 - len*this->h;
		} else if(x == FONT_MIN)
		{
			x = 0;
		} else if(x == FONT_MAX)
		{
			x = 1024 - len*this->h_into_2;
		}

		if(y == FONT_CENTER)
		{
			y = 384;
		} else if(y == FONT_MIN)
		{
			y = this->h*4;
		} else if(y == FONT_MAX)
		{
			y = 768 - this->h_into_2;
		}

		glRasterPos2i(x, y);
		glPushAttrib(GL_LIST_BIT);
			glListBase(this->fontBase);		
			glCallLists((GLsizei)len, GL_UNSIGNED_BYTE, text);
		glPopAttrib();
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		SwapBuffers(myGlobalGame->getApp()->getDC());
	}

	void FontObject::lcRender(int x, int y, FontColor4 color, const char* fmt, ...)
	{
		this->lclear();
		glColor4f(color.r, color.g, color.b, color.a);

		va_list ap;
		char text[1024];

		if(!fmt)
			return;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);	
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, 1024, 768, 0, 0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		int len = strlen(text);

		if(x == FONT_CENTER)
		{
			x = 512 - len*this->h;
		} else if(x == FONT_MIN)
		{
			x = 0;
		} else if(x == FONT_MAX)
		{
			x = 1024 - len*this->h_into_2;
		}

		if(y == FONT_CENTER)
		{
			y = 384;
		} else if(y == FONT_MIN)
		{
			y = this->h*4;
		} else if(y == FONT_MAX)
		{
			y = 768 - this->h_into_2;
		}

		glRasterPos2i(x, y);
		glPushAttrib(GL_LIST_BIT);
			glListBase(this->fontBase);		
			glCallLists((GLsizei)len, GL_UNSIGNED_BYTE, text);
		glPopAttrib();
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		SwapBuffers(myGlobalGame->getApp()->getDC());
	}

	FontObject::FontObject()
	{
	}

	int FontObject::getHeight(void)
	{
		return this->actualHeight;
	}

	RX_API void Print(int x, int y, int set, const char *fmt, ...)
	{
		va_list ap;
		char string[1024];;

		va_start(ap, fmt);
		vsprintf(string, fmt, ap);
		va_end(ap);	
		
		if (set>1)	set=1;
		glEnable(GL_BLEND);	
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);									

		glBindTexture(GL_TEXTURE_2D, g_Textures[FONT_TEXTURE]);		
		glDisable(GL_DEPTH_TEST);							
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();		
		glLoadIdentity();	
		glOrtho(0, 1024, 0, 768, 0, 1);	
		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();			
		glLoadIdentity();		
		glTranslated(x, y, 0);	
		glListBase(fontBase0-32+(128*set));
		glCallLists(strlen(string),GL_BYTE,string);	
		glMatrixMode(GL_PROJECTION);			
		glPopMatrix();						
		glMatrixMode(GL_MODELVIEW);		
		glPopMatrix();		
		
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);	
	}

	RX_API void sPrint(int x, int y, int set, const char *fmt, ...)
	{
		va_list ap;
		char string[1024];;

		va_start(ap, fmt);
		vsprintf(string, fmt, ap);
		va_end(ap);	
		
		if (set>1)	set=1;					
		
		//glRasterPos2i(x, y);
		glPushMatrix();
			glTranslated(x, y, 0);	
			glScalef(1, -1, 1);
			glListBase(fontBase0-32+(128*set));
			glCallLists(strlen(string),GL_BYTE,string);	
		glPopMatrix();
	}
}