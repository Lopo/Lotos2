#ifndef LOTOSPP_NETWORK_PROTOCOL_H
#define	LOTOSPP_NETWORK_PROTOCOL_H

#include "config.h"
#include "Connection.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/asio/ip/address.hpp>
#include <cstdint>
#ifdef OS_OPENBSD
#	include <unistd.h> // OpenBSD5.9+boost1.58
#endif


namespace LotosPP {
	namespace Common {
		class User;
		}
	namespace Network {
		typedef boost::shared_ptr<Connection> Connection_ptr;
		class NetworkMessage;
		class OutputMessage;
		typedef boost::shared_ptr<OutputMessage> OutputMessage_ptr;

class Protocol
	: boost::noncopyable
{
public:
	Protocol(Connection_ptr connection)
		: m_connection{connection}
	{};
	virtual ~Protocol()
	{};

	virtual void parsePacket(NetworkMessage& msg)=0;

	void onSendMessage(OutputMessage_ptr msg);
	void onRecvMessage(NetworkMessage& msg);
	virtual void onRecvFirstMessage(NetworkMessage& msg)=0;
	virtual void onConnect() {}; // Used to send first packet to client

	Connection_ptr getConnection()
	{
		return m_connection;
	};
	const Connection_ptr getConnection() const
	{
		return m_connection;
	};
	void setConnection(Connection_ptr connection)
	{
		m_connection=connection;
	};
	bool logout(bool forced);

	boost::asio::ip::address getAddress() const;
	u_short getPort() const;
	std::string getHostname() const;
	int32_t addRef()
	{
		return ++m_refCount;
	};
	int32_t unRef()
	{
		return --m_refCount;
	};

	void setUser(LotosPP::Common::User* p);

	virtual void write(const std::string& str);

	void parseDebug(NetworkMessage& msg);

protected:
	// Tell telnet to echo characters
	virtual void sendEchoOn()
	{};
	// Tell telnet not to echo characters - for password entry etc.
	virtual void sendEchoOff()
	{};

protected:
	boost::asio::io_service& getIoService()
	{
		return m_connection->getIoService();
	};
	//Use this function for autosend messages only
	OutputMessage_ptr getOutputBuffer();

	virtual void releaseProtocol();
	virtual void deleteProtocolTask();
	virtual void disconnect();

	LotosPP::Common::User* user{nullptr};

	friend class Connection;
	friend class LotosPP::Common::User;

private:
	OutputMessage_ptr m_outputBuffer{nullptr};
	Connection_ptr m_connection{nullptr};
	uint32_t m_refCount{0};
};

	}}

#endif
