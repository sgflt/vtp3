/*----------------------------------------------------------------------------
 * EthernetHeader.cpp
 *
 *  Created on: 9. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "EthernetHeader.h"

#include <ostream>
#include <arpa/inet.h>
#include <netinet/ether.h>

namespace VTP3
{
	std::ostream& operator<<(std::ostream& os, EthernetHeader const& eh)
	{
		os << "************************** Ethernet ***************************\n";
		os << "SRC:\t"
				<< std::hex
				<< static_cast<int>(eh.src_mac[0]) << ":"
				<< static_cast<int>(eh.src_mac[1]) << ":"
				<< static_cast<int>(eh.src_mac[2]) << ":"
				<< static_cast<int>(eh.src_mac[3])<< ":"
				<< static_cast<int>(eh.src_mac[4]) << ":"
				<< static_cast<int>(eh.src_mac[5]) << "\n";

		os << "DST:\t"
				<< static_cast<int>(eh.dst_mac[0]) << ":"
				<< static_cast<int>(eh.dst_mac[1]) << ":"
				<< static_cast<int>(eh.dst_mac[2]) << ":"
				<< static_cast<int>(eh.dst_mac[3]) << ":"
				<< static_cast<int>(eh.dst_mac[4]) << ":"
				<< static_cast<int>(eh.dst_mac[5]) << "\n";


		if ( ntohs(eh.length) > ETH_FRAME_LEN ) {
			os << "Type:\t" << ntohs(eh.length);
		}
		else {
			os << "Length:\t" << std::dec << ntohs(eh.length) << " B";
		}
		return os;
	}

} /* namespace VTP3 */
