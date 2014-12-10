/*----------------------------------------------------------------------------
 * VlanInfo.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "VlanInfo.h"

#include <ostream>

#include "common.h"

using namespace VTP3;

size_t VlanInfo::name_size(void)
{
	return this->data.name_length
			+ (4 - this->data.name_length % 4);
}

size_t VlanInfo::size(void)
{
	return sizeof(VlanInfoData)	+ name_size();
}

std::ostream& VTP3::operator<<(std::ostream& os, VlanInfo const& vinfo)
{
	os << "************************ VLAN info ***************************\n"
		<< "\tLength:\t\t\t\t" << static_cast<int>(vinfo.data.length) << "\n"
		<< "\tStatus:\t\t\t\t" << static_cast<int>(vinfo.data.status) << "\n"
		<< "\tType:\t\t\t\t" << static_cast<int>(vinfo.data.type) << "\n"
		<< "\tName length:\t\t\t" << static_cast<int>(vinfo.data.name_length) << "\n"
		<< "\tVlan id:\t\t\t" << vinfo.data.isl_vlan_id << "\n"
		<< "\tMTU:\t\t\t\t" << vinfo.data.mtu_size << "\n"
		<< "\tIndex:\t\t\t\t" << vinfo.data.index << "\n"
		<< "\tName:\t\t\t\t" << static_cast<char*>(vinfo.name.get()) << "\n";

	return os;
}
