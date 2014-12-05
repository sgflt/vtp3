/*----------------------------------------------------------------------------
 * VTP3Output.cpp
 *
 *  Created on: 4. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "VTP3Output.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <memory>
#include <iostream>

#include "common.h"

/**
 * Get ethernet header for destination MAC address and interface name
 * @param if_name
 * @param dst_mac
 * @param sockfd
 * @return
 */
const std::shared_ptr<VTP3::EthernetHeader> VTP3::ethernet_header(
		std::string const& if_name, uint8_t dst_mac[MAC_LENGTH], const int sockfd)
{
	ifreq ifr;

	std::memset(&ifr, 0, sizeof(ifreq));
	std::strncpy(ifr.ifr_name, if_name.c_str(), IFNAMSIZ - 1);

	if ( ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0 ) {
	    std::perror("Cannot find address of interface");
	}

	std::shared_ptr<VTP3::EthernetHeader> header (new EthernetHeader());


	std::memcpy(&header->src_mac, &ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data));
	std::memcpy(&header->dst_mac, dst_mac, sizeof(header->dst_mac));

	return header;
}

int VTP3::socket(void)
{
	int sockfd;

	if ( (sockfd = ::socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    std::perror("Cannot open socket!");
	}

	return sockfd;
}

/**
 * Send VTP summary advert on socket.
 * @param sockfd
 * @param ethernet_header
 * @param packet
 * @param if_name
 * @return
 */
int VTP3::send(
		int sockfd, std::shared_ptr<EthernetHeader> ethernet_header,
		SummaryAdvertPacket const& packet, std::string const& if_name)
{
	ifreq ifr;
	sockaddr_ll socket_address;
	size_t data_size = sizeof(EthernetHeader) + sizeof(LLCHeader) + sizeof(SummaryAdvertPacket);
	size_t llc_size = sizeof(EthernetHeader) + sizeof(LLCHeader);
	char buf[data_size];
	EthernetHeader* eh = reinterpret_cast<EthernetHeader*>( &buf );
	LLCHeader* llc = reinterpret_cast<LLCHeader*>( buf + sizeof(EthernetHeader) );


	std::memset(&ifr, 0, sizeof(ifreq));
	std::strncpy(ifr.ifr_name, if_name.c_str(), IFNAMSIZ - 1);

	if ( ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0 ) {
	    std::perror("Cannot find index of interface");
	}


	/* Copy ethernet header into buffer */
	std::memcpy(&eh->dst_mac, &ethernet_header->dst_mac, sizeof(ethernet_header->dst_mac));
	std::memcpy(&eh->src_mac, &ethernet_header->src_mac, sizeof(ethernet_header->src_mac));

	/* Use 802.3 Ethernet frame */
	eh->type = htons(ETH_P_802_3);

	socket_address.sll_ifindex = ifr.ifr_ifindex;
	socket_address.sll_halen = ETH_ALEN;

	llc->dsap = 0xaa;
	llc->ssap = 0xaa;
	llc->ctrl = 0x03;
	llc->org_code[0] = 0x00;
	llc->org_code[1] = 0x00;
	llc->org_code[2] = 0x0c;
	llc->pid = htons(0x2003);

	std::memcpy(buf + llc_size, &packet, sizeof(packet));

	for( int i = 0; i < 20; i++)
	if ( sendto(sockfd, buf, data_size, 0, reinterpret_cast<sockaddr*>(&socket_address), sizeof(sockaddr_ll)) < 0 ) {
		std::perror("Send error");
	}

	return 0;
}
