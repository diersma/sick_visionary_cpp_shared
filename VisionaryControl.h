//
// Copyright note: Redistribution and use in source, with or without modification, are permitted.
// 
// Created: August 2017
// 
// @author:  Andreas Richert
// SICK AG, Waldkirch
// email: TechSupport0905@sick.de
// 
// Last commit: $Date: 2017-12-06 17:17:50 +0100 (Mi, 06 Dez 2017) $
// Last editor: $Author: richean $
// 
// Version "$Revision: 15145 $"
//

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "CoLaCommand.h"
#include "attic/CoLaUserLevel.h"
#include "IProtocolHandler.h"
#include "ITransport.h"
#include "ControlSession.h"


class VisionaryControl
{
public:
  enum ProtocolType
  {
    INVALID_PROTOCOL = -1,
    COLA_A,
    COLA_B,
    COLA_2
  };

  /// Default session timeout
  static const uint32_t kSessionTimeout_ms = 5000;

  VisionaryControl();
  ~VisionaryControl();


  /// Opens a connection to a Visionary sensor
  ///
  /// \param[in] type     protocol type the sensor understands (CoLa-A, CoLa-B or CoLa-2). 
  ///                     This information is found in the sensor documentation.
  /// \param[in] hostname name or IP address of the Visionary sensor.
  /// \param[in] port     control command port of the sensor, usually 2112 for CoLa-B or 2122 for CoLa-2.
  ///
  /// \retval true The connection to the sensor successfully was established.
  /// \retval false The connection attempt failed; the sensor is either
  ///               - switched off or has a different IP address or name
  ///               - not available using for PCs network settings (different subnet)
  ///               - the protocol type or the port did not match. Please check your sensor documentation.
  bool open(ProtocolType type, const std::string& hostname, std::uint16_t port, uint32_t sessionTimeout_ms = kSessionTimeout_ms);

  /// Close a connection
  ///
  /// Closes the control connection. It is allowed to call close of a connection
  /// that is not open. In this case this call is a no-op.
  void close();
    

  /// <summary>Login to the device.</summary>
  /// <param name="userLevel">The user level to login as.</param>
  /// <param name="password">Password for the selected user level.</param>
  /// <returns>True if login was successful, false otherwise.</returns>
  bool login(CoLaUserLevel::Enum userLevel, const char* password);

  /// <summary>Logout from the device.</summary>
  /// <returns>True if logout was successful, false otherwise.</returns>
  bool logout();

  /// <summary>
  /// Start streaming the data by calling the "PLAYSTART" method on the device. Works only when acquisition is stopped.
  /// </summary>
  /// <returns>True if successful, false otherwise.</returns>
  bool startAcquisition();

  /// <summary>
  /// Trigger a single image on the device. Works only when acquisition is stopped.
  /// </summary>
  /// <returns>True if successful, false otherwise.</returns>
  bool stepAcquisition();

  /// <summary>
  /// Stops the data stream. Works always, also when acquisition is already stopped before.
  /// </summary>
  bool stopAcquisition();

  /// <summary>
  /// Tells the device that there is a streaming channel by invoking a method named GetBlobClientConfig.
  /// </summary>
  /// <returns>True if successful, false otherwise.</returns>
  bool getDataStreamConfig();

  /// <summary>Send a <see cref="CoLaBCommand" /> to the device and waits for the result.</summary>
  /// <param name="command">Command to send</param>
  /// <returns>The response.</returns>
  CoLaCommand sendCommand(CoLaCommand& command);
  
private:
  std::string receiveCoLaResponse();
  CoLaCommand receiveCoLaCommand();

  // Network byte ordered short value for default configuration port: 2114
  static const short DEFAULT_PORT = 0x4008;

  std::unique_ptr<ITransport>       m_pTransport;
  std::unique_ptr<IProtocolHandler> m_pProtocolHandler;
  std::unique_ptr<ControlSession>   m_pControlSession;
};
