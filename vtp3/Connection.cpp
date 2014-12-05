/*----------------------------------------------------------------------------
 * Connection.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/

#include "Connection.h"
#include "VTP3Output.h"

using namespace VTP3;

int Connection::socket(void)
{
	return this->sockfd = VTP3::socket();
}

void Connection::create_header(void)
{
	this->ethernet_header =
			VTP3::ethernet_header(
					this->if_name, this->dst_mac, this->sockfd);
}
