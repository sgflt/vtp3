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
#include "VTP3Output.h"

int main(void)
{
	uint8_t dst_mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	VTP3::SummaryAdvertPacket pkt;

	pkt.header.version = 0x03;
	pkt.header.code = 0x01;
	pkt.revision = 0x00;
	pkt.header.domain_length = 3;
	std::memcpy(&pkt.header.doman_name, "LAB", 3);

	/* Open RAW socket */
	int fd = VTP3::socket();



	/* Create Header */
	const std::shared_ptr<VTP3::EthernetHeader> header = VTP3::ethernet_header("eth0", dst_mac, fd);

	VTP3::send(fd, header, pkt, "eth0");
}
