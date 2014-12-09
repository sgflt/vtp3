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
#include <iostream>


namespace VTP3
{

	/* 802.1Q 32b header */
	struct Vlan8021Q {
		union {
				uint16_t tci; /* 1B, TCI - tag control information includes: PCP, CFI (DEI), VID */
				uint8_t bytearray[2];
		};
		union {
				uint16_t length;
				uint16_t type;	/* 2B, TPID - tag protocol identifier (0x8100 for 802.1Q) */
		};

		void pack(Vlan8021Q const* vlan);

		uint8_t get_pcp(void) const;
		uint8_t get_cfi(void) const;
		uint16_t get_vlan_id(void) const;

		uint8_t set_pcp(uint8_t pcp); /* 3b, PCP - Priority code point */
		uint8_t set_cfi(uint8_t cfi); /* 1b, CFI (also called DEI) */
		uint16_t set_vlan_id(uint16_t vlan_id); /* 12bit, VID - VLAN identifier (to which VLAN the frame belongs) */
	};

	std::ostream& operator<<(std::ostream& os, Vlan8021Q const& vlan);

} /* namespace VTP3 */

#endif /* VLAN8021Q_H_ */
