/*----------------------------------------------------------------------------
 * VlanInfo.h
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef VLANINFO_H_
#define VLANINFO_H_

#include <cstdint>
#include <memory>
#include <vector>
#include <ostream>

#include "common.h"

namespace VTP3
{

	struct VlanInfoData {
		uint8_t length;
		uint8_t status;
		uint8_t type;
		uint8_t name_length;
		uint16_t isl_vlan_id;
		uint16_t mtu_size;
		uint32_t index;
	}  __attribute__((packed));

	struct VlanInfo {
		VlanInfoData data;
		std::unique_ptr<char> name; /* Padded with zeros to Multiple of 4 Bytes */
		std::vector<std::shared_ptr<VlanInfoExtended>> extended;

		size_t size(void);
		size_t name_size(void);
	};

	std::ostream& operator<<(std::ostream& os, VlanInfo const& vinfo);

} /* namespace VTP3 */

#endif /* VLANINFO_H_ */
