/*
========================================================
	class BaseConsole
========================================================
*/

#ifndef BASE_CONSOLE_H
#define BASE_CONSOLE_H

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)

#include <string>
#include <vector>
#include <sstream>
#include "UtilsClient.h"
#include "Log.h"

namespace DifferentialArts
{
	struct STRING
	{
		unsigned char r, g, b;
		char contents[256];
	};

	typedef struct tFNPTR
	{
		int type;
		void (*mVoidFNPTR)(std::string &parameters);
	} FNPTR_HOLDER;

	/*
	typedef struct tMEMBER_FNPTR
	{
		int type;
		void (__cdecl * mVoidFNPTR)(void);
	} MEMBER_FNPTR_HOLDER;*/
	template <class TYPE> class MEMBER_FNPTR_HOLDER
	{
	public:
		int type;
		void (TYPE::* mVoidFNPTR)(void);
	};

	enum _1VP_TYPES
	{
		FLOAT_TYPE_VP,
		INT_TYPE_VP,
		BOOL_TYPE_VP,
		STRING_TYPE_VP,
	};

	struct _1VP
	{
		_1VP_TYPES type;
		int components;
		STRING definition;
		float* fp1;
		float* fp2;
		float* fp3;
		float* fp4;
		char* bindString;

		int* ip1;
		int* ip2;
		int* ip3;
		int* ip4;

		bool* bp;
	};

	/*
	==============================================
		BaseConsole Class 
	==============================================
	*/

	/*!	\class BaseConsole
		\brief BaseConsole class
		\author Rajesh Peter Douglas D'Monte	

		This console class is used inside the main engine class. Use this help
		as a reference on how to use the console methods.
	*/
	class RX_UTIL_API BaseConsole
	{
	public:
		BaseConsole();//!< Constructor
		~BaseConsole();//!< Destructor

		virtual void draw(void) = 0; //!< Renders the console on screen
		virtual void operate(float dt) = 0; //!< This should be called in the Update method of the engine.
		void init(void);//!< Initializes the console

		void toggleActivate(void);//!< Toggles the console on or off.				
		bool isInitialized(void);//!< Returns true if the console is initialized
		void addString(const char* txt, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255); //!< Adds a string to the console with a given color
		void pAddString(const char* txt, ...); //!< Adds a string with formatting to the console

		void bindStringVariable(const char* varName, char* strName); 
		//!< Binds a string variable to the console. It should always point to a 512 sized char array.

		void bindFloatVariable(const char* varName, float* Pointer1);
		//!< Binds a float to the console.
		void bindFloatVariable(const char* varName, float* Pointer1, float* Pointer2);
		//!< Binds 2 floats to the console.
		void bindFloatVariable(const char* varName, float* Pointer1, float* Pointer2, float* Pointer3);
		//!< Binds 3 floats to the console.
		void bindFloatVariable(const char* varName, float* Pointer1, float* Pointer2, float* Pointer3,
							   float* Pointer4);
		//!< Binds 4 floats to the console.

		void bindIntegerVariable(const char* varName, int* Pointer1);
		//!< Binds an integer to the console.
		void bindIntegerVariable(const char* varName, int* Pointer1, int* Pointer2);
		//!< Binds 2 integers to the console.
		void bindIntegerVariable(const char* varName, int* Pointer1, int* Pointer2, int* Pointer3);
		//!< Binds 3 integers to the console.
		void bindIntegerVariable(const char* varName, int* Pointer1, int* Pointer2, int* Pointer3,
							   int* Pointer4);
		//!< Binds 4 integers to the console.

		void bindBooleanVariable(const char* varName, bool* Pointer1);
		//!< Binds a boolean to the console.

		void bindCommand(const char* cmdStr, void (*mVoidFNPTR)(std::string &parameters)); /* STRICT WARNING: ONLY PASS VOID FUNCTIONS WITH VOID PARAMETERS */
		//!<Binds a function to the console. Only bind void C functions.
		//template<class T> void bindMemberVoidCommand(const char* cmdStr, void (T::*givenFunction)()); //DO NOT USE... EXPERIMENT
		bool isActivated(void);	//!< Returns true if the console is activated
		// 'void' to 'void (__cdecl *)(void)'

		void executeCommand(const char* cmdStr, const char* user = 0); //!< Executes a console command,  Commands are not case sensitive*
		std::string getVariable(const char* cmdStr); //!< Returns a string of information of the bound variable

		Log log;

		//void OutText (const char* txt);//!< Use the macro LogText as a BaseConsole member
		/*!<
			When using this method LogText(str)
			
			Eg: \a mConsole.LogText(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutInfo (const char* txt);//!< Use the macro LogInfo as a BaseConsole member
		/*!<
			When using this method LogInfo(str)
			
			Eg: \a mConsole.LogInfo(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutSuccessText (const char* txt);//!< Use the macro LogSuccessText as a BaseConsole member
		/*!<
			When using this method LogSuccessText(str)
			
			Eg: \a mConsole.LogSuccessText(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutWarning (const char *strfile, const char *func, const int line, const char* txt);//!< Use the macro LogWarning as a BaseConsole member
		/*!<
			When using this method LogWarning(str)
			
			Eg: \a mConsole.LogWarning(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutError (const char *strfile, const char *func, int line, const char* txt);//!< Use the macro LogError as a BaseConsole member
		/*!<
			When using this method LogError(str)
			
			Eg: \a mConsole.LogError(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

		void OutCritical (const char *strfile, const char *func, int line, const char* txt);//!< Use the macro LogCritical as a BaseConsole member
		/*!<
			When using this method LogCritical(str)
			
			Eg: \a mConsole.LogCritical(LString("test %d", num));

			Make sure you log string doesn't exceed 1024 characters, or a crash WILL occur.
		*/

	protected:
		bool bConsoleFullScreen;
		bool bBlink;
		char cursorChar;
		char insertCursorChar;
		float cursorTime;
		float runTime;
		//unsigned int mTexture;
		bool bActivate;
		bool bInit;
		float y;
		float targetY;
		int consoleLnPos;	
		char currentCommand[256];
		int visibleLines;		
		bool bEchoUserCommands;

		int scroll;
		std::vector<STRING> strings;

		//For Bound variables
		std::vector<_1VP> boundVariables;
		std::vector<_1VP>::iterator boundVariables_iterator;

		//For DOSKEY-like support
		std::vector<STRING> commands;
		unsigned int currentLetter;
		int command_current;	

		bool bQuitBound;
		bool bInsertMode;

		//For bound commands
		std::vector<STRING> storedVoidFunctionCommands;
		std::vector<STRING>::iterator storedVoidFunctionCommands_iterator;
		std::vector<FNPTR_HOLDER> functionPointers;
		std::vector<FNPTR_HOLDER>::iterator functionPointers_iterator;
		
		/*
		std::vector<STRING> stored_member_VoidFunctionCommands;
		std::vector<STRING>::iterator stored_member_VoidFunctionCommands_iterator;
		std::vector<MEMBER_FNPTR_HOLDER> _member_functionPointers;
		std::vector<MEMBER_FNPTR_HOLDER>::iterator _member_functionPointers_iterator;*/
	};

	extern RX_UTIL_API BaseConsole* gConsole; 
}

#endif