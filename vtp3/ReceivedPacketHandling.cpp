/*
 * ReceivedPacketHandling.cpp
 *
 *  Created on: Dec 7, 2014
 *      Author: tomas
 */

#include "SummaryAdvertPacket.h"
#include "SubsetAdvertPacket.h"
#include "AdvertRequestPacket.h"

#include <iostream>
#include <sys/time.h>
#include <sstream>


using namespace std;


namespace VTP3{


	/* CALLBACKS for each vtp message type  */


	void summary_advert_received(SummaryAdvertPacket *pkt){
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

		cout << "> SUMMARY ADVERT RECEIVED" << endl;
		cout << "> \tVTP Version:\t\t" << int(pkt->header.version) << endl;
		cout << "> \tCode:\t\t\t" << int(pkt->header.code) << endl;
		cout << "> \tFollowers:\t\t" << int(pkt->header.followers) << endl;
		cout << "> \tDomain length:\t\t" << int(pkt->header.domain_len) << endl;
		cout << "> \tDomain name:\t\t" << pkt->header.domain_name << endl;
		cout << "> \tRevision number:\t" << pkt->revision_nr << endl;
		cout << "> \tUpdate timestamp:\t" << timestamp << endl;
		cout << "> \tUpdater ID:\t\t" << updater_id << endl;
		cout << "> \tMD5 digest:\t\t" << md5 << endl;

		cout << endl;
//		cout << "> --SUMMARY ADVERT RECEIVED-----" << endl;
	}

	void subset_advert_received(SubsetAdvertPacket *pkt){
		cout << "> SUBSET ADVERT RECEIVED" << endl;
		cout << "> \tVTP Version:\t\t" << int(pkt->header.version) << endl;
		cout << "> \tCode:\t\t\t" << int(pkt->header.code) << endl;
		cout << "> \tDomain length:\t\t" << int(pkt->header.domain_len) << endl;
		cout << "> \tSequence number:\t" << int(pkt->header.sequence_nr) << endl;
		cout << "> \tDomain name:\t\t" << pkt->header.domain_name << endl;
		cout << "> \tRevision number:\t" << pkt->revision_nr << endl;
		cout << "> \tIncluded VLANs:" << endl;

		cout << endl;
//		cout << "> --SUBSET ADVERT RECEIVED-----" << endl;

	}



	void advert_request_received(AdvertRequestPacket *pkt){
		cout << "> ADVERT REQUEST RECEIVED" << endl;

		cout << endl;
//		cout << "> --ADVERT REQUEST RECEIVED-----" << endl;

	}

}
