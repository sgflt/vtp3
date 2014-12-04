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
#define TIMESTAMP_LENGTH 12
#define MD5_LENGTH 16


	struct EthernetHeader {
		uint8_t dst_mac[MAC_LENGTH];
		uint8_t src_mac[MAC_LENGTH];
		union {
			uint16_t type;
			uint16_t length;
		};
	};

	struct Vlan8021Q {
		uint16_t vlan_proto_id;
		uint16_t pcp:3;
		uint16_t cfi:1;
		uint16_t vlan_id:12;
	};

	struct LLCHeader {
		uint8_t dsap;
		uint8_t ssap;
		uint8_t ctrl;
		uint8_t org_code[ORG_CODE_LENGTH];
		uint16_t pid; /* protocol id */
	};

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

	struct SummaryAdvertPacket {
		VTP3CommonHeader header;
		uint32_t revision;
		uint32_t updater_id;
		uint8_t update_timestamp[TIMESTAMP_LENGTH];
		uint8_t md5[MD5_LENGTH];
	};

	struct SubsetAdvertPacket {
		VTP3CommonHeader header;
	};

	struct AdvertRequestPacket {
		VTP3CommonHeader header;
		uint32_t start;
	};

	struct VlanInfo {
		uint8_t length;
		uint8_t status;
		uint8_t type;
		uint8_t name_length;
		uint16_t isl_vlan_id;
		uint16_t mtu_size;
		uint32_t index;
		//std::unique_ptr<char> name; /* Padded with zeros to Multiple of 4 Bytes */
		/* VlanInfoExtended */
	};

	struct VlanInfoExtended {
			uint8_t type; 	/* Captured packets have this field 1 byte long... but cisco says it should be 2 */
			uint8_t length; /* this too */
			/* space[length] */
	};

}



#endif /* COMMON_H_ */
