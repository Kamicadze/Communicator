#pragma once
#include <cstdint>

struct SFrame
{
    char m_CID[12]; ///login of sending client
    char m_DCID[12]; ///destiantion client login
    uint8_t m_dataType;
    uint32_t m_sourceAddress;
    uint8_t m_sourcePort;
    uint32_t m_destenationAddress;
    uint8_t m_destenationPort;
    char m_messageData[150];
    uint8_t m_CRC;


};
