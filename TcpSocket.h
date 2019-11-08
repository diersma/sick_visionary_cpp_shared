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
};

