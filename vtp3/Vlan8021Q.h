/*----------------------------------------------------------------------------
 * Vlan8021Q.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef VLAN8021Q_H_
#define VLAN8021Q_H_

#include <cstdint>


namespace VTP3
{

	struct Vlan8021Q {
		union {
				uint32_t tci;
				uint8_t bytearray[4];
		};
		union {
				uint16_t length;
				uint16_t type;
		};

		void pack(Vlan8021Q const* vlan);

		uint8_t set_pcp(uint8_t pcp);
		uint8_t set_cfi(uint8_t cfi);
		uint16_t set_vlan_id(uint16_t vlan_id);
	};

} /* namespace VTP3 */

#endif /* VLAN8021Q_H_ */
