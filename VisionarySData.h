//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: August 2017
// 
// @author:  Andreas Richert
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de
// 
// Last commit: $Date: 2017-12-14 16:56:05 +0100 (Do, 14 Dez 2017) $
// Last editor: $Author: richean $
// 
// Version "$Revision: 15262 $"
//

#pragma once

#include "VisionaryData.h"

class VisionarySData : public VisionaryData
{
public:
  VisionarySData();
  ~VisionarySData();

  //-----------------------------------------------
  // Getter Functions
  const std::vector<uint16_t>& getZMap() const;
  const std::vector<uint32_t>& getRGBAMap() const;
  const std::vector<uint16_t>& getConfidenceMap() const;
  // Calculate and return the Point Cloud in the camera perspective. Units are in meters.
  void generatePointCloud(std::vector<PointXYZ> &pointCloud) override;

protected:
  //-----------------------------------------------
  // functions for parsing received blob

  // Parse the XML Metadata part to get information about the sensor and the following image data.
  // This function uses boost library. An other XML parser is needed to remove boost from source.
  // Returns true when parsing was successful.
  bool parseXML(const std::string & xmlString, uint32_t changeCounter);

  // Parse the Binary data part to extract the image data. 
  // Returns true when parsing was successful.
  bool parseBinaryData(std::vector<char>::iterator itBuf, size_t length);

private:
  /// Byte depth of images
  int m_zByteDepth, m_rgbaByteDepth, m_confidenceByteDepth;

  // Pointers to the image data
  std::vector<uint16_t> m_zMap;
  std::vector<uint32_t> m_rgbaMap;
  std::vector<uint16_t> m_confidenceMap;
};

