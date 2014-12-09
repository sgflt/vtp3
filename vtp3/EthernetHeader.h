/*----------------------------------------------------------------------------
 * EthernetHeader.h
 *
 *  Created on: 9. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef ETHERNETHEADER_H_
#define ETHERNETHEADER_H_

#include <ostream>

namespace VTP3
{
#define MAC_LENGTH 6

	struct EthernetHeader {
		uint8_t dst_mac[MAC_LENGTH];
		uint8_t src_mac[MAC_LENGTH];
		union {
			uint16_t type;
			uint16_t length;
		};
	};

	std::ostream& operator<<(std::ostream& os, EthernetHeader const& eh);

} /* namespace VTP3 */

#endif /* ETHERNETHEADER_H_ */
