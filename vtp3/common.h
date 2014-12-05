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

#define MAX_DOMAIN_LENGTH 32
#define ORG_CODE_LENGTH 3
#define MAC_LENGTH 6


	struct EthernetHeader {
		uint8_t dst_mac[MAC_LENGTH];
		uint8_t src_mac[MAC_LENGTH];
		union {
			uint16_t type;
			uint16_t length;
		};
	};



	struct LLCHeader {
		uint8_t dsap;
		uint8_t ssap;
		uint8_t ctrl;
		uint8_t org_code[ORG_CODE_LENGTH];
		uint16_t pid; /* protocol id */
	};

	/**
	 * Can be memcped, because all members are made of bytes
	 */
	struct VTP3CommonHeader {
		uint8_t version;
		uint8_t code;
		union {
			uint8_t followers;
			uint8_t sequence_number;
			uint8_t reserved;
		};
		uint8_t domain_length;
		uint8_t doman_name[MAX_DOMAIN_LENGTH];
	};

	struct VlanInfoExtended {
			uint8_t type; 	/* Captured packets have this field 1 byte long... but cisco says it should be 2 */
			uint8_t length; /* this too */
			/* space[length] */
	};

}


#endif /* COMMON_H_ */
