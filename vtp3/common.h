/*----------------------------------------------------------------------------
 * common.h
 *
 *  Created on: 4. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <memory>


namespace VTP3 {

#define ORG_CODE_LENGTH 3
#define MAC_LENGTH 6

	enum MessageType {
		SMMARY_ADVERT = 0x01,
		SUBSET_ADVERT = 0x02,
		ADVERT_REQUEST = 0x03
	};

	struct EthernetHeader {
		uint8_t dst_mac[MAC_LENGTH];
		uint8_t src_mac[MAC_LENGTH];
		union {
			uint16_t type;
			uint16_t length;
		};
	};


	/* SNAP encapsulated in LLC */
	struct LLCHeader {
		uint8_t dsap;	/* SNAP, 1B field*/
		uint8_t ssap;	/* SNAP, 1B field*/
		uint8_t ctrl;	/* SNAP, 1B field*/
		uint8_t org_code[ORG_CODE_LENGTH];	/* SNAP, 3B field called OUI - organizationally unique identifier - cisco */
		uint16_t pid; /* protocol id */		/* SNAP, 2B field called TYPE - 0x2003 for VTP */
	};


	struct VlanInfoExtended {
			uint8_t type; 	/* Captured packets have this field 1 byte long... but cisco says it should be 2 */
			uint8_t length; /* this too */
			/* space[length] */
	};

}


#endif /* COMMON_H_ */
