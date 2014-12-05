/*----------------------------------------------------------------------------
 * main.cpp
 *
 *  Created on: 4. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/

#include <iostream>
#include <cstring>


#include "common.h"
#include "Connection.h"
#include "VTP3Output.h"
#include "SummaryAdvertPacket.h"
#include "Vlan8021Q.h"


int main(void)
{

	VTP3::SummaryAdvertPacket pkt;
	VTP3::Connection con;
	con.vlan8021Q_header = std::shared_ptr<VTP3::Vlan8021Q>(new VTP3::Vlan8021Q);

	con.vlan8021Q_header->set_cfi(0);
	con.vlan8021Q_header->set_vlan_id(0x0fff);
	con.vlan8021Q_header->set_pcp(7);

	pkt.header.version = 0x03;
	pkt.header.code = 0x01;
	pkt.revision = 0x00;
	pkt.header.domain_length = 3;
	std::memcpy(&pkt.header.doman_name, "LAB", 3);

	/* Open RAW socket */
	con.if_name = "eth0";
	con.socket();
	con.create_header();


	pkt.send(con);
}
