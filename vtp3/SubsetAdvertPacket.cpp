/*----------------------------------------------------------------------------
 * SubsetAdvertPacket.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "SubsetAdvertPacket.h"

#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <array>

#include "common.h"
#include "VTP3Output.h"
#include "VlanInfo.h"

using namespace VTP3;

int SubsetAdvertPacket::send(Connection const& connection, std::vector<std::shared_ptr<VlanInfo>> vlans)
{
	size_t data_size = sizeof(*this);
	off_t offset = data_size;

	for (auto vlan : vlans) {
		data_size += vlan->size();
	}

	char buf[data_size];

	std::memcpy(buf, &this->header, sizeof(this->header));
	SubsetAdvertPacket* pkt = reinterpret_cast<SubsetAdvertPacket*>( buf );
	pkt->revision = htonl( this->revision );

	/* Append VLAN Info section */
	for(auto vlan : vlans)
	{
		VlanInfo* vinfo = reinterpret_cast<VlanInfo*>( buf + offset );
		std::memcpy(vinfo, vlan.get(), 4 * sizeof(uint8_t));

		vinfo->data.mtu_size = htons( vlan->data.mtu_size );
		vinfo->data.isl_vlan_id = htons( vlan->data.isl_vlan_id );
		vinfo->data.index = htonl( vlan->data.index );
		vinfo->data.length = vlan->size();

		offset += sizeof(VlanInfoData); /* size of static members of VlanInfo */

		std::memset(buf + offset, 0, vlan->data.name_length);
		std::memcpy(buf + offset, vlan->name.get(),  vlan->data.name_length);

		offset += vlan->name_size();
	}

	return VTP3::send(connection, buf, data_size);
}
