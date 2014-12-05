/*----------------------------------------------------------------------------
 * SummaryAdvertPacket.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "SummaryAdvertPacket.h"

#include <cstring>
#include <arpa/inet.h>

#include "VTP3Output.h"

using namespace VTP3;

int SummaryAdvertPacket::send(Connection const& connection)
{
	SummaryAdvertPacket pkt;

	std::memcpy(&pkt.header, &this->header, sizeof(this->header));
	pkt.revision = htonl( this->revision );
	pkt.updater_id = htonl( this->updater_id );

	return VTP3::send(connection, &pkt, sizeof(*this));
}
