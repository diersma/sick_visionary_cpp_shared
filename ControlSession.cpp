/// \copyright Redistribution and use in source, with or without modification, are permitted.
///
/// Definition of the controlsession interface
///
/// \author Marco Dierschke, SICK AG
/// \email TechSupport0905@sick.de
///
/// \version 0.0.1
//---------------------------------------------------------------------------------------
#include "ControlSession.h"
//---------------------------------------------------------------------------------------

ControlSession::ControlSession(IProtocolHandler& ProtocolHandler)
  : m_ProtocolHandler(ProtocolHandler)
{
}

ControlSession::~ControlSession()
{
}

CoLaCommand ControlSession::prepareRead(const std::string& varname)
{
  std::vector<uint8_t> buffer;
  CoLaCommand cmd(buffer);
  return cmd;
}

CoLaCommand ControlSession::prepareWrite(const std::string& varname)
{
  std::vector<uint8_t> buffer;
  CoLaCommand cmd(buffer);
  return cmd;
}

CoLaCommand ControlSession::prepareCall(const std::string& varname)
{
  std::vector<uint8_t> buffer = { 's', 'M', 'N' }; // sMN
  CoLaCommand cmd(buffer);
  return cmd;
}

CoLaCommand ControlSession::send(const CoLaCommand& cmd)
{
  // ToDo: send command via CoLaProtocolHandler?
  // ProcolHandler needs to add e.g. header and checksum
  // Afterwards send to socket and receive the response.
  // return the response.
  return m_ProtocolHandler.send(cmd);
}

//---------------------------------------------------------------------------------------
