/*----------------------------------------------------------------------------
 * AdvertRequestPacket.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef ADVERTREQUESTPACKET_H_
#define ADVERTREQUESTPACKET_H_

#include <ostream>

#include "common.h"
#include "Connection.h"
#include "VTP3CommonHeader.h"

namespace VTP3
{
	struct AdvertRequestPacket {
		VTP3CommonHeader header;
		uint32_t start;

		int send(Connection const& connection);
	};

	std::ostream& operator<<(std::ostream& os, AdvertRequestPacket const& pkt);
} /* namespace VTP3 */

#endif /* ADVERTREQUESTPACKET_H_ */
