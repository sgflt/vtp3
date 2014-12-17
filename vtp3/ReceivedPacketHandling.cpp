/*
 * ReceivedPacketHandling.cpp
 *
 *  Created on: Dec 7, 2014
 *      Author: tomas
 */

#include "SummaryAdvertPacket.h"
#include "SubsetAdvertPacket.h"
#include "AdvertRequestPacket.h"
#include "Constants.hpp"
#include "FileUtility.hpp"

#include <iostream>
#include <sys/time.h>
#include <sstream>
#include <cstring>


using namespace std;



namespace VTP3{

string domain_name;

	/* CALLBACKS for each vtp message type  */


	void summary_advert_received(SummaryAdvertPacket *pkt){
		if(strcmp((char *) (pkt->header.domain_name), domain_name.c_str()) != 0)
			//drop packets from different domain
			return;

		char buf[128];
		uint8_t *tmp;
		tmp = pkt->update_timestamp;
		string updater_id, md5, timestamp;
		sprintf(buf, "%x.%x.%x.%x",(pkt->updater_id >> 24) & 0xFF, (pkt->updater_id >> 16) & 0xFF, (pkt->updater_id >> 8) & 0xFF, (pkt->updater_id) & 0xFF);
		updater_id = string(buf);
		sprintf(buf, "%c%c-%c%c-%c%c %c%c:%c%c:%c%c",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11]);
		timestamp = string(buf);
		tmp = pkt->md5_digest;
		for(int i = 0 ; i < 16 ; i++){
			sprintf(buf, "%x", tmp[i]);
			md5 += buf;
		}

		cout << "** SUMMARY ADVERT RECEIVED" << endl;
		cout << "> \tVTP Version:\t\t" << int(pkt->header.version) << endl;
		cout << "> \tCode:\t\t\t" << int(pkt->header.code) << endl;
		cout << "> \tFollowers:\t\t" << int(pkt->header.followers) << endl;
		cout << "> \tDomain length:\t\t" << int(pkt->header.domain_len) << endl;
		cout << "> \tDomain name:\t\t" << pkt->header.domain_name << endl;
		cout << "> \tRevision number:\t" << pkt->revision_nr << endl;
		cout << "> \tUpdater identity:\t" << updater_id << endl;
		cout << "> \tUpdate timestamp:\t" << timestamp << endl;
		cout << "> \tMD5 digest:\t\t" << md5 << endl;

		cout << endl;
	}

	void subset_advert_received(SubsetAdvertPacket *pkt, std::vector<std::shared_ptr<VlanInfo>> vlans){
		if(strcmp((char *) (pkt->header.domain_name), domain_name.c_str()) != 0)
			//drop packets from different domain
			return;
		char buf[64];

		cout << "** SUBSET ADVERT RECEIVED" << endl;
		cout << "> \tVTP Version:\t\t" << int(pkt->header.version) << endl;
		cout << "> \tCode:\t\t\t" << int(pkt->header.code) << endl;
		cout << "> \tSequence number:\t" << int(pkt->header.sequence_nr) << endl;
		cout << "> \tDomain length:\t\t" << int(pkt->header.domain_len) << endl;
		cout << "> \tDomain name:\t\t" << pkt->header.domain_name << endl;
		cout << "> \tRevision number:\t" << pkt->revision_nr << endl;
		cout << "> \tIncluded VLANs:" << endl;
		for(vector<shared_ptr<VlanInfo>>::iterator it = vlans.begin() ; it != vlans.end() ; ++it){
			cout << "> \t  Vlan: ";
			cout << "Name = " << (*it)->name.get() << ", ";
			cout << "Vlan ID = " << int((*it)->data.isl_vlan_id) << ", ";
			cout << "Status = " << int((*it)->data.status) << ", ";
			cout << "VLAN type = " << int((*it)->data.type) << ", ";
			cout << "VLAN name len = " << int((*it)->data.name_length) << ", ";
			cout << "MTU size = " << int((*it)->data.mtu_size) << ", ";
			sprintf(buf, "0x%08x", (*it)->data.index);
			cout << "Index = " << buf;

			cout << endl;
		}
		if(vlans.size())
			cout << endl;

		writeVlansToFile(vlans);

	}



	void advert_request_received(AdvertRequestPacket *pkt){
		if(strcmp((char *) (pkt->header.domain_name), domain_name.c_str()) != 0)
			//drop packets from different domain
			return;

		cout << "** ADVERT REQUEST RECEIVED" << endl;
		cout << "> \tVTP Version:\t\t" << int(pkt->header.version) << endl;
		cout << "> \tCode:\t\t\t" << int(pkt->header.code) << endl;
		cout << "> \tFollowers:\t\t" << int(pkt->header.followers) << endl;
		cout << "> \tDomain length:\t\t" << int(pkt->header.domain_len) << endl;
		cout << "> \tDomain name:\t\t" << pkt->header.domain_name << endl;
		cout << "> \tStart-value:\t\t" << pkt->start << endl;

		cout << endl;

	}

}
