#pragma once
#include "IProtocolHandler.h"
#include "ITransport.h"

class CoLaBProtocolHandler:
  public IProtocolHandler
{
public:
  CoLaBProtocolHandler(ITransport& rTransport);

  bool openSession(uint32_t/*sessionTimeout*/_ms);
  void closeSession();
};
