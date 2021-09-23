#ifndef LOTOSPP_NETWORK_SERVICEBASE_H
#define LOTOSPP_NETWORK_SERVICEBASE_H


#include "config.h"

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>


namespace lotospp {
	namespace network {
		class Protocol;
		class Connection;
		typedef boost::shared_ptr<Connection> Connection_ptr;

class ServiceBase
	: boost::noncopyable
{
public:
	virtual ~ServiceBase() {}; // Redundant, but stifles compiler warnings

	virtual bool isSingleSocket() const=0;
	virtual const char* getProtocolName() const=0;

	virtual Protocol* makeProtocol(Connection_ptr c) const=0;
};

typedef boost::shared_ptr<ServiceBase> Service_ptr;

	} // namespace network
} // namespace lotospp

#endif // LOTOSPP_NETWORK_SERVICEBASE_H
