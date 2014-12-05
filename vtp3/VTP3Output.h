/*----------------------------------------------------------------------------
 * VTP3Output.h
 *
 *  Created on: 4. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#ifndef VTP3OUTPUT_H_
#define VTP3OUTPUT_H_

#include <memory>

#include "common.h"
#include "Connection.h"

namespace VTP3
{
	std::shared_ptr<VTP3::EthernetHeader> ethernet_header(
			std::string const& if_name, uint8_t dst_mac[MAC_LENGTH], const int sockfd);

	void init_header(LLCHeader* llc);
	int send(Connection const& connection, const void *data, size_t size);
	int socket(void);

} /* namespace VTP3 */

#endif /* VTP3OUTPUT_H_ */
