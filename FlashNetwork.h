/*	
============================================================================================
	Classes contained are
		class SimpleNetworkServer

	This class is used for simple network communication.
============================================================================================
*/

#ifndef FLASH_NETWORK_H
#define FLASH_NETWORK_H

#define NOMINMAX

#pragma comment(lib, "WS2_32.LIB")

#include <string>
#include <fstream>
#include <winsock2.h>

#include "Client.h"

#define	MAX_INPUT_BUFFER_SIZE_NETWORK_SERVER 128

namespace DifferentialArts
{
	/*!	\struct ClientData
		\brief Basic client data holder
		\author Rajesh Peter Douglas D'Monte

		This structure is used by \class SimpleNetworkServer
		internally to store client information
	*/
	struct RX_API ClientData
	{
		bool pwdVerified;
		SOCKET socket;
		char username[256];
	};

	/*!	\class SimpleNetworkServer
		\brief Simple network layer
		\author Rajesh Peter Douglas D'Monte

		Use this class if you would like to run a custom
		server which operates on raw data. This class is a 
		singleton. This is a thin engine control layer
		and can be used to control engine remotely through
		the console
	*/

	class RX_API SimpleNetworkServer
	{
	public:
		SimpleNetworkServer(void);	//!< Constructor
		~SimpleNetworkServer(void); //!< Destructor. Shuts down server.

		bool Init(unsigned short port, unsigned int totalClients = 32); //!< Initialize the server with a given port number
		bool Run(void);					//!< Makes the server listen, call in Update
		bool IsNotOperational(void);	//!< Returns if the server isn't operational even when \a Run is executed.

		static SimpleNetworkServer* getSingleton(void); //!< Returns the singleton object		

		unsigned int getMaxClients(void) const;			//!< Returns total clients supported by server
		unsigned int getCurrentClient(void) const;		//!< Returns total clients connected to server

		void SendMessage(const char* message, const char* username = 0);
		//!< Sends message to a given user. If \a username is 0 then message is send to all clients

		void KickUser(const char* username); //!< Kicks the user with the provided user name

		void ShowVariableToUser(const char* variableName,
								const char* clientVarID,
								const char* username = 0);
		//!<Sends a variable on server to the given user attached with \a clientVarID

	protected:
		ClientData* clientData;
		int maxClients;
		int currentClient;

		HANDLE threadHandle;
		HANDLE mutexHandle;
		FD_SET masterSet;

		int serverSocket;
		int threadID;
		bool bStatus;	

		static SimpleNetworkServer* singleton;

		static bool bInstance;

		char password[256];		

		bool bCleanupUnwantedClients;
		bool bChatServer;
		bool bEchoChatToConsole;

		int StartListening(unsigned short port);
		void ShutDownServer(void);	

		void DCUnwantedClients(void);
		virtual int ReceiveData(SOCKET socket, const char* receivedData,
						 unsigned int bufferMaxSize);
		std::string getClientInfo(unsigned int socket); 
		void SendMessageExceptGivenSocket(const char* message, unsigned int socket);
		void CleanSocketAfterDC(unsigned int socket);
		bool WriteToFile(const char* fn, const char* text, bool newLine = false);

		friend RX_API void acceptingThreadProcedure(int *serverSocket);
		friend void PrintConsoleServerInfo(std::string &parameters);
		friend void PrintClients(std::string &parameters);
		friend void Write(std::string &parameters);
		friend void WriteLine(std::string &parameters);		

		std::string dcMessage;
	};	

	RX_API void acceptingThreadProcedure(int *serverSocket);	
}

#endif