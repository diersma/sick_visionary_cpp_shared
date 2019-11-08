#pragma once
#include "IProtocolHandler.h"
#include "ITransport.h"

class CoLaBProtocolHandler :
  public IProtocolHandler
{
  CoLaBProtocolHandler(ITransport& rTransport);
};

