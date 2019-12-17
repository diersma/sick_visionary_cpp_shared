#include "TcpSocket.h"

int TcpSocket::connect(const std::string& hostname, uint16_t port)
{
  int iResult = 0;
#ifdef _WIN32
  //-----------------------------------------------
  // Initialize Winsock
  WSADATA wsaData;
  iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR)
  {
    return iResult;
  }
#endif

  //-----------------------------------------------
  // Create a receiver socket to receive datagrams
  m_recvSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_recvSocket == INVALID_SOCKET) {
    return INVALID_SOCKET;
  }

  //-----------------------------------------------
  // Bind the socket to any address and the specified port.
  sockaddr_in recvAddr;
  recvAddr.sin_family = AF_INET;
  recvAddr.sin_port = port;
  recvAddr.sin_addr.s_addr = inet_addr(hostname.c_str());

  iResult = ::connect(m_recvSocket, (sockaddr*)&recvAddr, sizeof(recvAddr));
  if (iResult != 0)
  {
    return false;
  }

  // Set the timeout for the socket to 5 seconds
  long timeoutSeconds = 5L;
#ifdef _WIN32
  // On Windows timeout is a DWORD in milliseconds (https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-setsockopt)
  long timeoutMs = timeoutSeconds * 1000L;
  iResult = setsockopt(m_recvSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutMs, sizeof(DWORD));
#else
  struct timeval tv;
  tv.tv_sec = timeoutSeconds;  /* 5 seconds Timeout */
  tv.tv_usec = 0L;  // Not init'ing this can cause strange errors
  iResult = setsockopt(m_recvSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
#endif

  return iResult;
}

int TcpSocket::shutdown()
{
  // Close the socket when finished receiving datagrams
#ifdef _WIN32
  closesocket(m_recvSocket);
  WSACleanup();
#else
  close(m_recvSocket);
#endif
  m_recvSocket = INVALID_SOCKET;
  return 0;
}

int TcpSocket::send(const std::vector<std::uint8_t>& buffer)
{
  // send buffer via TCP socket
  return ::send(m_recvSocket, (char*)buffer.data(), (int)buffer.size(), 0);
}

int TcpSocket::recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive)
{
  // receive from TCP Socket
  buffer.resize(maxBytesToReceive);
  return receiveData(maxBytesToReceive, reinterpret_cast<char*>(buffer.data()));
}

int TcpSocket::receiveData(uint32_t bytesToReceive, char* pBuffer)
{
  int bytesReceived = 0;
  while (bytesToReceive > 0)
  {
    bytesReceived = ::recv(m_recvSocket, pBuffer, bytesToReceive, 0);

    if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
    {
      return false;
    }
    pBuffer += bytesReceived;
    bytesToReceive -= bytesReceived;
  }
  pBuffer = NULL;
  return bytesReceived;
}

