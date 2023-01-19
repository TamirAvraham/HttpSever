#include "TcpServer.h"

#define INIT_ERROR 505

http::TcpServer::TcpServer(std::string port, std::string ip)
{
    
   /* int iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        throw INIT_ERROR;
    }
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;


    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        throw INIT_ERROR;
    }

    _ListenSocket = INVALID_SOCKET;
    _ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (_ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        throw INIT_ERROR;
    }*/
}

http::TcpServer::~TcpServer()
{
    closesocket(_ListenSocket);
    WSACleanup();
}

int http::TcpServer::startServer()
{
    
    if (WSAStartup(MAKEWORD(2, 0), &_wsaData) != 0)
    {

        logger.exitWithError("WSAStartup failed");
        return INIT_ERROR;
    }

    _ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_ListenSocket < 0)
    {
        logger.exitWithError("Cannot create socket");
        throw INIT_ERROR;
    }

    return 1;
}
