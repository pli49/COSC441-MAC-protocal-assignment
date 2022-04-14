cplusplus {{
#include "MacPacketType_m.h"
}}

enum MacPacketType;

packet MacPacket
{
    int    receiverAddress;
    int    transmitterAddress;
    int    macPacketType @enum(MacPacketType);
}