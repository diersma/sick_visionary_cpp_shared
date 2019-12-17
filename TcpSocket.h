#pragma once
#include <string>
#include <cstdint>

#include "ITransport.h"

// Include socket
#ifdef _WIN32    // Windows specific
#include <winsock2.h>
#include <Ws2tcpip.h>
// to use with other compiler than Visual C++ need to set Linker flag -lws2_32
#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#endif
#else        // Linux specific
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int SOCKET;
#define INVALID_SOCKET  ((SOCKET)(~0))
#define SOCKET_ERROR    (-1)
#endif

class TcpSocket :
  public ITransport
{
public:
  int connect(const std::string& hostname, uint16_t port);
  int shutdown();

  int send(const std::vector<std::uint8_t>& buffer) override;
  int recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive) override;

private:
  int receiveData(uint32_t bytesToReceive, char* pBuffer);
    
  unsigned short m_port;
  unsigned long m_ipAddress;

  SOCKET m_recvSocket;
};

