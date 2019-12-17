#include "CoLaBProtocolHandler.h"
#include "VisionaryEndian.h"


CoLaBProtocolHandler::CoLaBProtocolHandler(ITransport& rTransport)
: m_rTransport(rTransport)
{
}

CoLaBProtocolHandler::~CoLaBProtocolHandler()
{
}

bool CoLaBProtocolHandler::openSession(uint32_t/*sessionTimeout _ms*/)
{
  // we don't have a session id byte in CoLaB protocol. Nothing to do here.
  return true;
}

void CoLaBProtocolHandler::closeSession()
{
  // we don't have a session id byte in CoLaB protocol. Nothing to do here.
}

CoLaCommand CoLaBProtocolHandler::send(CoLaCommand cmd)
{
  //
  // convert cola cmd to vector buffer and add/fill header
  //

  std::vector<std::uint8_t> buffer;
  buffer = cmd.getBuffer();

  // insert magic bytes
  const uint8_t MAGIC_BYTE = 0x02;
  // inserts 8 bytes at front (Magic Bytes and length)
  auto it = buffer.begin();
  for (uint8_t i = 0; i < 8; i++)
  {
    it = buffer.insert(it, MAGIC_BYTE);
  }
  // Overwrite length
  *reinterpret_cast<uint32_t*>(&buffer[4]) = nativeToBigEndian(static_cast<uint32_t>(buffer.size()) - 8);
  
  // Add checksum to end
  buffer.insert(buffer.end(), calculateChecksum(buffer));

  //
  // send to socket
  //
  
  m_rTransport.send(buffer);
  buffer.clear();

  //
  // get response
  //
  
  m_rTransport.recv(buffer, sizeof(uint32_t));
  // check for magic bytes
  const std::vector<uint8_t> MagicBytes = { 0x02, 0x02, 0x02, 0x02 };
  bool result = std::equal(MagicBytes.begin(), MagicBytes.end(), buffer.begin());
  if (result)
  {
    // get length
    m_rTransport.recv(buffer, sizeof(uint32_t));
    const uint32_t length = readUnalignBigEndian<uint32_t>(buffer.data());
    m_rTransport.recv(buffer, length);
  }
  else
  {
    // drop invalid data
    buffer.clear();
  }
  CoLaCommand response(buffer);
  return response;
}

uint8_t CoLaBProtocolHandler::calculateChecksum(const std::vector<uint8_t>& buffer)
{
  uint8_t checksum = 0;
  for (size_t i = 8; i < buffer.size(); i++)
  {
    checksum ^= buffer[i];
  }
  return checksum;
}
