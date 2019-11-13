#ifndef ITRANSPORT_H_INCLUDED
#define ITRANSPORT_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

class ITransport
{
public:
	virtual ~ITransport() {}; // destructor, use it to call destructor of the inherit classes

  virtual int connect(const std::string& hostname, uint16_t port) = 0;
  virtual int shutdown() = 0;

  /// Send data on socket to device
  ///
  /// \e All bytes are sent on the socket. It is regarded as error if this is not possible.
  ///
  /// \param[in] buffer buffer containing the bytes that shall be sent.
  ///
  /// \return OS error code.
  virtual int send(const std::vector<std::uint8_t> &buffer) = 0;

  /// Send data on socket to device
  ///
  /// \e All bytes are sent on the socket. It is regarded as error if this is not possible.
  ///
  /// \param[in] buffer buffer containing the bytes that shall be sent.
  /// \param[in] maxBytesToReceive maximum number of bytes to receive.
  /// \param[in] minBytesToReceive minimum number of bytes to receive (if omitted, its ok if no bytes are received).
  ///
  /// \return OS error code.
  virtual int recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive, std::size_t minBytesToReceive=0u) = 0;
  
};

#endif /* ITRANSPORT_H_INCLUDED */
