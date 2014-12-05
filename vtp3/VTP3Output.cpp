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
#include "Connection.h"
#include "Vlan8021Q.h"

/**
 * Get ethernet header for destination MAC address and interface name
 * @param if_name
 * @param dst_mac
 * @param sockfd
 * @return
 */
std::shared_ptr<VTP3::EthernetHeader> VTP3::ethernet_header(
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

void VTP3::init_header(LLCHeader* llc)
{
	llc->dsap = 0xaa;
	llc->ssap = 0xaa;
	llc->ctrl = 0x03;
	llc->org_code[0] = 0x00;
	llc->org_code[1] = 0x00;
	llc->org_code[2] = 0x0c;
	llc->pid = htons( 0x2003 );
}

/**
 * Send VTP summary advert on socket.
 * @param sockfd
 * @param ethernet_header
 * @param packet
 * @param if_name
 * @return
 */
int VTP3::send(Connection const& connection, const void *data, size_t size)
{
	ifreq ifr;
	const bool has_vlan = connection.vlan8021Q_header.get() != nullptr;
	sockaddr_ll socket_address;
	const size_t eth_size = sizeof(EthernetHeader);
	const size_t llc_size = sizeof(LLCHeader);
	const size_t vlan8021Q_size = has_vlan ? sizeof(Vlan8021Q) : 0;

	const off_t data_offset = has_vlan ?
			eth_size + vlan8021Q_size + llc_size :
			eth_size + llc_size;
	const off_t llc_offset = has_vlan ?
			eth_size + vlan8021Q_size:
			eth_size;

	const size_t data_size = eth_size + vlan8021Q_size + llc_size + size;
	char buf[data_size];
	EthernetHeader* eh = reinterpret_cast<EthernetHeader*>( buf );
	Vlan8021Q* vlan8021Q = reinterpret_cast<Vlan8021Q*>( buf + eth_size );
	LLCHeader* llc = reinterpret_cast<LLCHeader*>( buf + llc_offset );


	std::memset(&ifr, 0, sizeof(ifreq));
	std::strncpy(ifr.ifr_name, connection.if_name.c_str(), IFNAMSIZ - 1);

	if ( ioctl(connection.sockfd, SIOCGIFINDEX, &ifr) < 0 ) {
	    std::perror("Cannot find index of interface");
	}

	socket_address.sll_ifindex = ifr.ifr_ifindex;
	socket_address.sll_halen = ETH_ALEN;

	/* Copy ethernet header into buffer */
	std::memcpy(&eh->dst_mac, &connection.ethernet_header->dst_mac, sizeof(connection.ethernet_header->dst_mac));
	std::memcpy(&eh->src_mac, &connection.ethernet_header->src_mac, sizeof(connection.ethernet_header->src_mac));

	/* Use 802.3 Ethernet frame without Vlan or Ethernet II with Vlan */
	eh->type = has_vlan ?
			htons( ETH_P_8021Q ) :
			htons( data_size - eth_size ); /* set length of 802.3 payload */



	if ( has_vlan ) {
		connection.vlan8021Q_header->length = llc_size + size;
		vlan8021Q->pack( connection.vlan8021Q_header.get() );
	}

	init_header( llc );

	std::memcpy(buf + data_offset, data, size);

	for( int i = 0; i < 20; i++)//XXX debug
	if ( sendto(connection.sockfd, buf, data_size, 0, reinterpret_cast<sockaddr*>(&socket_address), sizeof(sockaddr_ll)) < 0 ) {
		std::perror("Send error");
	}

	return 0;
}
