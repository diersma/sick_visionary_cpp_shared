#pragma once
#include "IProtocolHandler.h"
#include "ITransport.h"
#include "CoLaCommand.h"

class CoLaBProtocolHandler:
  public IProtocolHandler
{
public:
  CoLaBProtocolHandler(ITransport& rTransport);
  ~CoLaBProtocolHandler();

  bool openSession(uint32_t/*sessionTimeout*/_ms);
  void closeSession();

  // send cola cmd and receive cola response
  CoLaCommand send(CoLaCommand cmd);
};
