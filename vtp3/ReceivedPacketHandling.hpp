/*
 * ReceivedPacketHandling.hpp
 *
 *  Created on: Dec 7, 2014
 *      Author: tomas
 */

#ifndef RECEIVEDPACKETHANDLING_HPP_
#define RECEIVEDPACKETHANDLING_HPP_

#include "SummaryAdvertPacket.h"
#include "SubsetAdvertPacket.h"
#include "AdvertRequestPacket.h"
#include "VlanInfo.h"

namespace VTP3{

	void summary_advert_received(SummaryAdvertPacket *);
	void subset_advert_received(SubsetAdvertPacket *, std::vector<std::shared_ptr<VlanInfo>>);
	void advert_request_received(AdvertRequestPacket *);

}



#endif /* RECEIVEDPACKETHANDLING_HPP_ */
