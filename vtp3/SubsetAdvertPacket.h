/*----------------------------------------------------------------------------
 * SubsetAdvertPacket.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef SUBSETADVERTPACKET_H_
#define SUBSETADVERTPACKET_H_

#include <cstring>
#include <vector>
#include <memory>
#include <arpa/inet.h>

#include "common.h"
#include "VlanInfo.h"
#include "Connection.h"

namespace VTP3
{

	struct SubsetAdvertPacket {
		VTP3CommonHeader header;
		uint32_t revision;

		int send(Connection const& connection, std::vector<std::shared_ptr<VlanInfo>> vlans);
	};

} /* namespace VTP3 */

#endif /* SUBSETADVERTPACKET_H_ */
