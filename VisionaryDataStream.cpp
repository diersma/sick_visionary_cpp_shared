//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: August 2017
// 
// @author:  Andreas Richert
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de
// 
// Last commit: $Date: 2017-12-06 16:56:03 +0100 (Mi, 06 Dez 2017) $
// Last editor: $Author: richean $
// 
// Version "$Revision: 15144 $"
//

#include "VisionaryDataStream.h"
#include "VisionaryEndian.h"
#include <cstring>
#include <stdio.h>

VisionaryDataStream::VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler) :
  m_dataHandler(dataHandler)
{
}

VisionaryDataStream::VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler, unsigned long ipAddress) :
  m_dataHandler(dataHandler)
{
}

VisionaryDataStream::VisionaryDataStream(boost::shared_ptr<VisionaryData> dataHandler, unsigned long ipAddress, unsigned short port) :
  m_dataHandler(dataHandler)
{
}

VisionaryDataStream::~VisionaryDataStream()
{
}

bool VisionaryDataStream::getNextFrame()
{
  return false;
}

bool VisionaryDataStream::parseSegmentBinaryData(std::vector<char>::iterator itBuf)
{
  bool result = false;
  std::vector<char>::iterator itBufSegment = itBuf;

  //-----------------------------------------------
  // Extract informations in Segment-Binary-Data
  //const uint16_t blobID = readUnalignBigEndian<uint16_t>(&*itBufSegment);
  itBufSegment += sizeof(uint16_t);
  const uint16_t numSegments = readUnalignBigEndian<uint16_t>(&*itBufSegment);
  itBufSegment += sizeof(uint16_t);

  //offset and changedCounter, 4 bytes each per segment
  std::vector<uint32_t> offset(numSegments);
  std::vector<uint32_t> changeCounter(numSegments);
  for (int i = 0; i < numSegments; i++)
  {
    offset[i] = readUnalignBigEndian<uint32_t>(&*itBufSegment);
    itBufSegment += sizeof(uint32_t);
    changeCounter[i] = readUnalignBigEndian<uint32_t>(&*itBufSegment);
    itBufSegment += sizeof(uint32_t);
  }

  //-----------------------------------------------
  // First segment contains the XML Metadata
  uint32_t xmlSegmentSize = offset[1] - offset[0];
  std::string xmlSegment(&*(itBuf + offset[0]), xmlSegmentSize);

  if (m_dataHandler->parseXML(xmlSegment, changeCounter[0]))
  {
    //-----------------------------------------------
    // Second segment contains Binary data
    size_t binarySegmentSize = offset[2] - offset[1];
    result = m_dataHandler->parseBinaryData((itBuf + offset[1]), binarySegmentSize);
  }
  return result;
}
