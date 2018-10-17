/*
	Description - class for outputting errorrs, status, etc of the program
	to a text file for helping in debugging

	$classes contained,
	class Log
*/

#ifndef LOG_H
#define LOG_H

/*! \file log.h
    \brief Log header
    
    This is the Reaction Engine logger header. It contains the
	Log class. You need not use this class or header in your project
	as Reaction Engine uses it internally.

	It also contains macros that should be used with all instances of 
	the Log class. 
*/

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)

#include <stdio.h>
#include <time.h>

//#include "Console.h"
#include "UtilsClient.h"

namespace DifferentialArts
{
	//#define END_TAG  "</font>"

	//Logging macros
	#define	LogSuccessText(str)	OutSuccessText(str) 
	/*! \def LogSuccessText(str)
		\brief A macro that logs success text using the Log class and appends a new line character at the end

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	#define	LogText(str)	OutText(str)
	/*! \def LogText(str)
		\brief A macro that logs text using the Log class

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	#define	LogInfo(str)	OutInfo(str)
	/*! \def LogInfo(str)
		\brief A macro that logs information text using the Log class and appends a new line character at the end

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	#define	LogWarning(str)	OutWarning(__FILE__, __FUNCTION__, __LINE__, str)
	/*! \def LogWarning(str)
		\brief A macro that logs warning text using the Log class and appends a new line character at the end

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	#define	LogError(str)	OutError(__FILE__, __FUNCTION__, __LINE__, str)
	/*! \def LogError(str)
		\brief A macro that logs error text using the Log class and appends a new line character at the end

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	#define	LogCritical(str)	OutCritical(__FILE__, __FUNCTION__, __LINE__, str)
	/*! \def LogCritical(str)
		\brief A macro that logs critical error text using the Log class and appends a new line character at the end

		Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
	*/

	//Decorators
	/*
		#define	DEC_SUCCESS		"font color=#00ff00 size = 3"
		#define	DEC_INFO		"font color=#ffffff size = 3"
		#define	DEC_WARNING		"font color=#ffff00 size = 4"
		#define	DEC_ERROR		"font color=#ff0000 size = 4"
		#define	DEC_CRITICAL	"font color=#ff00ff size = 4"
	*/

	#define	DEC_SUCCESS		"div class='success'"
	#define	DEC_INFO		"div class='info'"
	#define	DEC_WARNING		"div class='warning'"
	#define	DEC_ERROR		"div class='error'"
	#define	DEC_CRITICAL	"div class='critical'"

	/*
	================================
		class Log
	================================
	*/


	/*!	\class Log
		\brief Handles all engine logging
		\author Rajesh Peter Douglas D'Monte

		An instance of this class exists in CoreEngine.

		It can be used as follows,
		\a myGlobalGame->mlog.LogError(LString("test %d", num));

		Notice that we don't use the Log methods, instead we use special Log Macros
	*/

	class RX_UTIL_API Log
	{
	private:
		FILE* file;
		char fileName[256];
		int ln;

		void OutLog(const char *decorator, const char *file, const char *func, int line, const char* txt);
		void OutLog(const char *decorator, const char *txt);

	public:
		Log (void); //!<Constructor
		~Log (void);//!<Deconstructor. Closes file pointer.

		bool Init (const char* strLog); //!< Initializes the logger. 
		/*!<
			You needn't initialize the logger as myGlobalGame initializes after
			it is initialized.

			@return true if it succeeds
		*/

		void OutText (const char* txt);//!< Use the macro LogText as a Log member
		/*!<
			When using this method LogText(str)
			
			Eg: \a myGlobalGame->mlog.LogText(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutInfo (const char* txt);//!< Use the macro LogInfo as a Log member
		/*!<
			When using this method LogInfo(str)
			
			Eg: \a myGlobalGame->mlog.LogInfo(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutSuccessText (const char* txt);//!< Use the macro LogSuccessText as a Log member
		/*!<
			When using this method LogSuccessText(str)
			
			Eg: \a myGlobalGame->mlog.LogSuccessText(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutWarning (const char *strfile, const char *func, const int line, const char* txt);//!< Use the macro LogWarning as a Log member
		/*!<
			When using this method LogWarning(str)
			
			Eg: \a myGlobalGame->mlog.LogWarning(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutError (const char *strfile, const char *func, int line, const char* txt);//!< Use the macro LogError as a Log member
		/*!<
			When using this method LogError(str)
			
			Eg: \a myGlobalGame->mlog.LogError(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutCritical (const char *strfile, const char *func, int line, const char* txt);//!< Use the macro LogCritical as a Log member
		/*!<
			When using this method LogCritical(str)
			
			Eg: \a myGlobalGame->mlog.LogCritical(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		//Console mConsole; //!< This is an instance of the console for input/output. Don't create any more instances.

		friend class RX_UTIL_API BaseConsole;
	};

	//Global routine to process argument list.
	RX_UTIL_API char* LString(char *txt, ...);
}

#endif