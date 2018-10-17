/*
========================================================
	class BaseConsole
========================================================
*/

#include "BaseConsole.h"
#include <stdarg.h>

namespace DifferentialArts
{
	/*
	==============================================
		BaseConsole Class 
	==============================================
	*/
	BaseConsole::BaseConsole(void)
	{
		this->bConsoleFullScreen = false;
		this->bInit = false;
		bQuitBound = false;
		this->bEchoUserCommands = true;
	}

	BaseConsole::~BaseConsole(void)
	{
		this->bInit = false;
	}

	void BaseConsole::OutSuccessText(const char* txt)
	{
		log.OutLog(DEC_SUCCESS, txt);
		addString(txt, 60, 187, 149);
	}

	void BaseConsole::OutInfo(const char* txt)
	{
		log.OutLog(DEC_INFO, txt);
		addString(txt);
	}

	void BaseConsole::OutWarning(const char *strfile, const char *func, int line, const char* txt)
	{
		log.OutLog(DEC_WARNING, strfile, func, line, txt);
		addString(txt, 241, 241, 133);
	}

	void BaseConsole::OutError(const char *strfile, const char *func, int line, const char* txt)
	{
		log.OutLog(DEC_ERROR, strfile, func, line, txt);
		addString(txt, 221, 0, 0);
	}

	void BaseConsole::OutCritical(const char *strfile, const char *func, int line, const char* txt)
	{
		log.OutLog(DEC_CRITICAL, strfile, func, line, txt);
		addString(txt, 172, 7, 214);
	}

	/*
	void localQuit(std::string parameters)
	{
		myGlobalGame->getApp()->end();
	}

	void localCopyright(std::string parameters)
	{
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("Reaction Engine " CLIENT_VERSION);
		myGlobalGame->mlog.LogInfo("Copyright � Differential Arts 2007 - 2008");
		myGlobalGame->mlog.LogInfo("www.differentialarts.com");
		myGlobalGame->mlog.LogInfo("==================================================");
	}

	void localHelp(std::string parameters)
	{
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("BaseConsole help");
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("ShowVars - Shows all bound system variables");
		myGlobalGame->mlog.LogInfo("ShowCommands - Shows all bound functions variables");
		myGlobalGame->mlog.LogInfo("AboutRx - Shows rxEngine information");
		myGlobalGame->mlog.LogInfo("==================================================");
	}*/

	void BaseConsole::init(void)
	{
		this->bInsertMode = false;
		this->scroll = 0;
		this->cursorTime = 0;
		this->runTime = 0;
		this->bInit = false;
		this->bActivate = false;
		this->cursorChar = char(3);
		this->insertCursorChar = char(95);		
		this->targetY = -200;
		this->y = targetY;
		this->consoleLnPos = 0;
		//this->mTexture = 0;
		//LoadTextureJPG("Data/Textures/BaseConsole.jpg", &this->mTexture, this->mTexture, true, -1);
		this->bInit = true;
		this->bBlink = false;
		memset(currentCommand, 0, sizeof(char)*256);
		this->currentLetter = 0;
		this->command_current = 0;
		this->visibleLines = 16;

		//void (*CoreEngine::shiter) = myGlobalGame->end;
		//void* pt2Object = (void*)myGlobalGame;	

		/*
		if(!bQuitBound)
		{
			bQuitBound = true;
			
			this->bindCommand("quit", localQuit);
			this->bindCommand("help", localHelp);
			this->bindCommand("AboutRx", localCopyright);			

			this->bindBooleanVariable("bConsoleFS", &this->bConsoleFullScreen);	
			this->bindBooleanVariable("bEchoUserCommands", &this->bEchoUserCommands);			
		}
		*/
	}

	void BaseConsole::toggleActivate(void)
	{
		if(this->bInit)
		{
			//this->scroll = 0;
			this->currentLetter = 0;
			memset(currentCommand, 0, sizeof(char)*256);
			this->bActivate = !this->bActivate; 
			this->command_current = (int)this->commands.size()-1;

			/*
				if(this->bActivate)
				{
					myGlobalGame->mbEnginePaused = true;
					MasterStopAllSounds();
				} else {
					myGlobalGame->mbEnginePaused = false;
					MasterResumeAllSounds();
				}
			*/
		}
	}

	void BaseConsole::bindCommand(const char* cmdStr, void (*mVoidFNPTR)(std::string &parameters))
	{
		STRING temp;
		strcpy(temp.contents, cmdStr);

		FNPTR_HOLDER tempFNPtr;
		
		tempFNPtr.type = 0;
		tempFNPtr.mVoidFNPTR = mVoidFNPTR;

		this->storedVoidFunctionCommands.push_back(temp);
		this->functionPointers.push_back(tempFNPtr);
	}

	/*
	template<class T> void BaseConsole::bindMemberVoidCommand(const char* cmdStr, void (T::*givenFunction)())
	{
		STRING temp;
		strcpy(temp.contents, cmdStr);

		MEMBER_FNPTR_HOLDER tempFNPtr;
		
		tempFNPtr.type = 0;
		tempFNPtr.mVoidFNPTR = givenFunction;

		this->stored_member_VoidFunctionCommands.push_back(temp);
		this->_member_functionPointers.push_back(tempFNPtr);
	}*/

	void BaseConsole::bindStringVariable(const char* varName, char* strName)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);
		//strcpy(temp.bindString.contents, strName);

		temp.bindString = strName;

		temp.bp = 0;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.type = STRING_TYPE_VP;

		temp.components = 1;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindFloatVariable(const char *varName, float *Pointer1)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.fp1 = Pointer1;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.type = FLOAT_TYPE_VP;

		temp.components = 1;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindFloatVariable(const char *varName, float *Pointer1, float *Pointer2)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.fp1 = Pointer1;
		temp.fp2 = Pointer2;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.type = FLOAT_TYPE_VP;

		temp.components = 2;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindFloatVariable(const char *varName, float *Pointer1, float *Pointer2, float *Pointer3)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.fp1 = Pointer1;
		temp.fp2 = Pointer2;
		temp.fp3 = Pointer3;
		temp.fp4 = 0;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.type = FLOAT_TYPE_VP;

		temp.components = 3;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindFloatVariable(const char *varName, float *Pointer1, float *Pointer2, float *Pointer3,
									 float *Pointer4)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.fp1 = Pointer1;
		temp.fp2 = Pointer2;
		temp.fp3 = Pointer3;
		temp.fp4 = Pointer4;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.type = FLOAT_TYPE_VP;

		temp.components = 4;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindBooleanVariable(const char *varName, bool *Pointer1)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = Pointer1;

		temp.ip1 = 0;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.type = BOOL_TYPE_VP;

		temp.components = 1;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindIntegerVariable(const char *varName, int *Pointer1)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.ip1 = Pointer1;
		temp.ip2 = 0;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.type = INT_TYPE_VP;

		temp.components = 1;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindIntegerVariable(const char *varName, int *Pointer1, int *Pointer2)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.ip1 = Pointer1;
		temp.ip2 = Pointer2;
		temp.ip3 = 0;
		temp.ip4 = 0;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.type = INT_TYPE_VP;

		temp.components = 2;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindIntegerVariable(const char *varName, int *Pointer1, int *Pointer2, int *Pointer3)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.ip1 = Pointer1;
		temp.ip2 = Pointer2;
		temp.ip3 = Pointer3;
		temp.ip4 = 0;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.type = INT_TYPE_VP;

		temp.components = 3;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::bindIntegerVariable(const char *varName, int *Pointer1, int *Pointer2, int *Pointer3,
									 int *Pointer4)
	{
		_1VP temp;

		strcpy(temp.definition.contents, varName);

		temp.bindString = this->currentCommand;
		temp.bp = 0;

		temp.ip1 = Pointer1;
		temp.ip2 = Pointer2;
		temp.ip3 = Pointer3;
		temp.ip4 = Pointer4;

		temp.fp1 = 0;
		temp.fp2 = 0;
		temp.fp3 = 0;
		temp.fp4 = 0;

		temp.type = INT_TYPE_VP;

		temp.components = 4;

		this->boundVariables.push_back(temp);
	}

	void BaseConsole::executeCommand(const char* cmdStr, const char* user)
	{
		int len = (int)strlen(cmdStr);
		int i, j, count;		

		int commandLength, componentsChecked = 0;
		bool mode = false;
		char temp[128];
		float tempFloat;
		int tempInt;

		int commSize = (int)this->commands.size();		
		
		if(len >= 128)
		{
			strncpy(temp, cmdStr, 124);
			strcat(temp, (const char*)"...");
			this->LogInfo(LString("%s: %s", ((user == 0) ? "Server" : user), temp));
			this->LogError(LString("Command too big!"));
			return;
		}

		if(commSize > 0)
		{
			if(0 != stricmp(cmdStr, this->commands[commSize-1].contents))
			{
				STRING tempCommand;
				strcpy(tempCommand.contents, cmdStr);

				this->commands.push_back(tempCommand);
			}
		} else {
			STRING tempCommand;
			strcpy(tempCommand.contents, cmdStr);

			this->commands.push_back(tempCommand);
		}

		if(0 == stricmp("ShowBoundCommands", cmdStr) || 0 == stricmp("ShowCommands", cmdStr) || 0 == stricmp("Commands", cmdStr))
		{
			this->LogInfo(LString("Showing bound commands..."));
			for(this->storedVoidFunctionCommands_iterator = this->storedVoidFunctionCommands.begin();
				this->storedVoidFunctionCommands_iterator != this->storedVoidFunctionCommands.end();
				this->storedVoidFunctionCommands_iterator++)
			{
					/*
					int ret = strcmp(this->storedVoidFunctionCommands[i].contents, cmdStr);
					this->LogInfo(LString("cmdStr: %s, checked with strcmp: %s, strcmp returned: %d", cmdStr, this->storedVoidFunctionCommands[i].contents, ret));
					*/

					this->LogInfo(LString(" %s", (*storedVoidFunctionCommands_iterator).contents));
			}
			return;
		}

		if(0 == stricmp("ShowBoundVars", cmdStr) || 0 == stricmp("ShowVars", cmdStr))
		{
			this->LogInfo(LString("Showing bound variables..."));
			for(this->boundVariables_iterator = this->boundVariables.begin();
				this->boundVariables_iterator != this->boundVariables.end();
				this->boundVariables_iterator++)
			{
					/*
					int ret = strcmp(this->storedVoidFunctionCommands[i].contents, cmdStr);
					this->LogInfo(LString("cmdStr: %s, checked with strcmp: %s, strcmp returned: %d", cmdStr, this->storedVoidFunctionCommands[i].contents, ret));
					*/

					if((*boundVariables_iterator).type == BOOL_TYPE_VP)
					{
						sprintf(temp, "BOOLEAN");
					} else if((*boundVariables_iterator).type == FLOAT_TYPE_VP)
					{
						sprintf(temp, "FLOATING POINT");
					} else if((*boundVariables_iterator).type == INT_TYPE_VP)
					{
						sprintf(temp, "INTEGER");
					}

					this->LogInfo(LString(" %s Components: %d %s", (*boundVariables_iterator).definition.contents,
											   (*boundVariables_iterator).components,
											   temp));
			}
			return;
		}

		for(this->boundVariables_iterator = this->boundVariables.begin();
			this->boundVariables_iterator != this->boundVariables.end();
			this->boundVariables_iterator++)
		{
			commandLength = (int)strlen((*this->boundVariables_iterator).definition.contents);

			/* 
				STRING TYPE CODE STARTS HERE
			*/

			if((*this->boundVariables_iterator).type == STRING_TYPE_VP)
			{
				if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
					strlen((*this->boundVariables_iterator).definition.contents)))
				{
					for(i = commandLength; i<len;i++)
					{
						if(cmdStr[i] == ' ')
						{
							count = 0;
							for(j = i; j<len;j++)
							{
								if(mode == false)
								{
									if(cmdStr[j] != ' ')
									{
										temp[count++] = cmdStr[j];
									} else {
										componentsChecked = 1;
									}
								}							
							}
						}
					}
					
					if(componentsChecked == 1)
					{
						memset((*this->boundVariables_iterator).bindString, 0, sizeof(char)*256);
						strncpy((*this->boundVariables_iterator).bindString, temp, count);
					}

					if(!user)
					{
						this->LogInfo(LString("%s is \"%s\"", 
												((*this->boundVariables_iterator).definition.contents),
												(*this->boundVariables_iterator).bindString));
					} else {
						this->LogInfo(LString("%s -> %s is \"%s\"", user,
												((*this->boundVariables_iterator).definition.contents),
												(*this->boundVariables_iterator).bindString));
					}
					
					return;
				}
			}
			/* 
				BOOLEAN TYPE CODE STARTS HERE
			*/
			else if((*this->boundVariables_iterator).type == BOOL_TYPE_VP)
			{
				if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
					strlen((*this->boundVariables_iterator).definition.contents)))
				{
					for(i = commandLength; i<len;i++)
					{
						if(cmdStr[i] == ' ')
						{
							count = 0;
							for(j = i; j<len;j++)
							{
								if(mode == false)
								{
									if(cmdStr[j] != ' ')
									{
										mode = true;
									}
								}
								
								if(mode == true)
								{
									if(cmdStr[j] == ' ')
									{
										count = 0;
										mode = false;
										break;
									} else {
										temp[count++] = cmdStr[j];
										if(componentsChecked == 0)
										{
											componentsChecked = 1;
											break;
										}
									}
								}
							}
						}
					}

					if(componentsChecked == 1)
					{
						if(strstr(temp, "true"))
						{
							*(*this->boundVariables_iterator).bp = true;
						} else if(strstr(temp, "false"))
						{
							*(*this->boundVariables_iterator).bp = false;
						} else {
							if(temp[0] == '0')
							{
								*(*this->boundVariables_iterator).bp = false;
							} else if(temp[0] == '1')
							{
								*(*this->boundVariables_iterator).bp = true;
							} else {
								this->LogError(LString("Invalid input"));
							}
						}
					}

					if(!user)
					{
						this->LogInfo(LString("%s is %d", 
												((*this->boundVariables_iterator).definition.contents),
												*(*this->boundVariables_iterator).bp));
					} else {
						this->LogInfo(LString("%s -> %s is %d", user,
												((*this->boundVariables_iterator).definition.contents),
												*(*this->boundVariables_iterator).bp));
					}
					
					return;
				}
			}
			/* 
				FLOAT TYPE CODE STARTS HERE
			*/

			else if((*this->boundVariables_iterator).type == FLOAT_TYPE_VP)
			{
				if((*this->boundVariables_iterator).components == 1)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									 strlen((*this->boundVariables_iterator).definition.contents)))
					{
						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;
											break;
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %f", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1));
						} else {
							this->LogInfo(LString("%s -> %s is %f", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1));
						}					
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 2)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %f, %f", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2));
						} else {
							this->LogInfo(LString("%s -> %s is %f, %f", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2));
						}						
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 3)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp3 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						} else if(componentsChecked == 2)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp2 = tempFloat;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %f, %f, %f", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2,
													*(*this->boundVariables_iterator).fp3));
						} else {
							this->LogInfo(LString("%s -> %s is %f, %f, %f", user, 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2,
													*(*this->boundVariables_iterator).fp3));
						}						
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 4)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp3 = tempFloat;
												componentsChecked = 4;
												continue;
											} else if(componentsChecked == 4)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp4 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						} else if(componentsChecked == 2)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp2 = tempFloat;
						} else if(componentsChecked == 3)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp3 = tempFloat;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %f, %f, %f, %f", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2,
													*(*this->boundVariables_iterator).fp3,
													*(*this->boundVariables_iterator).fp4));
						} else {
							this->LogInfo(LString("%s -> %s is %f, %f, %f, %f", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).fp1,
													*(*this->boundVariables_iterator).fp2,
													*(*this->boundVariables_iterator).fp3,
													*(*this->boundVariables_iterator).fp4));
						}						
						
						return;
					}
				}
			}

			/* 
				INTEGER TYPE CODE STARTS HERE
			*/

			else if((*this->boundVariables_iterator).type == INT_TYPE_VP)
			{
				if((*this->boundVariables_iterator).components == 1)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;
											break;
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %d", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1));
						} else {
							this->LogInfo(LString("%s -> %s is %d", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1));
						}					
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 2)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %d, %d", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2));
						} else {
							this->LogInfo(LString("%s -> %s is %d, %d", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2));
						}						
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 3)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip3 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						} else if(componentsChecked == 2)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip2 = tempInt;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %d, %d, %d", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2,
													*(*this->boundVariables_iterator).ip3));
						} else {
							this->LogInfo(LString("%s -> %s is %d, %d, %d", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2,
													*(*this->boundVariables_iterator).ip3));
						}						
						
						return;
					}
				} else if((*this->boundVariables_iterator).components == 4)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip3 = tempInt;
												componentsChecked = 4;
												continue;
											} else if(componentsChecked == 4)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip4 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						} else if(componentsChecked == 2)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip2 = tempInt;
						} else if(componentsChecked == 3)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip3 = tempInt;
						}

						if(!user)
						{
							this->LogInfo(LString("%s is %d, %d, %d, %d", 
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2,
													*(*this->boundVariables_iterator).ip3,
													*(*this->boundVariables_iterator).ip4));
						} else {
							this->LogInfo(LString("%s -> %s is %d, %d, %d, %d", user,
													((*this->boundVariables_iterator).definition.contents),
													*(*this->boundVariables_iterator).ip1,
													*(*this->boundVariables_iterator).ip2,
													*(*this->boundVariables_iterator).ip3,
													*(*this->boundVariables_iterator).ip4));
						}							
						
						return;
					}
				}
			}
		}	

		for(this->storedVoidFunctionCommands_iterator = this->storedVoidFunctionCommands.begin(),
			this->functionPointers_iterator = this->functionPointers.begin();
			this->storedVoidFunctionCommands_iterator != this->storedVoidFunctionCommands.end();
			this->storedVoidFunctionCommands_iterator++, this->functionPointers_iterator++)
		{
			int indexStart = (int)strlen((*this->storedVoidFunctionCommands_iterator).contents);

			if(cmdStr[indexStart] == ' ' || cmdStr[indexStart] == '\0')
			{
				if(0 == strnicmp(cmdStr,
								 (*this->storedVoidFunctionCommands_iterator).contents,
								 indexStart))
				{
					
					int cmdLength = (int)strlen(cmdStr);
					std::string temp = cmdStr;
					
					while(cmdStr[indexStart] == ' ')
					{
						indexStart++;
					}

					temp = temp.substr(indexStart, cmdLength);

					//this->LogInfo(LString("Server issued command %s throught console", cmdStr));
					if(this->bEchoUserCommands)
					{
						this->LogInfo(LString("%s issued command %s throught console", ((user == 0) ? "Server" : user), cmdStr));
						
					} else {
						if(!user)
						{
							this->LogInfo(LString("Server issued command %s throught console", cmdStr));
						}
					}

					(*(*this->functionPointers_iterator).mVoidFNPTR)(temp);

					return;
				}
			}
		}

		this->LogInfo(LString("%s: %s", ((user == 0) ? "Server" : user), cmdStr));
	}

	std::string BaseConsole::getVariable(const char* cmdStr)
	{
		int len = (int)strlen(cmdStr);
		int i, j, count;		

		int commandLength, componentsChecked = 0;
		bool mode = false;
		char temp[64];
		float tempFloat;
		int tempInt;
		
		if(len >= 64)
		{
			/*
			strncpy(temp, cmdStr, 60);
			strcat(temp, (const char*)"...");
			this->LogInfo(LString("%s: %s", ((user == 0) ? "Server" : user), temp));
			this->LogError(LString("Command too big!"));
			*/

			std::string output = cmdStr;
			output += " is bad variable name";
			return output;
		}

		for(this->boundVariables_iterator = this->boundVariables.begin();
			this->boundVariables_iterator != this->boundVariables.end();
			this->boundVariables_iterator++)
		{
			commandLength = (int)strlen((*this->boundVariables_iterator).definition.contents);

			/* 
				STRING TYPE CODE STARTS HERE
			*/

			if((*this->boundVariables_iterator).type == STRING_TYPE_VP)
			{
				if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
					strlen((*this->boundVariables_iterator).definition.contents)))
				{
					for(i = commandLength; i<len;i++)
					{
						if(cmdStr[i] == ' ')
						{
							count = 0;
							for(j = i; j<len;j++)
							{
								if(mode == false)
								{
									if(cmdStr[j] != ' ')
									{
										temp[count++] = cmdStr[j];
									} else {
										componentsChecked = 1;
									}
								}							
							}
						}
					}
					
					if(componentsChecked == 1)
					{
						memset((*this->boundVariables_iterator).bindString, 0, sizeof(char)*256);
						strncpy((*this->boundVariables_iterator).bindString, temp, count);
					}

					std::string output = (*this->boundVariables_iterator).bindString;
					
					return output;
				}
			}
			/* 
				BOOLEAN TYPE CODE STARTS HERE
			*/
			else if((*this->boundVariables_iterator).type == BOOL_TYPE_VP)
			{
				if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
					strlen((*this->boundVariables_iterator).definition.contents)))
				{
					for(i = commandLength; i<len;i++)
					{
						if(cmdStr[i] == ' ')
						{
							count = 0;
							for(j = i; j<len;j++)
							{
								if(mode == false)
								{
									if(cmdStr[j] != ' ')
									{
										mode = true;
									}
								}
								
								if(mode == true)
								{
									if(cmdStr[j] == ' ')
									{
										count = 0;
										mode = false;
										break;
									} else {
										temp[count++] = cmdStr[j];
										if(componentsChecked == 0)
										{
											componentsChecked = 1;
											break;
										}
									}
								}
							}
						}
					}

					if(componentsChecked == 1)
					{
						if(strstr(temp, "true"))
						{
							*(*this->boundVariables_iterator).bp = true;
						} else if(strstr(temp, "false"))
						{
							*(*this->boundVariables_iterator).bp = false;
						} else {
							if(temp[0] == '0')
							{
								*(*this->boundVariables_iterator).bp = false;
							} else if(temp[0] == '1')
							{
								*(*this->boundVariables_iterator).bp = true;
							} else {
								this->LogError(LString("Invalid input"));
							}
						}
					}

					std::string output = ((*(*this->boundVariables_iterator).bp) ? "true" : "false");
					
					return output;
				}
			}
			/* 
				FLOAT TYPE CODE STARTS HERE
			*/

			else if((*this->boundVariables_iterator).type == FLOAT_TYPE_VP)
			{
				if((*this->boundVariables_iterator).components == 1)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									 strlen((*this->boundVariables_iterator).definition.contents)))
					{
						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;
											break;
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						}

						std::ostringstream buffer;
						buffer << *(*this->boundVariables_iterator).fp1;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 2)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						}

						std::ostringstream buffer;

						buffer << *(*this->boundVariables_iterator).fp1<<" ";
						buffer << *(*this->boundVariables_iterator).fp2;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 3)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp3 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						} else if(componentsChecked == 2)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp2 = tempFloat;
						}

						std::ostringstream buffer;

						buffer << *(*this->boundVariables_iterator).fp1<<" ";
						buffer << *(*this->boundVariables_iterator).fp2<<" ";
						buffer << *(*this->boundVariables_iterator).fp3;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 4)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp1 = tempFloat;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp2 = tempFloat;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp3 = tempFloat;
												componentsChecked = 4;
												continue;
											} else if(componentsChecked == 4)
											{
												tempFloat = float(atof(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).fp4 = tempFloat;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp1 = tempFloat;
						} else if(componentsChecked == 2)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp2 = tempFloat;
						} else if(componentsChecked == 3)
						{
							tempFloat = float(atof(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).fp3 = tempFloat;
						}

						std::ostringstream buffer;

						buffer << *(*this->boundVariables_iterator).fp1<<" ";
						buffer << *(*this->boundVariables_iterator).fp2<<" ";
						buffer << *(*this->boundVariables_iterator).fp3<<" ";
						buffer << *(*this->boundVariables_iterator).fp4;
					
						return buffer.str();
					}
				}
			}

			/* 
				INTEGER TYPE CODE STARTS HERE
			*/

			else if((*this->boundVariables_iterator).type == INT_TYPE_VP)
			{
				if((*this->boundVariables_iterator).components == 1)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;
											break;
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						}

						std::ostringstream buffer;

						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip1;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 2)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						}

						std::ostringstream buffer;

						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip1<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip2;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 3)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip3 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						} else if(componentsChecked == 2)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip2 = tempInt;
						}

						std::ostringstream buffer;

						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip1<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip2<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip3;
					
						return buffer.str();
					}
				} else if((*this->boundVariables_iterator).components == 4)
				{
					if(0 == strnicmp(cmdStr, (*this->boundVariables_iterator).definition.contents,
									strlen((*this->boundVariables_iterator).definition.contents)))
					{
						count = 0;
						mode = false;

						for(i = commandLength; i<len;i++)
						{
							if(cmdStr[i] == ' ')
							{
								count = 0;
								for(j = i; j<len;j++)
								{
									if(mode == false)
									{
										if(cmdStr[j] != ' ')
										{
											mode = true;
										}
									}
									
									if(mode == true)
									{
										if(cmdStr[j] == ' ')
										{
											count = 0;
											mode = false;

											if(componentsChecked == 1)
											{						
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip1 = tempInt;
												componentsChecked = 2;
												continue;
											} else if(componentsChecked == 2)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip2 = tempInt;
												componentsChecked = 3;
												continue;
											} else if(componentsChecked == 3)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip3 = tempInt;
												componentsChecked = 4;
												continue;
											} else if(componentsChecked == 4)
											{
												tempInt = int(atoi(temp));
												memset(temp, 0, sizeof(char)*64);
												*(*this->boundVariables_iterator).ip4 = tempInt;
												break;
											} else {
												break;
											}
										} else {
											temp[count++] = cmdStr[j];
											if(componentsChecked == 0)
											{										
												componentsChecked = 1;
											}
										}
									}
								}
							}
						}

						if(componentsChecked == 1)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip1 = tempInt;
						} else if(componentsChecked == 2)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip2 = tempInt;
						} else if(componentsChecked == 3)
						{
							tempInt = int(atoi(temp));
							memset(temp, 0, sizeof(char)*64);
							*(*this->boundVariables_iterator).ip3 = tempInt;
						}

						std::ostringstream buffer;

						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip1<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip2<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip3<<" ";
						buffer << (std::ostringstream)*(*this->boundVariables_iterator).ip4;
					
						return buffer.str();
					}
				}
			}
		}

		return std::string("undefined");
	}	

	bool BaseConsole::isActivated(void)
	{
		return this->bActivate;
	}

	bool BaseConsole::isInitialized(void)
	{
		return this->bInit;
	}

	void BaseConsole::pAddString(const char* txt, ...)
	{
		va_list argList;
		va_start (argList, txt);

		STRING temp;
		vsprintf(temp.contents, txt, argList);

		this->strings.push_back(temp);	

		va_end (argList);
	}

	void BaseConsole::addString(const char* txt, unsigned char r, unsigned char g, unsigned char b)
	{
		STRING temp;
		unsigned int len = (unsigned int)strlen(txt);
		const unsigned int size = 111;
		memset(temp.contents, 0, sizeof(char)*256);

		temp.r = r;
		temp.g = g;
		temp.b = b;

		char cTemp[size];

		for(unsigned int i = 0; i < len; i += size) //For wordwrap
		{
			for(unsigned int j = 0; j < size; j++)
			{
				cTemp[j] = txt[i + j];
			}

			strncpy(temp.contents, cTemp, size);

			this->strings.push_back(temp);
		}

		/*
		if(len >= 512)
		{		
			strncpy(temp.contents, txt, 471);
			strcat(temp.contents, "...<truncated due to buffer overflow>");

			this->strings.push_back(temp);	
		} else {
			if(len >= 512)
			{

			strcpy(temp.contents, txt);

			this->strings.push_back(temp);	
		}
		*/
	}

	RX_UTIL_API BaseConsole* gConsole;
}