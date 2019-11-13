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

ControlSession::ControlSession()
{
// TODO Auto-generated constructor stub

}

ControlSession::~ControlSession()
{
// TODO Auto-generated destructor stub

}

CoLaCommand ControlSession::prepareRead(const std::string& varname)
{
  #if 0
  std::vector<uint8_t> buffer;
  CoLaCommand cmd(buffer);
  TODO
#endif

}

CoLaCommand ControlSession::prepareWrite(const std::string& varname)
{
}

CoLaCommand ControlSession::prepareCall(const std::string& varname)
{
}


//---------------------------------------------------------------------------------------
