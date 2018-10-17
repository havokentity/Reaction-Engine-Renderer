/*
	Description: Lua Script engine and manager code

	classes contained are:

	class LuaScriptEngine
*/

#include "LuaScriptEngine.h"
#include "CoreEngine.h"

namespace DifferentialArts
{
	/*
	======================================================
		LuaScriptEngine constructor
	======================================================
	*/
	LuaScriptEngine::LuaScriptEngine(void)
	{
		this->head = 0;
		this->mpLuaState = 0;
		this->miCount = 0;
	}

	/*
	======================================================
		LuaScriptEngine destructor
	======================================================
	*/
	LuaScriptEngine::~LuaScriptEngine(void)
	{
		//this->Free();	
	}

	/*
	======================================================
		LuaScriptEngine::Free
	======================================================
	*/
	void LuaScriptEngine::Free(void)
	{
		myGlobalGame->mlog.LogInfo(LString("Freeing LUA scripting engine..."));
		// Cleanup Lua

		LuaScript*  s;
		LuaScript*  next;

		// destroy all children scripts
		s = head;
		while(s)
		{
			// save the next pointer in case the script is deleted
			next = s->next;
			delete s;
			s = next;
		}

		this->miCount = 0;
		
		if(mpLuaState)
		{	
			lua_close(mpLuaState);
			mpLuaState = 0;
			myGlobalGame->mlog.LogSuccessText(LString("LUA scripting engine successfully freed!"));
		} else {
			myGlobalGame->mlog.LogError(LString("LUA::Free was called by user or by destructor without being initialized!"));
		}
	}

	int LuaScriptEngine::GetScriptCount(void)
	{
		return this->miCount;
	}

	/*
	=====================================================
			Creates a new script and pushes onto
			the script list 
	=====================================================
	*/
	LuaScript* LuaScriptEngine::NewScript(void)
	{
		LuaScript*  s;

		// create a script object
		s = new LuaScript(this);

		// link the new script into the list
		s->next = head;
		head = s;

		this->miCount++;

		return(s);
	}

	/*
	=====================================================
			Lua Engine communication libraries
	=====================================================
	*/

	/*
	=====================================================
			Lua +  Engine logger
	=====================================================
	*/
	RX_API int LuaLogSuccessText(lua_State* pState)
	{
		//To pop variables off the stack:
		//float X = (float)lua_tonumber(scriptState, 1);
		//float Y = (float)lua_tonumber(scriptState, 2);
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.LogSuccessText(LString((char* )txt));

		return 0;
	}

	RX_API int LuaLogText(lua_State* pState)
	{
		//To pop variables off the stack:
		//float X = (float)lua_tonumber(scriptState, 1);
		//float Y = (float)lua_tonumber(scriptState, 2);
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.log.LogText(LString((char* )txt));

		return 0;
	}

	RX_API int LuaLogError(lua_State* pState)
	{
		//To pop variables off the stack:
		//float X = (float)lua_tonumber(scriptState, 1);
		//float Y = (float)lua_tonumber(scriptState, 2);
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.LogError(LString((char* )txt));

		return 0;
	}

	RX_API int LuaLogCritical(lua_State* pState)
	{
		//To pop variables off the stack:
		//float X = (float)lua_tonumber(scriptState, 1);
		//float Y = (float)lua_tonumber(scriptState, 2);
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.LogCritical(LString((char* )txt));

		return 0;
	}

	RX_API int LuaLogWarning(lua_State* pState)
	{
		//To pop variables off the stack:
		//float X = (float)lua_tonumber(scriptState, 1);
		//float Y = (float)lua_tonumber(scriptState, 2);
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.LogWarning(LString((char* )txt));

		return 0;
	}


	/*
	=====================================================
			Library for logging through Lua
	=====================================================
	*/
	RX_API const luaL_reg EngineLogLibrary[] =
	{
		{"LogSuccessText", LuaLogSuccessText },
		{"LogText", LuaLogText },
		{"LogError", LuaLogError },
		{"LogCritical", LuaLogCritical },
		{"LogWarning", LuaLogWarning },
		{NULL, NULL}
	};

	/*
	=====================================================
			Gets a script pointer by using the
			scripts lua state.
	=====================================================
	*/
	RX_API LuaScript* GetScriptObject(lua_State* pLuaState)
	{
		lua_pushlightuserdata(pLuaState, pLuaState);
		lua_gettable(pLuaState, LUA_GLOBALSINDEX);
		return ((LuaScript*)lua_touserdata(pLuaState, -1));
	}

	/*
	=====================================================
			Lua +  Script control
	=====================================================
	*/

	RX_API int LuaWaitFrame(lua_State* l)
	{
		LuaScript*  s;

		s = GetScriptObject(l);

		s->miWaitFrame = (int)luaL_checknumber(l, 1);
		s->state = RX_LUA_WAIT_FRAME;

		return(lua_yield(l, 1));
	}


	/*
	=====================================================
		LUA WAIT TIME
		Causes a script to wait until the given timestamp.
	=====================================================
	*/

	RX_API int LuaWaitTime(lua_State* l)
	{
		LuaScript*  s;

		s = GetScriptObject(l);

		s->mfWaitTime = (float)luaL_checknumber(l, 1);
		s->state = RX_LUA_WAIT_TIME;

		return(lua_yield(l, 1));
	}


	/*
	==========================================================================
		LUA WAIT SEC
		Causes a script to wait for the given number of seconds.
	==========================================================================
	*/

	RX_API int LuaWaitSec(lua_State* l)
	{
		LuaScript*  s;

		s = GetScriptObject(l);

		s->mfWaitTime = s->mfTime + (float)luaL_checknumber(l, 1);
		s->state = RX_LUA_WAIT_TIME;

		return(lua_yield(l, 1));
	}

	/*
	==========================================================================
		LUA DELETE SCRIPT
		Causes a script to delete and unlink itself from script manager
	==========================================================================
	*/

	RX_API int LuaDeleteScript(lua_State* l)
	{
		LuaScript*  s;

		s = GetScriptObject(l);

		//s->mfWaitTime = s->mfTime + (float)luaL_checknumber(l, 1);
		s->state = RX_LUA_DONE;
		//myGlobalGame->mLuaScriptEngine->UnlinkScript(s);
		//delete s;

		return(lua_yield(l, 1));
	}
	/*
	=====================================================
			Library for script control through lua
	=====================================================
	*/
	RX_API const luaL_reg EngineScriptLibrary[] =
	{
		{"WaitTime", LuaWaitTime },
		{"WaitSec", LuaWaitSec },
		{"WaitFrame", LuaWaitFrame },
		{"Delete", LuaDeleteScript },
		{NULL, NULL}
	};

	/*
	=====================================================
			Console library for lua
	=====================================================
	*/

	/*
	==========================================================================
		LUA SendCommand
	==========================================================================
	*/
	RX_API int LuaSendCommandToConsole(lua_State* pState)
	{
		const char* txt = lua_tostring(pState, 1);
	 
		myGlobalGame->mlog.executeCommand(txt);

		return 0;
	}

	/*
	=====================================================
			Console library for lua bindings
	=====================================================
	*/
	RX_API const luaL_reg EngineConsoleLibrary[] =
	{
		{"SendCommand", LuaSendCommandToConsole },
		{NULL, NULL}
	};

	/*
	======================================================
		LuaScriptEngine::Initialize
	======================================================
	*/
	bool LuaScriptEngine::Initialize(bool loadDefaultLibraries)
	{
		myGlobalGame->mlog.LogInfo(LString("Initializing LUA scripting engine..."));
		

	#ifdef RX_USE_LUA_5_0
		this->mpLuaState = lua_open();	

		if(!this->mpLuaState)
		{
			myGlobalGame->mlog.LogError(LString("LUA scripting engine failed to initialize!"));
			return false;
		}

		// Load Lua libraries
		if(loadDefaultLibraries)
		{
			luaopen_base(this->mpLuaState);
			luaopen_table(this->mpLuaState);
			//luaopen_io(this->mpLuaState);
			luaopen_string(this->mpLuaState);
			luaopen_math(this->mpLuaState);
			luaopen_debug(this->mpLuaState);
		}

		/*
		luaL_newmetatable(this->mpLuaState, "RxEngine");

		//metatable.__index = metatable 
		lua_pushliteral(this->mpLuaState, "__index");
		lua_pushvalue(this->mpLuaState, -2);
		lua_rawset(this->mpLuaState, -3);

		//register methods
		luaL_openlib (this->mpLuaState, NULL, EnginelogObject, 0);*/

		luaL_openlib(this->mpLuaState, "log", EngineLogLibrary, 0);
		luaL_openlib(this->mpLuaState, "script", EngineScriptLibrary, 0);
		luaL_openlib(this->mpLuaState, "console", EngineConsoleLibrary, 0);

	#else
		this->mpLuaState = luaL_newstate();	

		if(!this->mpLuaState)
		{
			myGlobalGame->mlog.LogError(LString("LUA scripting engine failed to initialize!"));
			return false;
		}

		// Load Lua libraries
		if(loadDefaultLibraries)
		{
			luaL_openlibs(this->mpLuaState);
			myGlobalGame->mlog.LogInfo(LString("LUA default libraries were loaded..."));
		}
	#endif

		myGlobalGame->mlog.LogSuccessText(LString("LUA scripting engine successfully initialized!"));

		return true;
	}

	/*
	======================================================
		LuaScriptEngine::UnlinkScript

		Unlinks the provided LuaScript instance
	======================================================
	*/
	bool LuaScriptEngine::UnlinkScript(LuaScript* pScript)
	{
		LuaScript*  prev;

		// if s is at the head, simply unlink it
		if (head == pScript)
		{
			head = pScript->next;
			this->miCount--;
			return true;
		}

		// find previous link
		prev = head;
		while (prev)
		{
			if (prev->next == pScript)
			{
				prev->next = pScript->next;
				this->miCount--;
				return true;
			}

			prev = prev->next;
		}

		return false;
	}

	/*
	======================================================
		LuaScriptEngine::Update

		updates all linked lua scripts
	======================================================
	*/
	void LuaScriptEngine::Update(float dt)
	{
		LuaScript*  s;

		s = head;

		while (s)
		{
			s = s->Update(dt);
		}
	}

	/*
	======================================================
		LuaScriptEngine::getState
	======================================================
	*/
	lua_State* LuaScriptEngine::getState(void)
	{
		return this->mpLuaState;
	}

	/*
	======================================================
		LuaScriptEngine::setExtraStackSize
	======================================================
	*/
	bool LuaScriptEngine::setExtraStackSize(int n)
	{
		if(!lua_checkstack (this->mpLuaState, n))
		{
			myGlobalGame->mlog.LogError(LString("There was an error assigning extra stack space!"));
			return false;
		}

		return true;
	}

	/*
	======================================================
		class LuaScript
	======================================================
	*/
	LuaScript::LuaScript(void)
	{
		this->next = 0;
		memset(this->mFileName, 0, sizeof(char) * 1024);
		this->mpManager = 0;
	}

	LuaScript::LuaScript(char* fn)
	{
		this->next = 0;
		strcpy(this->mFileName, fn);
		this->mpManager = 0;
	}

	LuaScript::LuaScript(LuaScriptEngine* pManager)
	{
		this->next = 0;
		memset(this->mFileName, 0, sizeof(char) * 1024);
		this->mpManager = pManager;

		this->state = RX_LUA_NOT_LOADED;
		this->mfTime = 0;

		strcpy(lastErrorString, "No error.\n");

		// create a thread/state for this object
		this->mpLuaState = lua_newthread(this->mpManager->getState());
	    
		// save a pointer to the thread manager object in the global table
		// using the new thread's vm pointer as a key
		lua_pushlightuserdata(this->mpManager->getState(), this->mpLuaState);
		lua_pushlightuserdata(this->mpManager->getState(), this);
		lua_settable(this->mpManager->getState(), LUA_GLOBALSINDEX );
	}

	LuaScript::LuaScript(lua_State* pState, char* fn)
	{
		this->bFreeForm = true;
		this->next = 0;
		memset(this->mFileName, 0, sizeof(char) * 1024);
		this->mpManager = 0;

		this->state = RX_LUA_NOT_LOADED;
		this->mfTime = 0;

		strcpy(lastErrorString, "No error.\n");

		// create a thread/state for this object
		this->mpLuaState = lua_newthread(pState);
	    
		// save a pointer to the thread manager object in the global table
		// using the new thread's vm pointer as a key
		//lua_pushlightuserdata(this->mpManager->getState(), this->mpLuaState);
		//lua_pushlightuserdata(this->mpManager->getState(), this);
		lua_settable(pState, LUA_GLOBALSINDEX );

		int         status;

		//assert(manager->masterState);
		//assert(threadState);

		if(!this->mpLuaState)
		{
			myGlobalGame->mlog.LogCritical(LString("There was critical error in running a Lua script!"));
			return;
		}

		status = luaL_loadfile(this->mpLuaState, fn);

		if (status == 0)
		{
			this->ResumeScript(0.0f);
		}
		else 
		{
			this->FormatError();
			this->OutputError("Syntax Error");
		}

		//lua_close(this->mpLuaState);
	}

	LuaScript::~LuaScript(void)
	{
		if(!this->bFreeForm)
		{
			this->mpManager->UnlinkScript(this);
		}
	}

	void LuaScript::SetSourcePath(char* fn)
	{
		strcpy(this->mFileName, fn);
	}

	/*
	======================================================
		LuaScript::getState
	======================================================
	*/
	lua_State* LuaScript::getState(void)
	{
		return this->mpLuaState;
	}

	/*
	======================================================
		LuaScript::Update

		updates the script
	======================================================
	*/
	LuaScript* LuaScript::Update(float dt)
	{
		this->mfTime += dt;

		LuaScript* tempNext;

		switch(state)
		{
		case RX_LUA_WAIT_TIME:
			if (this->mfTime >= this->mfWaitTime)
			{
				this->ResumeScript(0.0f);
			}

			break;
		case RX_LUA_WAIT_FRAME:
			this->miWaitFrame--;
			if (this->miWaitFrame <= 0)
			{
				this->ResumeScript(0.0f);
			}

			break;
		case RX_LUA_NOT_LOADED:
			break;
		case RX_LUA_DONE:
			tempNext = this->next;
			//lua_close(this->mpLuaState);
			delete this;
			return tempNext;
			break;
		default :
			break;
		}

		return(next);
	}

	/*
	======================================================
		LuaScript::ResumeScript

		resumes the script
	======================================================
	*/
	void LuaScript::ResumeScript(float param)
	{
		int         status;

		// we're about to run/resume the thread, so set the global
		state  = RX_LUA_RUNNING;

		// param is treated as a return value from the function that yielded
		lua_pushnumber(this->mpLuaState, param);

		status = lua_resume(this->mpLuaState, 1);

		if (status)
		{
			FormatError();
			OutputError("Runtime Error:");
		}
	}

	/*
	======================================================
		LuaScript::LoadLuaScriptFile

		loads and runs a script
	======================================================
	*/
	void LuaScript::LoadLuaScriptFile(char* fileName, bool executeOnLoad)
	{
		int         status;

		//assert(manager->masterState);
		//assert(threadState);

		if(!this->mpManager || !this->mpLuaState)
		{
			myGlobalGame->mlog.LogCritical(LString("There was critical error in running a Lua script!"));
			return;
		}

		status = luaL_loadfile(this->mpLuaState, fileName);

		if (status == 0)
		{
			if(executeOnLoad)
			{
				this->ResumeScript(0.0f);
			}
		}
		else 
		{
			this->FormatError();
			this->OutputError("Syntax Error");
		}
	}

	/*
	======================================================
		LuaScript::RunString

		runs a single line of lua script
	======================================================
	*/
	int LuaScript::RunString(char* commandString)
	{
		int status;

		if(!this->mpManager || !this->mpLuaState)
		{
			myGlobalGame->mlog.LogCritical(LString("There was critical error in running a Lua script!"));
			return -1;
		}

		status = luaL_loadbuffer(this->mpLuaState, commandString, strlen(commandString), "Console");
		if (status == 0)
		{
			status = lua_pcall(this->mpLuaState, lua_gettop(this->mpLuaState)-1, 0, 0);
		}

		if (status)
		{
			FormatError();
			return -1;
		}

		return 0;
	}

	/*
	======================================================
		Lua Script error handling
	======================================================
	*/
	void LuaScript::FormatError(void)
	{
		const char* msg;

		msg = lua_tostring(this->mpLuaState, -1);

		if (msg == NULL)
		{
			msg = "(error with no message)";
		}

		lua_pop(this->mpLuaState, 1);

		strcpy(lastErrorString, msg);
	}

	void LuaScript::OutputError(char*   strType)
	{
		//printf("%s %s \n", strType, lastErrorString);
		myGlobalGame->mlog.LogError(LString("%s %s", strType, lastErrorString));
	}
}