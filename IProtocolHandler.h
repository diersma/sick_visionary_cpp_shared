#pragma once

#include <cstdint>
#include "CoLaCommand.h"

class IProtocolHandler
{
public:
  virtual bool openSession(uint8_t sessionTimeout /*secs*/) = 0;
  virtual void closeSession() = 0;
  virtual CoLaCommand send(CoLaCommand cmd) = 0;
};

