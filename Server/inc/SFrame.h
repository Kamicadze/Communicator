#pragma once
#include <cstdint>

struct Frame
{
	char m_CID[12]; ///login of sending client
	char m_DCID[12]; ///destiantion client login
//	unsigned char m_length; i think it's useless
	uint8_t m_dataType;
//	unsigned char m_parts;  same with thoes two
//	unsigned char m_partNumber;
	uint32_t m_sourceAddress;
	uint8_t m_sourcePort;
	uint32_t m_destenationAddress;
	uint8_t m_destenationPort;
//	unsigned char m_messageLength;
	char m_messageData[150];
	uint8_t m_CRC;


};
