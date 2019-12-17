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

private:
  ITransport& m_rTransport;
  uint8_t calculateChecksum(const std::vector<uint8_t>& buffer);
};
