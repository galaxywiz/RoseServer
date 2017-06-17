#pragma once
#include "stdafx.h"

class PacketAnalyzer : public Singleton < PacketAnalyzer >
{
public:
	Packet* analyzer(const char *rowPacket, size_t size);
};