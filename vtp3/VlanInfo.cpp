/*----------------------------------------------------------------------------
 * VlanInfo.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "VlanInfo.h"

#include <cstdint>
#include <vector>
#include <memory>

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
