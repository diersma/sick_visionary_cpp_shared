#pragma once

#include <cstdint>

class IProtocolHandler
{
public:
  virtual bool openSession(uint32_t/*sessionTimeout*/_ms) = 0;
  virtual void closeSession() = 0;
};

