#pragma once
#include <string>
#include <cstdint>

#include "ITransport.h"

class TcpSocket :
  public ITransport
{
public:
  int connect(const std::string& hostname, uint16_t port);
  int shutdown();

  int send(const std::vector<std::uint8_t>& buffer);
  int recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive, std::size_t minBytesToReceive);
};

