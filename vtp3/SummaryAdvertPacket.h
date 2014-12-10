/*----------------------------------------------------------------------------
 * SummaryAdvertPacket.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef SUMMARYADVERTPACKET_H_
#define SUMMARYADVERTPACKET_H_

#include <ostream>

#include "common.h"
#include "Connection.h"
#include "VTP3CommonHeader.h"

namespace VTP3
{

#define TIMESTAMP_LENGTH 12
#define MD5_LENGTH 16

	/*
	 *
	 */
	struct SummaryAdvertPacket {
			VTP3CommonHeader header;
			uint32_t revision_nr;
			uint32_t updater_id;
			uint8_t update_timestamp[TIMESTAMP_LENGTH];
			uint8_t md5_digest[MD5_LENGTH];

			int send(Connection const& connection);
	};

	std::ostream& operator<<(std::ostream& os, SummaryAdvertPacket const& pkt);

} /* namespace VTP3 */

#endif /* SUMMARYADVERTPACKET_H_ */
