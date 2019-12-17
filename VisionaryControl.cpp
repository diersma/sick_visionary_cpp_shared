//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: August 2017
// 
// @author:  Andreas Richert
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de
// 
// Last commit: $Date: 2017-12-06 17:17:50 +0100 (Mi, 06 Dez 2017) $
// Last editor: $Author: richean $
// 
// Version "$Revision: 15145 $"
//

#include <cassert>
#include "VisionaryControl.h"
#include "VisionaryEndian.h"
#include "CoLaParameterWriter.h"
#include "CoLaParameterReader.h"
#include "CoLaBProtocolHandler.h"
#include "TcpSocket.h"
#include "ControlSession.h"
#include "ColaParameterWriter.h"
#include "AuthenticationLegacy.h"
#include "CoLaParameterWriter.h"

VisionaryControl::VisionaryControl()
{
}

VisionaryControl::~VisionaryControl()
{
}

bool VisionaryControl::open(ProtocolType type, const std::string& hostname, std::uint16_t port, uint32_t sessionTimeout_ms)
{
  m_pProtocolHandler = nullptr;
  m_pTransport = nullptr;

  std::unique_ptr<TcpSocket> pTransport(new TcpSocket());
  
  if (pTransport->connect(hostname, port) != 0)
  {
    return false;
  }

  std::unique_ptr<IProtocolHandler> pProtocolHandler;

  switch (type)
  {
  case COLA_B:
    pProtocolHandler = std::unique_ptr<IProtocolHandler>(new CoLaBProtocolHandler(*pTransport));
    break;
  case COLA_2:
    //pProtocolHandler = std::unique_ptr<IProtocolHandler>(new CoLa2ProtocolHandler(*pTransport));
    break;
  default:
    assert(false /* unsupported protocol*/);
    return false;
  }

  if (!pProtocolHandler->openSession(sessionTimeout_ms))
  {
    pTransport->shutdown();
    return false;
  }

  std::unique_ptr <ControlSession> pControlSession;
  pControlSession = std::unique_ptr<ControlSession>(new ControlSession(*pProtocolHandler));

  std::unique_ptr <IAuthentication> pAuthentication;
  pAuthentication = std::unique_ptr<IAuthentication>(new AuthenticationLegacy(*this));

  m_pTransport       = std::move(pTransport);
  m_pProtocolHandler = std::move(pProtocolHandler);
  m_pControlSession  = std::move(pControlSession);
  m_pAuthentication  = std::move(pAuthentication);

  return true;
}

void VisionaryControl::close()
{
  if (m_pAuthentication)
  {
    (void)m_pAuthentication->logout();
    m_pAuthentication = nullptr;
  }
  if (m_pProtocolHandler)
  {
    m_pProtocolHandler->closeSession();
    m_pProtocolHandler = nullptr;
  }
  if (m_pTransport)
  {
    m_pTransport->shutdown();
    m_pTransport = nullptr;
  }
  if (m_pControlSession)
  {
    m_pControlSession = nullptr;
  }
}

int VisionaryControl::login(IAuthentication::UserLevel userLevel, const std::string password)
{
  return m_pAuthentication->login(userLevel, password);
}

int VisionaryControl::logout()
{
  return m_pAuthentication->logout();
}

bool VisionaryControl::startAcquisition() 
{
  CoLaCommand startCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "PLAYSTART").build();
  CoLaCommand response = m_pControlSession->send(startCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControl::stepAcquisition() 
{
  return false;
}

bool VisionaryControl::stopAcquisition() 
{
  CoLaCommand startCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "PLAYSTOP").build();
  CoLaCommand response = m_pControlSession->send(startCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControl::getDataStreamConfig() 
{
  return false;
}
