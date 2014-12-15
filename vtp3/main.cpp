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
#include <unistd.h>
#include <pthread.h>
#include <csignal>

#include "common.h"
#include "Connection.h"
#include "VTP3Output.h"
#include "SummaryAdvertPacket.h"
#include "AdvertRequestPacket.h"
#include "ReceivedPacketHandling.hpp"
#include "SubsetAdvertPacket.h"
#include "Vlan8021Q.h"
#include "PacketReceiving.hpp"
#include "Constants.hpp"

using namespace std;
using namespace VTP3;

bool app_running = true;
bool client_mode = true;
int rcv_sockfd;


void signal_handler(int signo){
	if(signo == SIGINT){
		stop_pkt_receiving();
		app_running = false;

		if(client_mode)
			exit(EXIT_SUCCESS);
		else{
			close(rcv_sockfd);
			exit(EXIT_SUCCESS);
		}
	}
}

void app_init(int argc, char **argv){
	signal(SIGINT, signal_handler);

	if(argc == 1){
		cerr << "> first argument is missing: device name" << endl;
		exit(EXIT_FAILURE);
	}

	if(argc == 3){
		if(strcmp(argv[2],"-s") == 0)
			client_mode = false;
		else
			cout << "> unknown parameter '" << argv[2] << "', type -s for starting as server" << endl;
	}
}


int main(int argc, char **argv)
{
	app_init(argc, argv);

	string command;
	Connection con;
	struct sniffing_data sd;

	// Open RAW create_socket, this socket is closed in pkt_receiving method
	con.if_name = string(argv[1]);
	con.create_socket();
	con.create_header();
	rcv_sockfd = con.sockfd;

	if(client_mode){
		while(app_running){
			//inserting commands such as creating vlan, renaming vlan etc.
			cout << ">";
			getline(cin, command);

			//do sth
		}
	}
	else{
		sd.dev_name = argv[1];
		sd.summary_advert_recv = &summary_advert_received;
		sd.subset_advert_recv = &subset_advert_received;
		sd.advert_request_recv = &advert_request_received;
		sd.sockfd = con.sockfd;
		pkt_receiving(&sd);
	}



	/*


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
