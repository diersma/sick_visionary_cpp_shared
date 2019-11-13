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

  std::unique_ptr<ITransport/*TcpSocket*/> pTransport(new TcpSocket());
  
  if (!pTransport->connect(hostname, port))
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

  m_pTransport = pTransport;
  m_pProtocolHandler = pProtocolHandler;
  return true;
}

void VisionaryControl::close()
{
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
}

bool VisionaryControl::login(CoLaUserLevel::Enum userLevel, const char* password)
{
  CoLaCommand loginCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "SetAccessMode").parameterSInt(userLevel).parameterPasswordMD5(password).build();
  CoLaCommand loginResponse = sendCommand(loginCommand);

  if (loginResponse.getError() == CoLaError::OK)
  {
    return CoLaParameterReader(loginResponse).readBool();
  }
  return false;
}

bool VisionaryControl::logout()
{
  CoLaCommand runCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "Run").build();
  CoLaCommand runResponse = sendCommand(runCommand);
  
  if (runResponse.getError() == CoLaError::OK)
  {
    return CoLaParameterReader(runResponse).readBool();
  }
  return false;
}

std::string VisionaryControl::receiveCoLaResponse()
{
  
  if (!syncCoLa())
  {
    return "false";
  }

  //-----------------------------------------------
  // Fill Buffer with data
  std::vector<char> buffer;

  if (!receiveData(sizeof(uint32_t), buffer))
  {
    return "false";
  }

  const uint32_t length = readUnalignBigEndian<uint32_t>(buffer.data());

  uint32_t bytesToReceive = length + 1;    // packetlength is only the data without STx, Packet Length and Checksum

  if (!receiveData(bytesToReceive, buffer))
  {
    return "false";
  }

  return (buffer.data());
}

CoLaCommand VisionaryControl::receiveCoLaCommand()
{
  if (!syncCoLa())
  {
    return CoLaCommand::networkErrorCommand();
  }

  //-----------------------------------------------
  // Fill Buffer with data
  std::vector<uint8_t> buffer;

  if (!receiveData(sizeof(uint32_t), buffer))
  {
    return CoLaCommand::networkErrorCommand();
  }

  const uint32_t length = readUnalignBigEndian<uint32_t>(buffer.data());

  uint32_t bytesToReceive = length + 1;    // packetlength is only the data without STx, Packet Length and Checksum

  if (!receiveData(bytesToReceive, buffer))
  {
    return CoLaCommand::networkErrorCommand();
  }

  // Add back magic word and length
  const uint32_t bigEndianLength = nativeToBigEndian(length);
  buffer.insert(buffer.begin(), reinterpret_cast<const uint8_t*>(&bigEndianLength), reinterpret_cast<const uint8_t*>(&bigEndianLength) + 4);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);

  return CoLaCommand(buffer);
}

bool VisionaryControl::startAcquisition() 
{
#if 0
  // example
  CoLaCommand myCmd = prepareCall("PLAYSTART");
  ColaParameterWriter(myCmd).uint8(56).flexString16("Hallo");

  ColaParameterWriter myColaParameterWriter(myCmd);
  myColaParameterWriter.uint8(56);
  myColaParameterWriter.flexString16("Yoohoo");

  myCmdResponse = send(myCmd);

  uint8 errorCode;
  uint32 nItems;
  CoLaParameterReader(myCmdResponse).uint8(errorCode).uint32(nItems);
#else
//old
  CoLaCommand startCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "PLAYSTART").build();
  CoLaCommand response = sendCommand(startCommand);
#endif
  return response.getError() == CoLaError::OK;
}

bool VisionaryControl::stepAcquisition() 
{
  CoLaCommand stepCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "PLAYNEXT").build();
  CoLaCommand response = sendCommand(stepCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControl::stopAcquisition() 
{
  CoLaCommand stopCommand = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "PLAYSTOP").build();
  CoLaCommand response = sendCommand(stopCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControl::getDataStreamConfig() 
{
  CoLaCommand command = CoLaParameterWriter(CoLaCommandType::METHOD_INVOCATION, "GetBlobClientConfig").build();
  CoLaCommand response = sendCommand(command);

  return response.getError() == CoLaError::OK;
}

CoLaCommand VisionaryControl::sendCommand(CoLaCommand & command)
{
  write(command.getBuffer());

  CoLaCommand response = receiveCoLaCommand();

  return response;
}
