#pragma once

#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <iphlpapi.h>



#include "Logger.h"

#pragma comment(lib, "Ws2_32.lib")

namespace http {
	class TcpServer
	{
	public:
		TcpServer(int port,std::string ip);
		~TcpServer();
		
	private:
		int startServer();

		int _port;
		WSADATA _wsaData;
		SOCKET _ListenSocket;
		Logger logger;
	};
	
}


