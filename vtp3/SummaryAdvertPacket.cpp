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
	pkt.revision_nr = htonl( this->revision_nr );
	pkt.updater_id = htonl( this->updater_id );

	return VTP3::send(connection, &pkt, sizeof(*this));
}

std::ostream& VTP3::operator<<(std::ostream& os, SummaryAdvertPacket const& pkt)
{
	os << pkt.header << "\n"
		<< "******************* Summary Advert Packet *********************\n"
		<< "\tRevision:\t" << pkt.revision_nr << "\n"
		<< "\tUpdater id:\t" << pkt.updater_id << "\n"
		<< "\tTimestamp:\t" << reinterpret_cast<const char*>(pkt.update_timestamp) << "\n"
		<< "\tMD5 digest:\t";

	for (int i = 0; i < MD5_LENGTH; ++i) {
		os << pkt.md5_digest[i];
	}

	os << "\n";

	return os;
}
