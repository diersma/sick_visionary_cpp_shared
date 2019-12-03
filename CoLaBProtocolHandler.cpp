#include "CoLaBProtocolHandler.h"

CoLaBProtocolHandler::CoLaBProtocolHandler(ITransport& rTransport)
{
  // ToDo: Remember rTransport
}

CoLaBProtocolHandler::~CoLaBProtocolHandler()
{
}

bool CoLaBProtocolHandler::openSession(uint32_t/*sessionTimeout*/_ms)
{
  // ToDo: generate session ID and remember it
  return true;
}

void CoLaBProtocolHandler::closeSession()
{
  // ToDo: clear session ID
}

CoLaCommand CoLaBProtocolHandler::send(CoLaCommand cmd)
{
  // ToDo: convert cola cmd to vector buffer and add/fill header
  // send to socket and get response
  // convert response buffer to cola cmd and return it
  return cmd;
}

