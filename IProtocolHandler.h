#pragma once

#include <cstdint>

class IProtocolHandler
{
public:
  bool openSession(uint32_t/*sessionTimeout*/_ms);
  void closeSession();
};

