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

#include "VisionaryControlOld.h"

#include "VisionaryEndian.h"

#include "CoLaBCommandBuilder.h"
#include "CoLaBCommandReader.h"

VisionaryControlOld::VisionaryControlOld() :
  Network(DEFAULT_PORT)
{
}

VisionaryControlOld::VisionaryControlOld(unsigned long ipAddress) :
  Network(ipAddress, DEFAULT_PORT)
{
}

VisionaryControlOld::VisionaryControlOld(unsigned long ipAddress, unsigned short port) :
  Network(ipAddress, port)
{
}

VisionaryControlOld::~VisionaryControlOld()
{
}

bool VisionaryControlOld::login(CoLaUserLevel::Enum userLevel, const char* password)
{
  CoLaBCommand loginCommand = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "SetAccessMode").parameterSInt(userLevel).parameterPasswordMD5(password).build();
  CoLaBCommand loginResponse = sendCommand(loginCommand);

  if (loginResponse.getError() == CoLaError::OK)
  {
    return CoLaBCommandReader(loginResponse).readBool();
  }
  return false;
}

bool VisionaryControlOld::logout()
{
  CoLaBCommand runCommand = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "Run").build();
  CoLaBCommand runResponse = sendCommand(runCommand);
  
  if (runResponse.getError() == CoLaError::OK)
  {
    return CoLaBCommandReader(runResponse).readBool();
  }
  return false;
}

std::string VisionaryControlOld::receiveCoLaResponse()
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

CoLaBCommand VisionaryControlOld::receiveCoLaBCommand()
{
  if (!syncCoLa())
  {
    return CoLaBCommand::networkErrorCommand();
  }

  //-----------------------------------------------
  // Fill Buffer with data
  std::vector<uint8_t> buffer;

  if (!receiveData(sizeof(uint32_t), buffer))
  {
    return CoLaBCommand::networkErrorCommand();
  }

  const uint32_t length = readUnalignBigEndian<uint32_t>(buffer.data());

  uint32_t bytesToReceive = length + 1;    // packetlength is only the data without STx, Packet Length and Checksum

  if (!receiveData(bytesToReceive, buffer))
  {
    return CoLaBCommand::networkErrorCommand();
  }

  // Add back magic word and length
  const uint32_t bigEndianLength = nativeToBigEndian(length);
  buffer.insert(buffer.begin(), reinterpret_cast<const uint8_t*>(&bigEndianLength), reinterpret_cast<const uint8_t*>(&bigEndianLength) + 4);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);
  buffer.insert(buffer.begin(), 0x02);

  return CoLaBCommand(buffer);
}

bool VisionaryControlOld::startAcquisition() 
{
  CoLaBCommand startCommand = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "PLAYSTART").build();
  CoLaBCommand response = sendCommand(startCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControlOld::stepAcquisition() 
{
  CoLaBCommand stepCommand = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "PLAYNEXT").build();
  CoLaBCommand response = sendCommand(stepCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControlOld::stopAcquisition() 
{
  CoLaBCommand stopCommand = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "PLAYSTOP").build();
  CoLaBCommand response = sendCommand(stopCommand);

  return response.getError() == CoLaError::OK;
}

bool VisionaryControlOld::getDataStreamConfig() 
{
  CoLaBCommand command = CoLaBCommandBuilder(CoLaCommandType::METHOD_INVOCATION, "GetBlobClientConfig").build();
  CoLaBCommand response = sendCommand(command);

  return response.getError() == CoLaError::OK;
}

CoLaBCommand VisionaryControlOld::sendCommand(CoLaBCommand & command)
{
  write(command.getBuffer());

  CoLaBCommand response = receiveCoLaBCommand();

  return response;
}
