/*----------------------------------------------------------------------------
 * AdvertRequestPacket.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "AdvertRequestPacket.h"

#include <cstring>
#include <arpa/inet.h>

#include "VTP3CommonHeader.h"
#include "VTP3Output.h"

using namespace VTP3;

int AdvertRequestPacket::send(Connection const& connection)
{
	AdvertRequestPacket pkt;

	std::memcpy(&pkt.header, &this->header, sizeof(this->header));
	pkt.start = htonl( this->start );

	return VTP3::send(connection, &pkt, sizeof(*this));
}

std::ostream& VTP3::operator<<(std::ostream& os, AdvertRequestPacket const& pkt)
{
	os << pkt.header << "\n"
		<< "\tStart:\t" << pkt.start;
	return os;
}
