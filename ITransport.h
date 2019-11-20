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

  /// Send data on socket to device
  ///
  /// \e All bytes are sent on the socket. It is regarded as error if this is not possible.
  ///
  /// \param[in] buffer buffer containing the bytes that shall be sent.
  ///
  /// \return OS error code.
  virtual int send(const std::vector<std::uint8_t> &buffer) = 0;

  /// Receive data on socket to device
  ///
  /// Receive at most \a maxBytesToReceive bytes.
  ///
  /// \param[in] buffer buffer containing the bytes that shall be sent.
  /// \param[in] maxBytesToReceive maximum number of bytes to receive.
  ///
  /// \return OS error code.
  virtual int recv(std::vector<std::uint8_t>& buffer, std::size_t maxBytesToReceive) = 0;
  
  /// Read a number of bytes
  ///
  /// Contrary to recv this method reads precisely \a nBytesToReceive bytes.
  int read(std::vector<std::uint8_t>& buffer, std::size_t nBytesToReceive);

};

#endif /* ITRANSPORT_H_INCLUDED */
