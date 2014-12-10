/*
 * PacketReceiving.cpp
 *
 *  Created on: Dec 6, 2014
 *      Author: tomas
 */

#include <pcap.h>
#include <iostream>
#include <iomanip>
#include <atomic>

#include "PacketReceiving.hpp"
#include "common.h"
#include "AdvertRequestPacket.h"
#include "SummaryAdvertPacket.h"
#include "SubsetAdvertPacket.h"

#define SNAP_LEN 1518 /* default snap length - maximum bytes per packet to capture */
#define PROMISC_MODE 1 /* promiscuous mode */
#define READ_TIME_OUT 1000 /* read time out in milliseconds. just use non-zero timeout */
#define SNAP_8023_OFFSET 14
#define SNAP_ETHERNETII_OFFSET 18
#define SNAP_DSAP_SSAP_HEADER 0xAAAA
#define VTP_ID 0x2003
#define VTP_ID_OFFSET 6
#define VTP_VERSION_OFFSET 8
#define VTP_MESSAGE_OFFSET 1
#define LLC_HEADER_SIZE 8

using namespace std;

pcap_t *handle;
atomic_bool loop_running;

namespace VTP3{

void (*summary_advert_received)(struct SummaryAdvertPacket *);
void (*subset_advert_received)(struct SubsetAdvertPacket *, std::vector<std::shared_ptr<VlanInfo>>);
void (*advert_request_received)(struct AdvertRequestPacket *);

	void process_summary_advert(int offset, const u_char *packet){
		SummaryAdvertPacket pkt_sum;
		struct summary_advert_mask *mask;
		mask = (struct summary_advert_mask *) (packet + offset);
		pkt_sum.header.version = mask->version;
		pkt_sum.header.code = mask->code;
		pkt_sum.header.followers = mask->followers;
		pkt_sum.header.domain_len = mask->domain_len;
		memcpy(&pkt_sum.header.domain_name, &mask->domain_name, sizeof(pkt_sum.header.domain_name));
		pkt_sum.revision_nr = htonl(*((uint32_t *)(mask->revision_nr)));
		pkt_sum.updater_id = htonl(*((uint32_t *)(mask->updater_id)));
		memcpy(&pkt_sum.update_timestamp, &mask->update_timestamp, sizeof(pkt_sum.update_timestamp));
		memcpy(&pkt_sum.md5_digest, &mask->md5_digest, sizeof(pkt_sum.md5_digest));

		summary_advert_received(&pkt_sum);
	}

	void process_subset_advert(int payload_len, int offset, const u_char *packet){
		struct subset_advert_mask *mask;
		int act_bytes = 0;
		vector<shared_ptr<VlanInfo>> vlans;
		SubsetAdvertPacket pkt_sub;
		mask = (struct subset_advert_mask *) (packet + offset);
		pkt_sub.header.version = mask->version;
		pkt_sub.header.code = mask->code;
		pkt_sub.header.sequence_nr = mask->sequence_number;
		pkt_sub.header.domain_len = mask->domain_length;
		memcpy(&pkt_sub.header.domain_name, &mask->domain_name, sizeof(pkt_sub.header.domain_name));
		pkt_sub.revision_nr = htonl(*((int32_t *)(mask->revision_nr)));
		offset += sizeof(struct subset_advert_mask);
		act_bytes += sizeof(struct subset_advert_mask);
		act_bytes += LLC_HEADER_SIZE;
		//now should (packet + offset) point to the first VLAN Information (if available)
		struct vlan_field_mask *vf_mask;
		while(true){
			//is this end of the packet?
			if(payload_len == act_bytes)
				break;
			vf_mask = (struct vlan_field_mask *) (packet + offset);
			shared_ptr<VlanInfo> vlan( new VlanInfo );
			vlan->data.length = vf_mask->len;
			vlan->data.status = vf_mask->status;
			vlan->data.type = vf_mask->type;
			vlan->data.name_length = vf_mask->name_len;
			vlan->data.isl_vlan_id = htons(*((uint16_t *)(vf_mask->vlan_id)));
			vlan->data.mtu_size = htons(*((uint16_t *)(vf_mask->mtu_size)));
			vlan->data.index = htonl(*((uint32_t *)(vf_mask->index80211)));
			vlan->name = unique_ptr<char>(new char[vf_mask->name_len+1]);
			memcpy(vlan->name.get(), packet + offset + sizeof(struct vlan_field_mask), vf_mask->name_len+1);
			vlan->name.get()[vf_mask->name_len] = '\0';
			//push vlan
			vlans.push_back(vlan);
			//move to next vlan field
			offset += vf_mask->len;
			act_bytes += vf_mask->len;
		}


		subset_advert_received(&pkt_sub, vlans);
	}

	void process_advert_request(int offset, const u_char *packet){
		AdvertRequestPacket pkt_adv;
		struct advert_request_mask *mask;
		mask = (struct advert_request_mask *) (packet + offset);
		pkt_adv.header.version = mask->version;
		pkt_adv.header.code = mask->code;
		pkt_adv.header.reserved = mask->reserved;
		pkt_adv.header.domain_len = mask->domain_len;
		memcpy(&pkt_adv.header.domain_name, &mask->domain_name, sizeof(pkt_adv.header.domain_name));
		pkt_adv.start = htonl(*((uint32_t *)(mask->start_value)));

		advert_request_received(&pkt_adv);
	}

	void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
		if(!loop_running)
			pcap_breakloop(handle);

		int offset;
		uint8_t vtp_msg_code;
		uint16_t frame_len;

		//is received frame of SNAP protocol?
		if (*((u_int16_t *)(packet + SNAP_8023_OFFSET)) == SNAP_DSAP_SSAP_HEADER){
			//SNAP frame (encapsulated within IEEE 802.3 frame) received
			if((*((packet + SNAP_8023_OFFSET + VTP_ID_OFFSET)) << 8 | *(packet + SNAP_8023_OFFSET + VTP_ID_OFFSET + 1)) == VTP_ID)
				offset = SNAP_8023_OFFSET;
			else return;
		}
		else if(*((u_int16_t *)(packet + SNAP_ETHERNETII_OFFSET)) == SNAP_DSAP_SSAP_HEADER){
			//SNAP frame (encapsulated within Ethernet II frame) received
			if((*((packet + SNAP_ETHERNETII_OFFSET + VTP_ID_OFFSET)) << 8 | *(packet + SNAP_ETHERNETII_OFFSET + VTP_ID_OFFSET + 1)) == VTP_ID)
				offset = SNAP_ETHERNETII_OFFSET;
			else return;
		}
		else return; //not SNAP frame (maybe not even VTP packet) - drop
		frame_len = htons(*((uint16_t *)(packet + offset - 2)));//frame-len is on previous 2 bytes
		offset += VTP_VERSION_OFFSET + VTP_MESSAGE_OFFSET;
		vtp_msg_code = *((u_int8_t *)(packet + offset));
		switch(vtp_msg_code){
			//summary advertisement
			case 1:
				process_summary_advert(offset - VTP_MESSAGE_OFFSET, packet);
				break;
			//subset advertisement
			case 2:
				process_subset_advert(int(frame_len), offset - VTP_MESSAGE_OFFSET, packet);
				break;
			//advertisement request
			case 3:
				process_advert_request(offset - VTP_MESSAGE_OFFSET, packet);
				break;
			//takeover request/set
			case 5:
				cout << "** VTP packet received, but takeover messages are not supported yet" << endl << endl;
				break;
			//takeover response
			case 6:
				cout << "** VTP packet received, but takeover messages are not supported yet" << endl << endl;
				break;
			default:
				cout << "** VTP packet received, but message type code is unknown: " << vtp_msg_code << endl << endl;;
		}

	}

	void *init_pcap(void *d){
		char *dev = ((thread_data *) d)->dev_name;
		summary_advert_received = ((thread_data *) d)->summary_advert_recv;
		subset_advert_received = ((thread_data *) d)->subset_advert_recv;
		advert_request_received = ((thread_data *) d)->advert_request_recv;

		char errbuf[PCAP_ERRBUF_SIZE];
		loop_running = true;
		handle = pcap_open_live(dev, SNAP_LEN, PROMISC_MODE, READ_TIME_OUT, errbuf);

		if (handle == NULL) {
			cerr << "> couldn't open device " << errbuf << endl;
			exit(EXIT_FAILURE);
		}
		//ethernet device test
		if (pcap_datalink(handle) != DLT_EN10MB) {
			cerr << "> '" << dev << "' is not an Ethernet device" << endl;
			exit(EXIT_FAILURE);
		}

		pcap_loop(handle, 0, got_packet, NULL);

		pcap_close(handle);

		return NULL;
	}

	void stop_pkt_receiving(){
		loop_running = false;
//		cout << "breaking pcap loop" << endl;
	}

}
