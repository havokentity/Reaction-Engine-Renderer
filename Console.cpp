/*
========================================================
	class Console
========================================================
*/

#include "Console.h"
#include "CoreEngine.h"
#include "font.h"

namespace DifferentialArts
{
	/*
	==============================================
		Console Class 
	==============================================
	*/
	Console::Console(void)
	{
		this->bConsoleFullScreen = false;
		this->bInit = false;
		bQuitBound = false;
		this->bEchoUserCommands = true;
	}

	Console::~Console(void)
	{
		this->bInit = false;
	}

	void localQuit(std::string &parameters)
	{
		myGlobalGame->getApp()->end();
	}

	void localCopyright(std::string &parameters)
	{
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("Reaction Engine " CLIENT_VERSION);
		myGlobalGame->mlog.LogInfo("Copyright � Differential Arts 2007 - 2008");
		myGlobalGame->mlog.LogInfo("www.differentialarts.com");
		myGlobalGame->mlog.LogInfo("==================================================");
	}

	void localHelp(std::string &parameters)
	{
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("Console help");
		myGlobalGame->mlog.LogInfo("==================================================");
		myGlobalGame->mlog.LogInfo("ShowVars - Shows all bound system variables");
		myGlobalGame->mlog.LogInfo("ShowCommands - Shows all bound functions variables");
		myGlobalGame->mlog.LogInfo("AboutRx - Shows rxEngine information");
		myGlobalGame->mlog.LogInfo("==================================================");
	}

	void Console::init(void)
	{
		BaseConsole::init();
		this->mTexture = 0;
		unsigned int w, h;
		LoadTexture("Data/Textures/Console.jpg", &this->mTexture, this->mTexture, true, w, h, true, true);

		if(!bQuitBound)
		{
			bQuitBound = true;

			this->bindCommand("quit", localQuit);
			this->bindCommand("help", localHelp);
			this->bindCommand("AboutRx", localCopyright);

			this->bindBooleanVariable("bConsoleFS", &this->bConsoleFullScreen);	
			this->bindBooleanVariable("bEchoUserCommands", &this->bEchoUserCommands);				
		}
	}

	void Console::operate(float dt)
	{
		if(!this->bInit)
		{
			return;
		}	

		float acc;
		if(this->bActivate)
		{
			char tChar;
			bool shift = false;			

			if(myGlobalGame->getApp()->keyReleased(RX_RETURN))
			{				
				this->currentLetter = 0;
				this->executeCommand(currentCommand);
				this->command_current = this->commands.size();
				memset(currentCommand, 0, sizeof(char)*256);				
			} else if(myGlobalGame->getApp()->keyReleased(RX_INSERT))
			{
				this->bInsertMode = !bInsertMode;
				this->command_current = this->commands.size();
			} else if(myGlobalGame->getApp()->keyReleased(RX_BACKSPACE)) //For Backspace
			{
				this->currentLetter = (this->currentLetter<=0)?0:--this->currentLetter;
				if(!currentCommand[this->currentLetter + 1])
				{
					currentCommand[this->currentLetter] = 0;
				} else {				
					int len = strlen(currentCommand);
					for(int i = (this->currentLetter+1);i < len; i++)
					{
						currentCommand[i-1] = currentCommand[i];
					}

					currentCommand[len-1] = 0;
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_LEFT))
			{
				this->currentLetter = (this->currentLetter<=0)?0:--this->currentLetter;
			} else if(myGlobalGame->getApp()->keyReleased(RX_RIGHT))
			{
				if(!currentCommand[this->currentLetter])
				{
				} else {
					this->currentLetter++;	
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_DELETE))
			{
				unsigned int len = strlen(currentCommand);
				if(this->currentLetter >= len)
				{
				} else {
					for(unsigned int i = (this->currentLetter);i < len-1; i++)
					{
						currentCommand[i] = currentCommand[i+1];
					}
					
					currentCommand[len-1] = 0;
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_PRIOR))
			{
				int size = strings.size();

				if(size > (this->visibleLines + 5))
				{
					scroll += 5;

					if(scroll >= size - this->visibleLines)
					{
						scroll = size - this->visibleLines;
					}				
				}		

				if(myGlobalGame->getApp()->keyPressed(RX_LSHIFT) ||
						   myGlobalGame->getApp()->keyPressed(RX_RSHIFT))
				{
					scroll = size - this->visibleLines;
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_NEXT))
			{
				if(myGlobalGame->getApp()->keyPressed(RX_LSHIFT) ||
				   myGlobalGame->getApp()->keyPressed(RX_RSHIFT))
				{
					scroll = 0;
				}

				if(scroll <= 0)
				{
				} else {
					if(scroll < 5)
					{
						scroll -= 1;
					} else {
						scroll -= 5;
					}
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_ESCAPE))
			{
				this->toggleActivate();
			} else if(myGlobalGame->getApp()->keyReleased(RX_UP))
			{
				this->command_current--;

				if(this->command_current < 0)
				{
					this->command_current = 0;
				}

				if(!this->commands.empty())
				{
					currentLetter = 0;
					memset(this->currentCommand, 0, sizeof(char)*256);
					for(unsigned int i = 0; i<strlen(this->commands[this->command_current].contents); i++)
					{
						if(this->commands[this->command_current].contents[i] == '\0')
						{
							break;
						}
						this->currentCommand[i] = this->commands[this->command_current].contents[i];
						currentLetter++;
					}					
				}	
			} else if(myGlobalGame->getApp()->keyReleased(RX_DOWN))
			{		
				this->command_current++;

				if(this->command_current >= (int)this->commands.size())
				{
					this->command_current = this->commands.size() - 1;
				}

				if(!this->commands.empty())
				{
					currentLetter = 0;
					memset(this->currentCommand, 0, sizeof(char)*256);
					for(unsigned int i = 0; i<strlen(this->commands[this->command_current].contents); i++)
					{
						if(this->commands[this->command_current].contents[i] == '\0')
						{
							break;
						}
						this->currentCommand[i] = this->commands[this->command_current].contents[i];
						currentLetter++;
					}					
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_HOME))
			{			
				currentLetter = 0;
			} else if(myGlobalGame->getApp()->keyReleased(RX_END))
			{			
				currentLetter = strlen(currentCommand);
			}/* else if(myGlobalGame->getApp()->keyReleased(RX_CAPS))
			{			
			} else if(myGlobalGame->getApp()->keyReleased(RX_SPACE))
			{		
				this->currentCommand[currentLetter++] = ' ';
			} else if(myGlobalGame->getApp()->keyReleased(RX_QUESTION))
			{		
				this->currentCommand[currentLetter++] = '/';
			} else if(myGlobalGame->getApp()->keyReleased(RX_BACKSLASH))
			{		
				this->currentCommand[currentLetter++] = '\\';
			} else if(myGlobalGame->getApp()->keyReleased(RX_OPEN_BRACE))
			{		
				this->currentCommand[currentLetter++] = '[';
			} else if(myGlobalGame->getApp()->keyReleased(RX_CLOSE_BRACE))
			{		
				this->currentCommand[currentLetter++] = ']';
			} else if(myGlobalGame->getApp()->keyReleased(RX_MINUS))
			{		
				if(myGlobalGame->getApp()->keyPressed(RX_SHIFT))
				{
					this->currentCommand[currentLetter++] = '_';
				} else {
					this->currentCommand[currentLetter++] = '-';
				}
			} else if(myGlobalGame->getApp()->keyReleased(RX_PERIOD))
			{
				this->currentCommand[currentLetter++] = '.';
			} else if(myGlobalGame->getApp()->keyReleased(RX_COLON))
			{
				this->currentCommand[currentLetter++] = ':';
			}*/ else {
				tChar = myGlobalGame->getApp()->getLastKeyReleased();

				if(currentLetter < 256)
				{
					if(this->bInsertMode)
					{
						if(tChar)
						{
							char temp[256];

							strcpy(temp, currentCommand);

							for(int i = currentLetter; i < 255; i++)
							{
								this->currentCommand[i + 1] = temp[i];
							}						

							this->currentCommand[currentLetter++] = tChar;
						}
					} else {
						if(tChar)
						{
							this->currentCommand[currentLetter++] = tChar;
						}
					}
				}

				/*
				if(tUChar == RX_SHIFT)
				{
				} else {
					tChar = (char)tUChar;
					if(myGlobalGame->getApp()->keyPressed(RX_SHIFT))
					{					
						if(tChar)
						{						
							this->currentCommand[currentLetter++] = tChar;				
						}
					} else {
						if(tChar)
						{		
							if(tChar == '1' || tChar == '2' || tChar == '3' ||
							   tChar == '4' || tChar == '5' || tChar == '6' ||
							   tChar == '7' || tChar == '8' || tChar == '9' ||
							   tChar == '0')
							{
								this->currentCommand[currentLetter++] = tChar;
							} else {
								this->currentCommand[currentLetter++] = tChar + 32;	//swapped for due to incorrect sync of I/P data
																				//Something to do with when shift key is being pressed
							}
						}
					}
				}*/
			}		

			if((this->runTime - this->cursorTime) >= 0.4f)
			{
				this->bBlink = !this->bBlink;
				this->cursorTime = this->runTime;
			} else {
				this->runTime += dt;
			}

			if(this->y == 0)
			{
				return;
			}

			acc = -this->y;
			this->y += acc * 4.0f * dt;

			if(Math::abs(acc) <= 0.01f)
			{
				this->y = 0;
			}
		} else {
			if(this->y == this->targetY)
			{
				return;
			}

			acc = (this->targetY-this->y);
			this->y += acc * 4.0f * dt;	
			
			if(Math::abs(acc) <= 0.01f)
			{
				this->y = this->targetY;
			}		
		}
	}

	void Console::draw(void)
	{	
		if(this->y > this->targetY && this->bInit)
		{		
			glDisable(GL_DEPTH_TEST);
			myGlobalGame->SaveAndSetup2DScreen(800, 600);
			glBindTexture(GL_TEXTURE_2D, this->mTexture);

			this->visibleLines = 16;
			int textYOrigin = 283;
			this->targetY = -200;
			int inputLineOffset = 300;

			if(bConsoleFullScreen)
			{
				this->targetY = -600;
				textYOrigin = 893;
				this->visibleLines = 52;
				inputLineOffset = 910;

				glBegin(GL_QUADS);
					glTexCoord2f(0, 1.0f); glVertex2i(0, 600 + (int)y);
					glTexCoord2f(1.0f, 1.0f); glVertex2i(800, 600 + (int)y);
					glTexCoord2f(1.0f, 0); glVertex2i(800, (int)y);
					glTexCoord2f(0, 0); glVertex2i(0, (int)y);
				glEnd();
			} else {
				glBegin(GL_QUADS);
					glTexCoord2f(0, 1.0f); glVertex2i(0, 200 + (int)y);
					glTexCoord2f(1.0f, 1.0f); glVertex2i(800, 200 + (int)y);
					glTexCoord2f(1.0f, 0); glVertex2i(800, (int)y);
					glTexCoord2f(0, 0); glVertex2i(0, (int)y);
				glEnd();
			}
			
			glBindTexture(GL_TEXTURE_2D,g_Textures[FONT_TEXTURE]);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glPushMatrix();
			glScalef(0.65f, 0.65f, 1);

			int count = 0;
			int size = this->strings.size()  - scroll;
			for(int i = size - 1; i>= size - visibleLines ; i--)
			{
				glColor4ub(this->strings[i].r, this->strings[i].g, this->strings[i].b, 255);
				sPrint( 0, textYOrigin + (int)(this->y*1.5384f) - count * 17, 1, this->strings[i].contents);
				count++;
			}

			glColor4f(1, 1, 1, 1);
			float ty = this->y*1.5384f + inputLineOffset;
			sPrint( 0, (int)ty, 1, "%s", this->currentCommand);
			
			if(this->bBlink)
			{			
				if(this->bInsertMode)
				{
					sPrint( this->currentLetter*11, (int)ty, 1, "%c", this->insertCursorChar);
				} else {
					sPrint( this->currentLetter*11, (int)ty, 1, "%c", this->cursorChar);
				}
			}

			glPopMatrix();
			myGlobalGame->RestoreOldScreen();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		} 
	}
}