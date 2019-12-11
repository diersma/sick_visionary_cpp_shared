//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: November 2019
// 
// @author:  Andreas Richert
// @author:  Marco Dierschke
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de

#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>
#include "VisionaryData.h"
#include "Network.h"

class VisionaryDataStream: public Network
{
public:
  VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler);
  VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler, unsigned long ipAddress);
  VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler, unsigned long ipAddress, unsigned short port);
  ~VisionaryDataStream();

  //-----------------------------------------------
  // Receive a single blob from the connected device and store it in buffer.
  // Returns true when valid frame completely received.
  bool getNextFrame();
private:
  // Network byte ordered long value for default data stream port: 2114
  static const long DEFAULT_PORT = 0x4208;


  boost::shared_ptr<VisionaryData> m_dataHandler;

  // Parse the Segment-Binary-Data (Blob data without protocol version and packet type).
  // Returns true when parsing was successful.
  bool parseSegmentBinaryData(const std::vector<char>::iterator itBuf);
};
