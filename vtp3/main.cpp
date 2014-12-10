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
#include <sys/msg.h>
#include <pthread.h>
#include <csignal>
#include <pcap.h>

#include "common.h"
#include "Connection.h"
#include "VTP3Output.h"
#include "SummaryAdvertPacket.h"
#include "AdvertRequestPacket.h"
#include "ReceivedPacketHandling.hpp"
#include "SubsetAdvertPacket.h"
#include "Vlan8021Q.h"
#include "PacketReceiving.hpp"

using namespace std;
using namespace VTP3;

bool app_running = true;


void signal_handler(int signo){
	//TODO what if user selects non-active device: it will work, but it will get stuck in the loop
	stop_pkt_receiving(); //break receiving loop
	app_running = false;
	cout << "> waiting for one more packet to break sniffing loop" << endl;
}

void app_init(int argc, char **argv){
	signal(SIGINT, signal_handler);

	if(argc == 1){
		cerr << "> first argument is missing: device name" << endl;
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv)
{
	app_init(argc, argv);

	//data structure with callbacks for new sniffing thread
	thread_data td;
	td.dev_name = argv[1];
	td.summary_advert_recv = &summary_advert_received;
	td.subset_advert_recv = &subset_advert_received;
	td.advert_request_recv = &advert_request_received;

	pthread_t receiving_thread;
	pthread_create(&receiving_thread, NULL, init_pcap, &td);

	while(app_running){
		//inserting commands such as creating vlan, renaming vlan etc.
	}

	pthread_join(receiving_thread, NULL);

	/*
	Connection con;

	// Open RAW socket
	con.if_name = string(dev);
	con.socket();
	con.create_header();

	con.vlan8021Q_header = shared_ptr<Vlan8021Q>(new Vlan8021Q);

	con.vlan8021Q_header->set_cfi(0);
	con.vlan8021Q_header->set_vlan_id(0x0fff);
	con.vlan8021Q_header->set_pcp(7);

	{
		SummaryAdvertPacket pkt;

		pkt.header.version = 0x03;
		pkt.header.code = 0x01;
		pkt.revision = 0x23;
		pkt.header.domain_length = 3;
		memcpy(&pkt.header.doman_name, "LAB", 3);
		pkt.send(con);
	}

	{
		SubsetAdvertPacket pkt;
		vector<shared_ptr<VlanInfo>> vlans;

		shared_ptr<VlanInfo> vlan( new VlanInfo );
		vlan->name = unique_ptr<char>( new char[500] );

		vlan->data.mtu_size = 1500;
		vlan->data.name_length = 7;
		vlan->data.isl_vlan_id = 0xabcd;
		vlan->data.type = 0x01;
		memcpy(vlan->name.get(), "default", 7);

		vlans.push_back( vlan );

		pkt.header.version = 0x03;
		pkt.header.code = 0x02;
		pkt.revision = 0x03;
		pkt.header.domain_length = 3;
		memcpy(&pkt.header.doman_name, "LAB", 3);
		pkt.send(con, vlans);
	}*/

}
