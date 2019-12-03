#pragma once
#include "VisionaryControl.h"

class AuthenticationLegacy:
  public IAuthentication
{
public:
  explicit AuthenticationLegacy(VisionaryControl& vctrl);
  virtual ~AuthenticationLegacy();

  virtual int login(UserLevel userLevel, const std::string& password);
  virtual int logout();

};

