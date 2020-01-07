//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: May 2019
// 
// @author:  Patrick Ebner
// @author:  Andreas Richert
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de

#pragma once

#include <cstring>
#include <string>

class VisionaryAutoIPScan
{
public:
  struct DeviceInfo
  {
    std::string DeviceName;
    std::string MacAddress;
    std::string IpAddress;
    std::string SubNet;
    std::string Port;
  };
  VisionaryAutoIPScan();
  VisionaryAutoIPScan(unsigned short port);
  VisionaryAutoIPScan(unsigned long ipAddress, unsigned short port);

  ~VisionaryAutoIPScan();

  /// <summary>
  /// Runs an autoIP scan and returns a list of devices
  /// </summary>
  /// <returns>A list of devices.</returns>
  std::vector<DeviceInfo> doScan(int timeOut);

private:
  DeviceInfo parseAutoIPXml(std::stringstream& rStringStream);
  static const short DEFAULT_PORT = 30718;
};
