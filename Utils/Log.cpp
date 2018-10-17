/*
	THIS CODE WAS TAKEN FROM MY OLDER
	DIRECTX 2D GAME FRAMEWORK

	NO CHANGES WERE MADE TO IT,
	IT WAS USED DIRECTLY
*/

/*
	Author : Rajesh Peter Douglas D'Monte
	Last Modified: 25 April 2005
	Description - refer 'log.h'

*/
#include <stdarg.h>

#include "log.h"

/*
void* operator new (size_t size)
{
	void *p=_aligned_malloc(size, 16);
	if (p==0) // did malloc succeed?
		return NULL;
	return p;
}

void* operator new[] (size_t size)
{
	void *mem = _mm_malloc(size, 16);
	return mem;
}*/

namespace DifferentialArts
{
	//Returns the processed arg list after storing it in a global string.
	RX_UTIL_API char* LString(char *txt, ...)
	{
		static char	str[1024];		//<PARAM: watch the buffer size...>

		va_list argList;
		va_start (argList, txt);	

		vsprintf(str, txt, argList);
		va_end (argList);

		return str;
	}


	Log::Log (void)
	{
		file = NULL;
	}

	Log::~Log (void)
	{
		if(file)
			fclose(file);
	}

	bool Log::Init (const char* strLog)
	{
		ln = 0;
		sprintf(fileName, strLog);

		//Open the file with commit flag on and 
		//sequential access caching indicator.
		file = fopen (fileName, "wcSt");

		if(!file)
		{
			return false;
		}

		//Disable buffering. We want immediate write.
		setvbuf(file, NULL, _IONBF, 0);

		//Get the timestamp.
		time_t curTime;
		time(& curTime);

		/*
			<style type="text/css">
			body { background: #1a242a; color: #b4c8d2; margin-right: 20px; margin-left: 20px; font-size: 14px; font-family: Arial, sans-serif, sans; }
			a { text-decoration: none; }
			a:link { color: #b4c8d2; }
			a:active { color: #ff9900; }
			a:visited { color: #b4c8d2; }
			a:hover { color: #ff9900; }
			h1 { text-align: center; }
			h2 { color: #ffffff; }
			.message, .warning, .error, .fatal { padding: 3px; }
			.message { background-color: #080c10; color: #b4c8d2; }
			.warning { background-color: #839ca7; color: #1a242a; }
			.error { background-color: #ff9933; color: #1a242a; }
			.fatal { background-color: #ff0000; color: #1a242a; }
			</style>
		*/



		OutText( LString("<?xml version='1.0' encoding='utf-8' standalone='yes'?>\n") );
		OutText( LString("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n") );

		OutText( LString("<html><head><title>Reaction Engine Log</title>\n<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>\n") );

		OutText( LString("<style type='text/css'>\n") );
		OutText( LString("body { background: #444444; color: #ffffff; margin-right: 20px; margin-left: 20px; font-size: 14px; font-family: Arial, sans-serif, sans; }\n") );	
		OutText( LString("h1 { text-align: center; }\nh2 { color: #ffffff; }\n") );	

		OutText( LString(".success .info, .warning, .error, .critical .num { padding: 3px; }\n") );	

		OutText( LString(".success { background-color: #080c10; color: #b4ffd2; text-align: left; }\n") );
		OutText( LString(".success:hover { border: solid 1px #6983B3; background-color: #d0ffe1; color: #000000; font-weight: bold }\n") );

		OutText( LString(".info { background-color: #080c10; color: #b4c8d2; text-align: left; }\n") );
		OutText( LString(".info:hover { border: solid 1px #6983B3; background-color: #E1E4F2; color: #000000; font-weight: bold }\n") );

		OutText( LString(".warning { background-color: #839ca7; color: #1a242a; text-align: left; }\n") );
		OutText( LString(".warning:hover { border: solid 1px #6983B3; background-color: #b1b4FF; color: #000000; font-weight: bold }\n") );

		OutText( LString(".error { background-color: #FF7777; color: #1a242a; text-align: left; }\n") );
		OutText( LString(".error:hover { border: solid 1px #6983B3; background-color: #dD2D2D; color: #000000; font-weight: bold }\n") );

		OutText( LString(".critical { background-color: #E48CFB; color: #1a242a; text-align: left; }\n") );
		OutText( LString(".critical:hover { border: solid 1px #6983B3; background-color: #D95AFA; color: #000000; font-weight: bold }\n") );

		OutText( LString(".num { color: #1a242a; text-align: right; font-weight: bold }\n") );
		OutText( LString(".num:hover { border: solid 0px #6983B3; background-color: #080c10; color: #b4c8d2; font-weight: bold }\n") );

		OutText( LString("table { table-layout: fixed; border-spacing: 0px; }\n") );	

		OutText( LString("</style>\n") );	

		const char percent = '%';

		OutText( LString("</head><body text=#ffffff><table><tr><td align = 'left' width = 77%c><h2>Reaction Engine Log Started</h2></td>", percent) );
		OutText( LString("<font size=3 face = \"verdana\"><td align = 'right'><b>Log Init Time: </b><u>%s</u><br> <b>Version: </b><u>"CLIENT_VERSION"</u></b></td></table>", ctime(&curTime)) );	
		OutText("</tr><br><table>");

		return true;
	}

	//Normal log message.
	void Log::OutLog (const char *decorator, const char *strfile, const char *func, int line, const char* txt) 
	{	
		fprintf(file, "<tr><td bgcolor = #ff9933 height = 25px> <div class = 'num'>%d&nbsp;</div></td><td bgcolor = #000000 height = 25px><%s>  <u>File:</u><i> %s </i><u>Function:</u><i> %s </i><u>Line:</u><i> %d: </i> %s</div></td></tr>", ln++, decorator, strfile, func, line, txt);

		fflush(file);
	}

	//Direct output to stream.
	void Log::OutLog (const char *decorator, const char *txt)
	{
		if(decorator)	
		{
			//Info.
			//fprintf(file, "<%s> %d: %s</div>", decorator, ln++, txt);
			fprintf(file, "<tr><td bgcolor = #ff9933 height = 25px> <div class = 'num'>%d&nbsp;</div></td><td bgcolor = #000000 height = 25px><%s> %s</div></td></tr>", ln++, decorator, txt);
		}
		else
		{		
			//Plain text.
			fprintf(file, "%s", txt);
		}

		fflush(file);
	}

	void Log::OutText(const char* txt)
	{
		OutLog(NULL, txt);
	}

	void Log::OutSuccessText(const char* txt)
	{
		OutLog(DEC_SUCCESS, txt);
		//mConsole.addString(txt, 60, 187, 149);
	}

	void Log::OutInfo(const char* txt)
	{
		OutLog(DEC_INFO, txt);
		//mConsole.addString(txt);
	}

	void Log::OutWarning(const char *strfile, const char *func, int line, const char* txt)
	{
		OutLog(DEC_WARNING, strfile, func, line, txt);
		//mConsole.addString(txt, 241, 241, 133);
	}

	void Log::OutError(const char *strfile, const char *func, int line, const char* txt)
	{
		OutLog(DEC_ERROR, strfile, func, line, txt);
		//mConsole.addString(txt, 221, 0, 0);
	}

	void Log::OutCritical(const char *strfile, const char *func, int line, const char* txt)
	{
		OutLog(DEC_CRITICAL, strfile, func, line, txt);
		//mConsole.addString(txt, 172, 7, 214);
	}
}