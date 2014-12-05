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
#include "SubsetAdvertPacket.h"
#include "Vlan8021Q.h"


int main(void)
{
	VTP3::Connection con;

	/* Open RAW socket */
	con.if_name = "eth0";
	con.socket();
	con.create_header();

	con.vlan8021Q_header = std::shared_ptr<VTP3::Vlan8021Q>(new VTP3::Vlan8021Q);

	con.vlan8021Q_header->set_cfi(0);
	con.vlan8021Q_header->set_vlan_id(0x0fff);
	con.vlan8021Q_header->set_pcp(7);

	{
		VTP3::SummaryAdvertPacket pkt;

		pkt.header.version = 0x03;
		pkt.header.code = 0x01;
		pkt.revision = 0x23;
		pkt.header.domain_length = 3;
		std::memcpy(&pkt.header.doman_name, "LAB", 3);
		pkt.send(con);
	}

	{
		VTP3::SubsetAdvertPacket pkt;
		std::vector<std::shared_ptr<VTP3::VlanInfo>> vlans;

		std::shared_ptr<VTP3::VlanInfo> vlan( new VTP3::VlanInfo );
		vlan->name = std::unique_ptr<char>( new char[500] );

		vlan->data.mtu_size = 1500;
		vlan->data.name_length = 7;
		vlan->data.isl_vlan_id = 0xabcd;
		vlan->data.type = 0x01;
		std::memcpy(vlan->name.get(), "default", 7);

		vlans.push_back( vlan );

		pkt.header.version = 0x03;
		pkt.header.code = 0x02;
		pkt.revision = 0x03;
		pkt.header.domain_length = 3;
		std::memcpy(&pkt.header.doman_name, "LAB", 3);
		pkt.send(con, vlans);
	}

}
