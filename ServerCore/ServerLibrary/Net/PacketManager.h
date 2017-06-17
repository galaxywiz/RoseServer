#pragma once
#include "stdafx.h"

class Packet
{
	void encode()
	{
		

	}

	void decode()
	{

	}
};

class PacketManager
{
	ThreadJobQueue<Packet *> packetQueue_;

public:
	bool inputPacket(Packet *packet)
	{
		packetQueue_.push(packet);
	}

	// 큐에서 하나 뽑아서 워커에게 나눠준다.
	Packet *getPacket()
	{
		Packet *packet;
		if (!packetQueue_.pop(packet)) {
			packetQueue_.swap();
			return nullptr;
		}
		return packet;
	}
};