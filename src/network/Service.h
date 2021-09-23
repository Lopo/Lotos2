#ifndef LOTOSPP_NETWORK_SERVICE_H
#define LOTOSPP_NETWORK_SERVICE_H


#include "config.h"

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "network/ServiceBase.h"


namespace lotospp {
	namespace network {
		class Protocol;
		class Connection;
		typedef boost::shared_ptr<Connection> Connection_ptr;

// The Service class is very thin, it's only real job is to create dynamic dispatch of the protocol attributes,
// which would otherwise be very hard, and require templating of many Service functions

template <typename ProtocolType>
class Service
	: public ServiceBase
{
public:
	bool isSingleSocket() const { return ProtocolType::server_sends_first;};
	const char* getProtocolName() const { return ProtocolType::protocolName();};

	Protocol* makeProtocol(Connection_ptr c) const { return new ProtocolType(c);};
};

	} // namespace network
} // namespace lotospp

#endif // LOTOSPP_NETWORK_SERVICE_H
