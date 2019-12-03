#include "TcpSocket.h"

int TcpSocket::connect(const std::string& hostname, uint16_t port)
{
  // ToDo: Connect to camera TCP Socket
  return 0;
}

int TcpSocket::shutdown()
{
  // ToDo: Close TCP connection to camera
  return 0;
}

int TcpSocket::send(const std::vector<std::uint8_t>& buffer)
{
  // ToDo: send buffer via TCP socket
  return 0;
}

int TcpSocket::recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive)
{
  // ToDo: receive from TCP Socket
  return 0;
}
