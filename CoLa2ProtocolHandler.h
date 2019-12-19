#pragma once
#include "IProtocolHandler.h"
#include "ITransport.h"
#include "CoLaCommand.h"

class CoLa2ProtocolHandler:
  public IProtocolHandler
{
public:
  CoLa2ProtocolHandler(ITransport& rTransport);
  ~CoLa2ProtocolHandler();

  bool openSession(uint8_t sessionTimeout/*secs*/);
  void closeSession();

  // send cola cmd and receive cola response
  CoLaCommand send(CoLaCommand cmd);

private:
  ITransport& m_rTransport;
  uint16_t m_ReqID;
  uint32_t m_sessionID;
  uint8_t calculateChecksum(const std::vector<uint8_t>& buffer);
  uint16_t getReqId();
  std::vector<std::uint8_t> createCoLa2Header();

};
