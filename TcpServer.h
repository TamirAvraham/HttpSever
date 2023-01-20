#pragma once

#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <functional>
#include <thread>

#include "Logger.h"

#pragma comment(lib, "Ws2_32.lib")

namespace http {
	

	class TcpServer
	{
	public:
		TcpServer(int port,std::string ip);
		~TcpServer();
		void HandleConection(std::function<void(SOCKET sock)> handleFunction);
		template<class I>
		void HandleConection(std::function<void(I,SOCKET sock)> handleFunction,I inputs);
	private:
		int startServer();
		void startListen();
		void acceptConnection(SOCKET& new_socket);

		int _port;
		std::string _ip_address;

		WSADATA _wsaData;
		SOCKET _ListenSocket;
		
		sockaddr_in _socketAddress;
		socklen_t _socketAddress_len;
	};



	class simpleSocket
	{
	public:
		simpleSocket(SOCKET socket);
		~simpleSocket();
		std::string read(int bufferSize);
		void write(std::string msg);
	private:
		SOCKET _socket;

	};
}


