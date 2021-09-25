#ifndef LOTOSPP_NETWORK_NETWORKMESSAGE_H
#define LOTOSPP_NETWORK_NETWORKMESSAGE_H

#include "config.h"
#include "ProtocolConst.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <cstdint>


namespace LotosPP::Network {

class NetworkMessage
{
public:
	enum {
		max_body_length=NETWORKMESSAGE_MAXSIZE
		};

	// constructor/destructor
	NetworkMessage();
	virtual ~NetworkMessage();

	// simply read functions for incoming message
	uint8_t GetByte();
	uint16_t GetU16();
	uint32_t GetU32();
	uint32_t PeekU32() const;
	uint64_t GetU64() const;
	std::string GetString();
	std::string GetRaw();
	uint8_t GetAt(uint32_t pos);
	uint8_t operator[](uint32_t pos)
	{
		return GetAt(pos);
	};

	// skips count unknown/unused bytes in an incoming message
	NetworkMessage* SkipBytes(int count);

	// simply write functions for outgoing message
	NetworkMessage* AddByte(uint8_t value);
	NetworkMessage* AddU16(uint16_t value);
	NetworkMessage* AddU32(uint32_t value);
	NetworkMessage* AddU64(uint64_t value);
	NetworkMessage* AddBytes(const char* bytes, uint32_t size);

	NetworkMessage* AddString(const std::string& value);
	NetworkMessage* AddString(const char* value);

	std::size_t getMessageLength() const;
	void setMessageLength(int32_t newSize);
	std::size_t getReadPos() const;
	void setReadPos(size_t pos);

	char* getBuffer();

#ifdef __TRACK_NETWORK__
	virtual void Track(const std::string& file, long line, const std::string& func)
	{};
	virtual void clearTrack()
	{};
#endif

protected:
	void Reset();
	bool canAdd(uint32_t size) const;

	std::size_t m_MsgSize{0};
	std::size_t m_ReadPos{0};

	uint8_t m_MsgBuf[NETWORKMESSAGE_MAXSIZE];
};

typedef boost::shared_ptr<NetworkMessage> NetworkMessage_ptr;

	}

#endif
