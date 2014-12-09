/*----------------------------------------------------------------------------
 * Connection.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <cstdint>

#include "common.h"
#include "Vlan8021Q.h"
#include "EthernetHeader.h"

namespace VTP3
{
	struct Connection {
			int sockfd;
			std::string if_name;
			uint8_t dst_mac[MAC_LENGTH] = {0x01, 0x00, 0x0c, 0xcc, 0xcc, 0xcc};
			std::shared_ptr<VTP3::EthernetHeader> ethernet_header;
			std::shared_ptr<VTP3::Vlan8021Q> vlan8021Q_header;

			int socket(void);
			void create_header(void);
	};
}


#endif /* CONNECTION_H_ */
