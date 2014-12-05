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


#include "common.h"
#include "Connection.h"

namespace VTP3
{

#define TIMESTAMP_LENGTH 12
#define MD5_LENGTH 16

	/*
	 *
	 */
	struct SummaryAdvertPacket {
			VTP3CommonHeader header;
			uint32_t revision;
			uint32_t updater_id;
			uint8_t update_timestamp[TIMESTAMP_LENGTH];
			uint8_t md5[MD5_LENGTH];

			int send(Connection const& connection);
	};

} /* namespace VTP3 */

#endif /* SUMMARYADVERTPACKET_H_ */
