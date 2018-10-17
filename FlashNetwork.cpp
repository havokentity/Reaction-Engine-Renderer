/*	
============================================================================================
	Classes contained are
		class SimpleNetworkServer

	This class is used for simple network communication.
============================================================================================
*/

#include "FlashNetwork.h"
#include "CoreEngine.h"

namespace DifferentialArts
{
	void PrintConsoleServerInfo(std::string &parameters)
	{
		myGlobalGame->mlog.LogInfo(LString("=================================="));
		myGlobalGame->mlog.LogInfo(LString("DifferentialArts Raw Text Server"));
		myGlobalGame->mlog.LogInfo(LString("=================================="));
		myGlobalGame->mlog.LogInfo(LString("Clients: %d/%d", SimpleNetworkServer::getSingleton()->currentClient,
										   SimpleNetworkServer::getSingleton()->maxClients));

		char ac[128];

		if(gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Error:  %d when getting local host!", WSAGetLastError()));
			return;
		}

		myGlobalGame->mlog.LogSuccessText(LString("Server started: %s", ac));

		struct hostent* h;

		h = gethostbyname(ac); 

		myGlobalGame->mlog.LogInfo(LString("Host name  : %s", h->h_name));

		int count = 0;

		while(h->h_addr_list[count])
		{
			if(!h->h_addr_list[count])
			{
				break;
			}

			myGlobalGame->mlog.LogInfo(LString("IP Address(%d): %s", count,
									   inet_ntoa(*((struct in_addr *)h->h_addr_list[count]))));

			count++;
		}		

		myGlobalGame->mlog.LogInfo(LString("=================================="));
	}

	void PrintClients(std::string &parameters)
	{
		myGlobalGame->mlog.LogInfo(LString("=================================="));
		myGlobalGame->mlog.LogInfo(LString("Client Information"));
		myGlobalGame->mlog.LogInfo(LString("=================================="));
		for(int i = 0; i < SimpleNetworkServer::getSingleton()->currentClient; i++)
		{
			if(SimpleNetworkServer::getSingleton()->clientData[i].pwdVerified)
			{
				sockaddr_in name;
				int namelen = sizeof(name);
				getpeername(SimpleNetworkServer::getSingleton()->clientData[i].socket, (SOCKADDR*) &name, &namelen);				

				myGlobalGame->mlog.LogInfo(LString("(%d) %s, %s:%d %d", (i + 1),
												   SimpleNetworkServer::getSingleton()->clientData[i].username,
												   inet_ntoa(name.sin_addr),
												   name.sin_port,
												   SimpleNetworkServer::getSingleton()->clientData[i].socket));
			}
		}

		myGlobalGame->mlog.LogInfo(LString("=================================="));
	}

	std::string welcomeMessage =  "Welcome to DA Engine server\0";
	std::string sorryPasswordMessage =  "Password incorrect!\0";
	std::string sorryNoUserNameMessage =  "Provide a username to join the server\0";
	std::string userNameAlreadyExistsMessage =  "Username already exists on server!\0";
	std::string denialMessage =  "Server full! Sorry you can't join!\0";

	void ClearFile(std::string &parameters);

	RX_API void acceptingThreadProcedure(int *serverSocket)
	{
		int mySocket = *serverSocket;

		if(SimpleNetworkServer::getSingleton()->currentClient < 0)
		{
			SimpleNetworkServer::getSingleton()->currentClient = 0;
		}

		for(;;)
		{
			unsigned int clientSocket = accept(mySocket,
											   0,
											   0);

			if(clientSocket == SOCKET_ERROR)
			{
				myGlobalGame->mlog.LogError(LString("Accept failed!"));
				SimpleNetworkServer::getSingleton()->bStatus = true;
				return;
			} else {
				WaitForSingleObject(SimpleNetworkServer::getSingleton()->mutexHandle, INFINITE);
				FD_SET(clientSocket, &SimpleNetworkServer::getSingleton()->masterSet);
				ReleaseMutex(SimpleNetworkServer::getSingleton()->mutexHandle);				

				SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].socket = clientSocket;
				SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = false;

				if(SimpleNetworkServer::getSingleton()->currentClient >= SimpleNetworkServer::getSingleton()->maxClients)
				{
					char buffer[MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER];
					int messageSize;			

					messageSize = SimpleNetworkServer::getSingleton()->ReceiveData(
													   clientSocket,
													   buffer,
													   MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER);

					SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = false;

					myGlobalGame->mlog.LogWarning(LString("Client on %d was not allowed to join because server is full!", clientSocket));					

					if(send(clientSocket, denialMessage.c_str(), denialMessage.length() + 1, 0) == SOCKET_ERROR)
					{
						myGlobalGame->mlog.LogError(LString("Failed sending server full message to client!"));
					}	

					SimpleNetworkServer::getSingleton()->bCleanupUnwantedClients = true;
				} else {
					char buffer[MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER];
					memset(buffer, 0, sizeof(char) * MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER);
					int messageSize;			

					messageSize = SimpleNetworkServer::getSingleton()->ReceiveData(
													   clientSocket,
													   buffer,
													   MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER);

					SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = false;

					int indexForUsername = 0;
					for(int k = 0; k < 256; k++)
					{
						if(buffer[k] == '|')
						{
							indexForUsername = k;
							break;
						}
					}
					char temp[256];

					memset(temp, 0, sizeof(char) * 256);

					if(indexForUsername > 0)
					{
						strncpy(temp, buffer, indexForUsername);
					}					

					std::string tempUserName = buffer;	

					if(tempUserName.length() > 255 || tempUserName == "" || tempUserName =="\0" || tempUserName == "|")
					{
						myGlobalGame->mlog.LogWarning(LString("Client on %d was not allowed to join client provided no username or user name data was corrupt!", clientSocket));

						if(send(clientSocket, sorryNoUserNameMessage.c_str(), sorryNoUserNameMessage.length() + 1, 0) == SOCKET_ERROR)
						{
							myGlobalGame->mlog.LogError(LString("Failed sending no-username message to client!"));
						}

						SimpleNetworkServer::getSingleton()->bCleanupUnwantedClients = true;
					} else {
						tempUserName = tempUserName.substr(indexForUsername + 1);
						strncpy(SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].username,
								tempUserName.c_str(), 256);

						if(0 == strcmp(SimpleNetworkServer::getSingleton()->password, temp))
						{
							SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = true;							

							for(int i = 0; i < SimpleNetworkServer::getSingleton()->currentClient; i++)
							{
								if(0 == strcmp(SimpleNetworkServer::getSingleton()->clientData[i].username,
											   SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].username))
								{
									SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = false;

									myGlobalGame->mlog.LogWarning(LString("Client on %d was not allowed to join because %s username already exists on server!",																  
																  clientSocket,
																  SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].username));

									if(send(clientSocket, userNameAlreadyExistsMessage.c_str(), userNameAlreadyExistsMessage.length() + 1, 0) == SOCKET_ERROR)
									{
										myGlobalGame->mlog.LogError(LString("Failed sending username already exists message to client!"));
									}

									SimpleNetworkServer::getSingleton()->bCleanupUnwantedClients = true;
								}
							}
						} else {
							myGlobalGame->mlog.LogWarning(LString("Client on %d was not allowed to join client provided incorrect password!", clientSocket));

							if(send(clientSocket, sorryPasswordMessage.c_str(), sorryPasswordMessage.length() + 1, 0) == SOCKET_ERROR)
							{
								myGlobalGame->mlog.LogError(LString("Failed sending incorrect password message to client!"));
							}

							SimpleNetworkServer::getSingleton()->bCleanupUnwantedClients = true;
						}

						if(!SimpleNetworkServer::getSingleton()->bCleanupUnwantedClients)
						{
							SimpleNetworkServer::getSingleton()->SendMessageExceptGivenSocket(
								LString("%s has joined the server!",
								SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].username),
								-1);

							if(send(clientSocket, welcomeMessage.c_str(), welcomeMessage.length() + 1, 0) == SOCKET_ERROR)
							{
								myGlobalGame->mlog.LogError(LString("Failed sending welcome message to client!"));
							}

							SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].pwdVerified = true;
							
							myGlobalGame->mlog.LogSuccessText(LString("Client %s on %d connected!", 
															  SimpleNetworkServer::getSingleton()->clientData[SimpleNetworkServer::getSingleton()->currentClient].username,
															  clientSocket));
						}
					}
				}	

				SimpleNetworkServer::getSingleton()->currentClient++;
			}
		}
	}

	bool SimpleNetworkServer::bInstance = false;
	SimpleNetworkServer* SimpleNetworkServer::singleton = 0;

	void SimpleNetworkServer::DCUnwantedClients(void)
	{
		if(!this->bCleanupUnwantedClients)
		{
			return;
		}

		int clientReduction = 0;

		for(int i = 0; i < this->currentClient; i++)
		{
			if(!this->clientData[i].pwdVerified)
			{
				WaitForSingleObject(mutexHandle, INFINITE);

				FD_CLR(this->clientData[i].socket, &masterSet);

				ReleaseMutex(mutexHandle);

				clientReduction++;

				if(dcMessage == "")
				{
					myGlobalGame->mlog.LogWarning(LString("Client on %d was forcefully disconnected!", this->clientData[i].socket));
					closesocket(this->clientData[i].socket);					
				} else {
					std::string details = getClientInfo(this->clientData[i].socket);					
					
					this->SendMessageExceptGivenSocket(LString("%s has been kicked from the server!",
								this->clientData[i].username),
								this->clientData[i].socket);					

					myGlobalGame->mlog.LogWarning(LString("Client %s (%s) on %d was forcefully disconnected!", details.c_str(), dcMessage.c_str(), this->clientData[i].socket));										
					closesocket(this->clientData[i].socket);

					int lastClientIndex = (this->currentClient - 1);
					if(i != lastClientIndex)
					{
						//SOCKET temp = this->clientData[i].socket;
						this->clientData[i].socket = this->clientData[lastClientIndex].socket;
						this->clientData[lastClientIndex].socket = 0;

						this->clientData[i].pwdVerified = this->clientData[lastClientIndex].pwdVerified;
						memcpy(this->clientData[i].username, this->clientData[lastClientIndex].username, 256);
						memset(this->clientData[lastClientIndex].username, 0, sizeof(char) * 256);
						this->clientData[lastClientIndex].pwdVerified = false;
						this->currentClient -= clientReduction;
						return;
					}
				}				
			}
		}

		if(dcMessage != "")
		{
			dcMessage = "";
		}

		this->currentClient -= clientReduction;

		bCleanupUnwantedClients = false;
	}

	SimpleNetworkServer* SimpleNetworkServer::getSingleton(void)
	{
		if(!bInstance)
		{
			singleton = new SimpleNetworkServer();

			if(!singleton)
			{
				myGlobalGame->mlog.LogError(LString("Error creating SimpleNetworkServer singleton"));
				return 0;
			} else {
				bInstance = true;
				return singleton;				
			}
		} else {
			return singleton;
		}
	}

	SimpleNetworkServer::SimpleNetworkServer(void)
	{		
		this->bChatServer = true;
		this->bEchoChatToConsole = true;
		this->dcMessage = "";
		this->bCleanupUnwantedClients = false;
		this->currentClient = 0;
		this->clientData = 0;
		memset(password, 0, sizeof(char) * 256);
		//strcpy(password, "rajesh");
		this->bStatus = true;
	}

	SimpleNetworkServer::~SimpleNetworkServer(void)
	{
		if(clientData)
		{
			delete[] clientData;
			clientData = 0;
		}

		this->ShutDownServer();
		bInstance = false;
	}

	bool SimpleNetworkServer::Run(void)
	{
		this->DCUnwantedClients();

		if(this->bStatus == true)
		{
			return false;
		}

		WaitForSingleObject(mutexHandle, INFINITE);
		FD_SET pollingSet = masterSet;
		ReleaseMutex(mutexHandle);

		if (pollingSet.fd_count == 0)
		{
			return true;
		}

		timeval waitTime;
		waitTime.tv_sec = 0;
		waitTime.tv_usec = 0;

		int result = select(pollingSet.fd_count, &pollingSet, NULL, NULL, &waitTime);

		if(result == 0)
		{
			return true;
		}

		if(result == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Error in select"));

			return false;
		}

		for(unsigned int i = 0; i< pollingSet.fd_count; i++)
		{
			unsigned int clientSocket = pollingSet.fd_array[i];

			char buffer[MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER];
			memset(buffer, 0, sizeof(char) * MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER);
			int messageSize;			

			messageSize = ReceiveData(clientSocket, buffer, MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER);

			if(messageSize == -1)
			{
				continue;
			} else if(messageSize == 0)
			{
				return false;
			}

			char command[256];
			 
			if(0 == strnicmp("\\co", buffer, 3))
			{
				for(int m = 0; m < this->currentClient ; m++)
				{
					if(this->clientData[m].socket == clientSocket)
					{
						memcpy(command, &buffer[4], sizeof(char) * 256);
						//myGlobalGame->mlog.LogInfo(LString("%s: %s", this->clientData[m].username, command));
						myGlobalGame->mlog.executeCommand(command, this->clientData[m].username);
						break;
					}
				}
				
			} else {
				
				if(this->bChatServer)
				{
					for(int m = 0; m < this->currentClient ; m++)
					{
						if(this->clientData[m].socket == clientSocket)
						{
							if(this->bEchoChatToConsole)
							{
								myGlobalGame->mlog.LogInfo(LString("%s: %s", this->clientData[m].username, buffer));
							}

							this->SendMessageExceptGivenSocket(LString("%s: %s", this->clientData[m].username, buffer), -1); 

							break;
						}
					}
				}

				//myGlobalGame->mlog.LogInfo(LString("Client %d sent: %s\n", clientSocket, buffer));
			}
		}

		return true;
	}

	void SimpleNetworkServer::KickUser(const char* username)
	{
		for(int m = 0; m < this->currentClient ; m++)
		{
			if(0 == stricmp(username, this->clientData[m].username))
			{
				std::string ywkMessage = "You were kicked from the server!";

				if(send(this->clientData[m].socket, ywkMessage.c_str(), ywkMessage.length() + 1, 0) == SOCKET_ERROR)
				{
					myGlobalGame->mlog.LogError(LString("Failed sending kick message to client %s!", this->clientData[m].username));
				}

				this->clientData[m].pwdVerified = false;
				dcMessage = "Kicked";
				this->bCleanupUnwantedClients = true;
				return;
			}
		}

		myGlobalGame->mlog.LogError(LString("Client %s was not found, kick failed!", username));
	}

	void Kick(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			std::vector<int> userNamePosition;
			for(unsigned int i = 0; i < parameters.length();)
			{
				i = parameters.find("/", i);

				if(i == -1)
				{
					break;
				}

				userNamePosition.push_back(i);
				i++;
			}	

			if(userNamePosition.size() <= 0)
			{
			} else {
				std::string username;

				for(unsigned int i = 0; i < userNamePosition.size(); i++)
				{
					if(i == (userNamePosition.size() - 1))
					{
						username = parameters.substr(userNamePosition[i] + 1, (parameters.length() - userNamePosition[i]) - 1);						
					} else {
						username = parameters.substr(userNamePosition[i] + 1, (userNamePosition[i + 1] - userNamePosition[i]) - 2);						
					}

					SimpleNetworkServer::getSingleton()->KickUser(username.c_str());
				}
			}
		}
	}

	void SimpleNetworkServer::ShowVariableToUser(const char* variableName,
								const char* clientVarID,
								const char* username)
	{
		std::string finalMessage = myGlobalGame->mlog.getVariable(variableName) + "-";
		finalMessage += clientVarID;
		finalMessage += "@";

		if(!username)
		{
			for(int m = 0; m < this->currentClient ; m++)
			{
				if(send(this->clientData[m].socket, finalMessage.c_str(), finalMessage.length() + 1, 0) == SOCKET_ERROR)
				{
					myGlobalGame->mlog.LogError(LString("Failed sending variable client message to client %s!", this->clientData[m].username));
				}
			}
			return;
		} else {
			for(int m = 0; m < this->currentClient ; m++)
			{
				if(0 == stricmp(username, this->clientData[m].username))
				{
					if(send(this->clientData[m].socket, finalMessage.c_str(), finalMessage.length() + 1, 0) == SOCKET_ERROR)
					{
						myGlobalGame->mlog.LogError(LString("Failed sending variable client message to client %s!", this->clientData[m].username));
					}

					return;
				}
			}
		}

		myGlobalGame->mlog.LogError(LString("Error showing %s to %s with cID: %s",
											 variableName,
											 username,
											 clientVarID));
	}

	void GetVar(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			std::vector<int> userNamePosition;
			for(unsigned int i = 0; i < parameters.length();)
			{
				i = parameters.find("/", i);

				if(i == -1)
				{
					break;
				}

				userNamePosition.push_back(i);
				i++;
			}			
			
			if(userNamePosition.size() <= 0)
			{
				int spaceSearchResult = parameters.find(" ");
				if(spaceSearchResult == -1)
				{
					std::string variableString = parameters.substr(0);

					SimpleNetworkServer::getSingleton()->ShowVariableToUser(variableString.c_str(), variableString.c_str(), 0);
				} else {
					std::string variableString = parameters.substr(0, spaceSearchResult);
					std::string variableClientIDString = parameters.substr(spaceSearchResult + 1);

					SimpleNetworkServer::getSingleton()->ShowVariableToUser(variableString.c_str(), variableClientIDString.c_str(), 0);
				}
			} else {
				int messagePosition = 0;
				int tempMessagePosition = 0;

				for(unsigned int i = 0; i < parameters.size();)
				{
					tempMessagePosition = parameters.find(" ", i);

					if(tempMessagePosition == -1)
					{
						break;
					}
					
					messagePosition = tempMessagePosition;
					i = messagePosition + 1;	

					if(parameters.find("/", tempMessagePosition) == -1)
					{
						break;
					}
				}

				std::string variableString = parameters.substr(messagePosition + 1);
				std::string variableClientIDString;

				if(variableString.find("/") != -1)
				{
					myGlobalGame->mlog.LogError(LString("Variable name not found in command, GetVar operation failed!"));
					return;
				}

				int spaceSearchResult = variableString.find(" ");

				if(spaceSearchResult == -1)
				{
					variableString = variableString.substr(0);
				} else {					
					variableClientIDString = variableString.substr(spaceSearchResult + 1);
					variableString = variableString.substr(0, spaceSearchResult);
				}

				std::string username;

				for(unsigned int i = 0; i < userNamePosition.size(); i++)
				{
					if(i == (userNamePosition.size() - 1))
					{
						username = parameters.substr(userNamePosition[i] + 1, (messagePosition - userNamePosition[i]) - 1);						
					} else {
						username = parameters.substr(userNamePosition[i] + 1, (userNamePosition[i + 1] - userNamePosition[i]) - 2);						
					}
					
					SimpleNetworkServer::getSingleton()->ShowVariableToUser(variableString.c_str(),
																			variableClientIDString.c_str(),
																			username.c_str());
				}
			}
		}
	}

	void SimpleNetworkServer::SendMessageExceptGivenSocket(const char* message, unsigned int socket)
	{
		for(int m = 0; m < this->currentClient ; m++)
		{
			if(this->clientData[m].socket != socket)
			{
				if(send(this->clientData[m].socket, message, strlen(message) + 1, 0) == SOCKET_ERROR)
				{
					myGlobalGame->mlog.LogError(LString("Failed sending peer message to client %s!", this->clientData[m].username));
				}
			}
		}
	}

	void SimpleNetworkServer::SendMessage(const char* message, const char* username)
	{
		if(!username)
		{
			for(int m = 0; m < this->currentClient ; m++)
			{
				if(send(this->clientData[m].socket, message, strlen(message) + 1, 0) == SOCKET_ERROR)
				{
					myGlobalGame->mlog.LogError(LString("Failed sending message to client %s!", this->clientData[m].username));
				}
			}
		} else {
			int lengthOfInputUserName = strlen(username);

			for(int m = 0; m < this->currentClient ; m++)
			{
				if(0 == stricmp(username, this->clientData[m].username))
				{
					if(send(this->clientData[m].socket, message, strlen(message) + 1, 0) == SOCKET_ERROR)
					{
						myGlobalGame->mlog.LogError(LString("Failed sending message to client %s!", this->clientData[m].username));
					}

					return;
				}
			}

			myGlobalGame->mlog.LogError(LString("Client %s was not found, message failed to send!", username));
		}
	}

	void Send(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			std::vector<int> userNamePosition;
			for(unsigned int i = 0; i < parameters.length();)
			{
				i = parameters.find("/", i);

				if(i == -1)
				{
					break;
				}

				userNamePosition.push_back(i);
				i++;
			}			
			
			if(userNamePosition.size() <= 0)
			{
				std::string message = parameters.substr(0);

				SimpleNetworkServer::getSingleton()->SendMessage(message.c_str());
			} else {
				int messagePosition = 0;
				int tempMessagePosition = 0;

				for(unsigned int i = 0; i < parameters.size();)
				{
					tempMessagePosition = parameters.find(" ", i);

					if(tempMessagePosition == -1)
					{
						break;
					}
					
					messagePosition = tempMessagePosition;
					i = messagePosition + 1;	

					if(parameters.find("/", tempMessagePosition) == -1)
					{
						break;
					}
				}

				std::string message = parameters.substr(messagePosition + 1);

				if(message.find("/") != -1)
				{
					myGlobalGame->mlog.LogError(LString("Message not found in command, message operation failed!"));
					return;
				}

				std::string username;

				for(unsigned int i = 0; i < userNamePosition.size(); i++)
				{
					if(i == (userNamePosition.size() - 1))
					{
						username = parameters.substr(userNamePosition[i] + 1, (messagePosition - userNamePosition[i]) - 1);						
					} else {
						username = parameters.substr(userNamePosition[i] + 1, (userNamePosition[i + 1] - userNamePosition[i]) - 2);						
					}

					SimpleNetworkServer::getSingleton()->SendMessage(message.c_str(), username.c_str());
				}
			}
		}
	}

	std::string SimpleNetworkServer::getClientInfo(unsigned int socket)
	{
		std::string output = "";
		for(int m = 0; m < this->currentClient ; m++)
		{
			if(this->clientData[m].socket == socket)
			{
				sockaddr_in name;
				int namelen = sizeof(name);
				getpeername(socket, (SOCKADDR*) &name, &namelen);				

				output = clientData[m].username;
				output += " ";
				output += inet_ntoa(name.sin_addr);
				output += ":";
				char temp[8];
				sprintf(temp, "%d", name.sin_port);
				output += temp;

				return output;
			}
		}

		return output;
	}

	void SimpleNetworkServer::CleanSocketAfterDC(unsigned int socket)
	{
		int lastClientIndex = this->currentClient - 1;
		for(int m = 0; m < this->currentClient; m++)
		{
			if(m != lastClientIndex)
			{
				if(socket == this->clientData[m].socket)
				{
					this->clientData[m].socket = this->clientData[lastClientIndex].socket;
					this->clientData[lastClientIndex].socket = 0;

					this->clientData[m].pwdVerified = this->clientData[lastClientIndex].pwdVerified;
					this->clientData[lastClientIndex].pwdVerified = false;

					memcpy(this->clientData[m].username, this->clientData[lastClientIndex].username, 256);
					memset(this->clientData[lastClientIndex].username, 0, sizeof(char) * 256);
					return;
				}
			}
		}
	}

	int SimpleNetworkServer::ReceiveData(SOCKET socket, const char* receivedData,
										 unsigned int bufferMaxSize)
	{
		unsigned int clientSocket = socket;

		int nBytes;

		nBytes = recv(clientSocket, (char*)receivedData, bufferMaxSize, 0);

		if(nBytes == SOCKET_ERROR)
		{
			int error = WSAGetLastError();

			if(error == WSAECONNRESET)
			{
				WaitForSingleObject(mutexHandle, INFINITE);

				FD_CLR(clientSocket, &masterSet);

				ReleaseMutex(mutexHandle);

				std::string dcUserName = getClientInfo(clientSocket).c_str();

				myGlobalGame->mlog.LogInfo(LString("%s on %d disconnected", dcUserName.c_str(), clientSocket));

				this->SendMessageExceptGivenSocket(LString("%s left the server!", dcUserName.c_str()), clientSocket);

				closesocket(clientSocket);

				this->CleanSocketAfterDC(clientSocket);

				SimpleNetworkServer::getSingleton()->currentClient--;
				return -1;
			} else {
				myGlobalGame->mlog.LogError(LString("Recv failed"));
				this->bStatus = true;
			}
		}

		if(nBytes == 0)
		{
			WaitForSingleObject(mutexHandle, INFINITE);

			FD_CLR(clientSocket, &masterSet);

			ReleaseMutex(mutexHandle);

			std::string dcUserName = getClientInfo(clientSocket).c_str();

			myGlobalGame->mlog.LogInfo(LString("%s on %d disconnected", dcUserName.c_str(), clientSocket));

			this->SendMessageExceptGivenSocket(LString("%s left the server!", dcUserName.c_str()), clientSocket);

			closesocket(clientSocket);			

			this->CleanSocketAfterDC(clientSocket);
			
			SimpleNetworkServer::getSingleton()->currentClient--;
            
            return -1;
		}

		if(nBytes == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Recv failed"));
			this->bStatus = true;
			return 0;
		}

		//messageSize = strlen(receivedData);

		return nBytes;
	}

	bool SimpleNetworkServer::Init(unsigned short port, unsigned int totalClients)
	{
		this->bStatus = true;

		this->maxClients = totalClients;

		myGlobalGame->mlog.bindStringVariable("password", this->password);

		myGlobalGame->mlog.bindCommand("ServerInfo", PrintConsoleServerInfo);		
		myGlobalGame->mlog.bindCommand("ClientInfo", PrintClients);
		myGlobalGame->mlog.bindCommand("Kick", Kick);
		myGlobalGame->mlog.bindCommand("Send", Send);
		myGlobalGame->mlog.bindCommand("Get", GetVar);
		myGlobalGame->mlog.bindCommand("Write", Write);
		myGlobalGame->mlog.bindCommand("WriteLn", WriteLine);
		myGlobalGame->mlog.bindCommand("ClearFile", ClearFile);		

		myGlobalGame->mlog.bindBooleanVariable("bChatServer", &this->bChatServer);
		myGlobalGame->mlog.bindBooleanVariable("bEchoChat", &this->bEchoChatToConsole);
		
		if(clientData)
		{
			delete[] clientData;
			clientData = 0;
		}

		clientData = new ClientData[this->maxClients];

		if(!clientData)
		{
			myGlobalGame->mlog.LogError(LString("Network startup failed, insufficient memory for clients!"));
			return false;
		}

		this->serverSocket = this->StartListening(port);

		if(this->serverSocket == -1) 
		{
			myGlobalGame->mlog.LogError(LString("Network startup failed!"));
			return false;
		}

		mutexHandle = CreateMutex(NULL, false, NULL);

		if(mutexHandle == NULL)
		{
			myGlobalGame->mlog.LogError(LString("Could not create mutex"));
			this->ShutDownServer();
			this->bStatus = true;
			return false;
		}		

		threadHandle = CreateThread(NULL, 0,
								    (LPTHREAD_START_ROUTINE)acceptingThreadProcedure,
									&this->serverSocket, 0, (LPDWORD)&threadID);

		if(threadHandle == NULL)
		{
			myGlobalGame->mlog.LogError(LString("Could not create thread!"));
			this->ShutDownServer();
			this->bStatus = true;
			return false;
		}

		//Sleep(100);

		FD_ZERO(&masterSet);

		this->bStatus = false;

		myGlobalGame->mlog.LogSuccessText(LString("Network init success!"));

		std::string param;
		PrintConsoleServerInfo(param);

		return true;
	}

	int SimpleNetworkServer::StartListening(unsigned short port)
	{
		int error;

		WSAData wsadata;

		if((error = WSAStartup(MAKEWORD(2,2), &wsadata) == SOCKET_ERROR))
		{
			myGlobalGame->mlog.LogError(LString("Could not start SimpleNetworkServer!"));
			return -1;
		}

		int mySocket = socket(AF_INET, SOCK_STREAM, 0);

		if(mySocket == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Could not create socket!"));
			return -1;
		}

		struct sockaddr_in server;

		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.s_addr = INADDR_ANY;

		if(bind(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Could not bind socket!"));
			closesocket(mySocket);
			return -1;
		}

		if(listen(mySocket, 5) == SOCKET_ERROR)
		{
			myGlobalGame->mlog.LogError(LString("Listen failed!"));
			closesocket(mySocket);
			return -1;
		}		

		return mySocket;
	}

	void SimpleNetworkServer::ShutDownServer(void)
	{
		WaitForSingleObject(mutexHandle, INFINITE);
		CloseHandle(threadHandle);
		CloseHandle(mutexHandle);

		closesocket(this->serverSocket);

		myGlobalGame->mlog.LogSuccessText(LString("Server shutdown!"));
	}

	bool SimpleNetworkServer::IsNotOperational(void)
	{
		return this->bStatus;
	}	

	unsigned int SimpleNetworkServer::getMaxClients(void) const
	{
		return this->maxClients;
	}

	unsigned int SimpleNetworkServer::getCurrentClient(void) const
	{
		return this->currentClient;
	}

	bool SimpleNetworkServer::WriteToFile(const char* fn, const char* text, bool newLine)
	{

		std::ofstream fout;

		fout.open(fn, std::ios_base::out|std::ios_base::app);

		if(fout.fail ())
		{
			return false;
		}

		if(!newLine)
		{
			fout<<text;
		} else {
			fout<<"\n"<<text;
		}

		fout.close();
		return true;
	}

	void WriteLine(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			int tempFileNamePosition = parameters.find('/', 0);

			if(tempFileNamePosition == -1)
			{
				myGlobalGame->mlog.LogError(LString("Please check input parameters"));
				return;
			}

			int linePosition = parameters.find('/', tempFileNamePosition + 1);

			if(linePosition == -1)
			{
				myGlobalGame->mlog.LogError(LString("Please check input parameters"));
				return;
			}

			std::string fileName = parameters.substr(tempFileNamePosition + 1, linePosition - 2);

			if(parameters.length() <= 0)
			{
				myGlobalGame->mlog.LogError(LString("Bad file name"));
				return;
			}

			std::string data = parameters.substr(linePosition + 1);

			if(!SimpleNetworkServer::getSingleton()->WriteToFile(fileName.c_str(), data.c_str(), true))
			{
				myGlobalGame->mlog.LogError(LString("There was a failure writing to file %s", fileName.c_str()));
				return;
			}
		}
	}	

	void Write(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			int tempFileNamePosition = parameters.find('/', 0);

			if(tempFileNamePosition == -1)
			{
				myGlobalGame->mlog.LogError(LString("Please check input parameters"));
				return;
			}

			int linePosition = parameters.find('/', tempFileNamePosition + 1);

			if(linePosition == -1)
			{
				myGlobalGame->mlog.LogError(LString("Please check input parameters"));
				return;
			}

			std::string fileName = parameters.substr(tempFileNamePosition + 1, linePosition - 2);

			if(fileName.length() <= 0)
			{
				myGlobalGame->mlog.LogError(LString("Bad file name"));
				return;
			}

			std::string data = parameters.substr(linePosition + 1);

			if(!SimpleNetworkServer::getSingleton()->WriteToFile(fileName.c_str(), data.c_str()))
			{
				myGlobalGame->mlog.LogError(LString("There was a failure writing to file %s", fileName.c_str()));
				return;
			}
		}
	}

	void ClearFile(std::string &parameters)
	{
		if(parameters.length() > 0)
		{
			int tempFileNamePosition = parameters.find('/', 0);

			if(tempFileNamePosition == -1)
			{
				myGlobalGame->mlog.LogError(LString("Please check input parameters"));
				return;
			}

			std::string fileName = parameters.substr(tempFileNamePosition + 1);

			if(fileName.length() <= 0)
			{
				myGlobalGame->mlog.LogError(LString("Bad file name"));
				return;
			}

			std::ofstream fout;

			fout.open(fileName.c_str(), std::ios_base::out);

			if(fout.fail ())
			{
				myGlobalGame->mlog.LogError(LString("Error clearing file %s", fileName));
				return;
			}

			fout.close();
		}
	}
}