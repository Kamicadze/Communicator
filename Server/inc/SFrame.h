#include <cstdint>

struct Frame
{
	uint8_t m_CID;
	uint8_t m_DCID; ///destiantion client id
	unsigned char m_length;
	unsigned char m_dataType;
	unsigned char m_parts;
	unsigned char m_partNumber;
	uint32_t m_sourceAddress;
	uint8_t m_soucrePort;
	uint32_t m_destenationAddress;
	uint8_t m_destenationPort;
	unsigned char m_messageLength;
	unsigned char m_messageData[150];
	uint8_t m_CRC;


	Frame();
	void parseFrame();
	void createFrame();
};
