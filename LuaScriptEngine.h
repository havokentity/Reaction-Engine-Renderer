/*
	Description: Lua Script engine and manager code

	classes contained are:

	class LuaScriptEngine

	class LuaScript
*/

#ifndef RX_LUA_SCRIPT_ENGINE_H_
#define RX_LUA_SCRIPT_ENGINE_H_

#define RX_USE_LUA_5_0

#ifdef RX_USE_LUA_5_0
	#ifdef NDEBUG
		#pragma comment(lib, "lua.lib")
		//#pragma comment(lib, "lua50.lib")
		//#pragma comment(lib, "tolua++.lib")
	#else
		#pragma comment(lib, "lua_d.lib")
		//#pragma comment(lib, "lua50_d.lib")
		//#pragma comment(lib, "tolua++_d.lib")
	#endif
#else
//#pragma comment(lib, "lua5.1.lib")
#endif

extern "C"
{
    #include <lua.h>
	#include <lauxlib.h>
    #include <lualib.h>
};

#include <stdio.h>

#include "Client.h"

namespace DifferentialArts
{
	class RX_API LuaScript;
	/*
	==============================================================================
		class LuaScriptEngine

		This is the main Lua Scripting class, the main engine object has an
		instance of this class. Use that instead. Don't declare any instance
		of this class anywhere in your Reaction program.
	==============================================================================
	*/

	/*! \enum RX_LUA_SCRIPT_STATE
		\brief Lua script state enumeration
	*/
	typedef enum tRX_lua_ScriptState
	{
		RX_LUA_WAIT_TIME,//!< Lua script wait for some time state
		RX_LUA_WAIT_FRAME, //!< Lua script wait for some frames state
		RX_LUA_RUNNING,//!< Lua script running state
		RX_LUA_NOT_LOADED,//!< Lua script not loaded state
		RX_LUA_DONE//!< Lua script done state
	} RX_LUA_SCRIPT_STATE;

	/*! \class LuaScriptEngine
		\brief Lua Scripting manager class

		This is the main Lua Scripting class, the main engine object has an
		instance of this class. Use that instead. Don't declare any instance
		of this class anywhere in your Reaction program. The engine initializes an
		instance of this class also for you, so that you don't have to worry about
		the hassle of maintaining Lua states.
	*/
	class RX_API LuaScriptEngine
	{
	public:
		LuaScriptEngine();//!<Constructor
		~LuaScriptEngine();//!<DeConstructor

		bool Initialize(bool loadDefaultLibraries = true); //!<This function initializes LuaScriptEngine.
		/*!<
			This function initializes the LuaScriptEngine object.

			\param loadDefaultLibraries Specify true if you want to initialize LUA's default libraries

			@return true if successfull
		*/
		void Free(void); //!<Frees the lua scripting engine	and all linked scripts.
		lua_State* getState(void); //!<Returns the lua state. Needed for making use of LUA.
		bool setExtraStackSize(int n); //!<Sets the virtual stack size that LUA uses. Lua may not perform.

		LuaScript* NewScript(void);//!< Returns a pointer to a newly created script
		/*!<
			This method creates a script and pushes it onto the LuaScriptEngine instance's scripts list.

			@return Pointer to newly created script
		*/

		bool UnlinkScript(LuaScript* pScript); //!< This method unlinks the given script object.
		/*!<
			This method unlinks the provided script from the LuaScriptEngine instance scripting list

			\param pScript Pointer to a LuaScript instance to be unlinked

			@return false if theres no such script linked. true if successfull.
		*/

		void Update(float dt);//!< This method is used to update all scripts.
		/*!<
			This method is used to update all scripts

			\param dt Time interval between the present frame and the last in seconds
		*/

		int GetScriptCount(void); //!< This method returns the total scripts linked to the scripting engine.

	private:
		lua_State* mpLuaState;	
		LuaScript* head;
		int miCount;
	};

	/*
	===============================================================================
		class LuaScript

		Make sure all filenames or paths are less than 1024 characters long
	===============================================================================
	*/

	/*! \class LuaScript
		\brief Lua script class
		\A Lua script class.

		For loading, running and managing lua scripts
	*/
	class RX_API LuaScript
	{
	public:
		LuaScript(); //!<Constructor
		LuaScript(char* fn); //!<Constructor that passes the Lua script file name into the object
		LuaScript(lua_State* pState, char* fn); //!<Constructor that creates a free form un managed lua script and loads and runs the file name and closes the script
		~LuaScript();//!<Destructor - Free's script

		lua_State* getState(void); //!<Returns the lua state of the script thread
		void SetSourcePath(char* fn);//!<Sets the lua script file path
		/*!<
			This method is used to Set the path of the script to be run.

			\param fn The file path.
		*/

		LuaScript* next;//!< Pointer to the next lua script. To be used by LuaScriptEngine only.

		LuaScript* Update(float dt);//!< Updates the script. 
		/*!<
			Updates the script. Make sure LuaScriptEngine updates the script alone.
			It is highly recommended you DON'T call Update, let the LuaScriptEngine update
			all linked scripts

			\param dt Difference between two frames in seconds

			@return A pointer to the next script.
		*/

		void LoadLuaScriptFile(char* fileName, bool executeOnLoad = false); //!<Loads and runs a lua script
		/*!<
			This method loads and runs a lua script

			\param fileName The lua script file path on disk
			\param executeOnLoad This boolean specifies if the lua script should execute itself on loading
		*/

		void ResumeScript(float param);//!< This method resumes the script from idle state.
		/*!<
			Resumes a script from idle state.

			\param param The parameter to this function is pushed on the stack as the return value
			of the previous yield.
		*/

		int RunString(char* commandString);//!<Runs a single line of lua script
		/*!<
			This method is used to compile and execute a single line of lua script

			\param commandString The string of lua script

			@return -1 if there was an error and 0 if there was no error
		*/

	private:
		bool bFreeForm;

		void FormatError(void);
		void OutputError(char*   strType);

		char mFileName[1024];
		char lastErrorString[1024];

		lua_State* mpLuaState;

		RX_LUA_SCRIPT_STATE  state; //Script state
		float mfWaitTime;// Wait time
		int miWaitFrame;//Wait frames
		float mfTime;// Current time

		LuaScript(LuaScriptEngine* pManager); //!<Constructor that passes a manager pointer to the script object
		LuaScriptEngine* mpManager;

		friend class LuaScriptEngine;

		RX_API friend int LuaWaitFrame(lua_State* l);
		RX_API friend int LuaWaitTime(lua_State* l);
		RX_API friend int LuaWaitSec(lua_State* l);
		RX_API friend int LuaDeleteScript(lua_State* l);		
	};

	RX_API int LuaLogSuccessText(lua_State* pState);
}

#endif