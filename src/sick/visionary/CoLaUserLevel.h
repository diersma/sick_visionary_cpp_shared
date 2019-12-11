//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: October 2018
// 
// @author:  Johan Falk
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de

#pragma once

#include <stdint.h>

namespace CoLaUserLevel
{
  /// <summary>Available CoLa user levels.</summary>
  enum Enum
  {
    RUN = 0,
    OPERATOR = 1,
    MAINTENANCE = 2,
    AUTHORIZED_CLIENT = 3,
    SERVICE = 4
  };
}