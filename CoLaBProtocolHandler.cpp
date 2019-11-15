#include "CoLaBProtocolHandler.h"

CoLaBProtocolHandler::CoLaBProtocolHandler(ITransport& rTransport)
{
}

CoLaBProtocolHandler::~CoLaBProtocolHandler()
{
}

bool CoLaBProtocolHandler::openSession(uint32_t/*sessionTimeout*/_ms)
{
  return true;
}

void CoLaBProtocolHandler::closeSession()
{
}

