/*
 * PacketReceiving.hpp
 *
 *  Created on: Dec 6, 2014
 *      Author: tomas
 */

#ifndef PACKETRECEIVING_HPP_
#define PACKETRECEIVING_HPP_

#include "SummaryAdvertPacket.h"
#include "SubsetAdvertPacket.h"
#include "AdvertRequestPacket.h"
#include <pcap.h>

namespace VTP3{

struct summary_advert_mask {
	u_char version;
	u_char code;
	u_char followers;
	u_char domain_len;
	u_char domain_name[32];
	u_char revision_nr[4];
	u_char updater_id[4];
	u_char update_timestamp[12];
	u_char md5_digest[16];
};

struct subset_advert_mask {
	u_char version;
	u_char code;
	u_char sequence_number;
	u_char domain_length;
	u_char domain_name[32];
	u_char revision_nr[4];
};

struct vlan_field_mask {
	u_char len;
	u_char status;
	u_char type;
	u_char name_len;
	u_char vlan_id[2];
	u_char mtu_size[2];
	u_char index80211[4];
};

struct advert_request_mask{
	u_char version;
	u_char code;
	u_char reserved;
	u_char domain_len;
	u_char domain_name[32];
	u_char start_value[4];
};

struct thread_data{
	char *dev_name;
	void (*summary_advert_recv)(SummaryAdvertPacket *);
	void (*subset_advert_recv)(SubsetAdvertPacket *, std::vector<std::shared_ptr<VlanInfo>>);
	void (*advert_request_recv)(AdvertRequestPacket *);
};

void process_advert_request(int, const u_char *);
void process_subset_advert(uint16_t, int, const u_char *);
void process_summary_advert(int, u_char *);
void *init_pcap(void *);
void stop_pkt_receiving();
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

}



#endif /* PACKETRECEIVING_HPP_ */
