/*----------------------------------------------------------------------------
 * VTP3CommonHeader.cpp
 *
 *  Created on: 9. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "VTP3CommonHeader.h"

#include <ostream>
#include <cstdint>

#include "common.h"

namespace VTP3
{
	std::ostream& operator<<(std::ostream& os, VTP3CommonHeader const& vch)
	{
		os << "************************** VTP3 ******************************\n"
		   << "\tVersion:\t\t\t" << static_cast<int>(vch.version) << "\n"
		   << "\tCode:\t\t\t\t" << static_cast<int>(vch.code) << "\t";

		switch (vch.code)
		{
			case MessageType::SMMARY_ADVERT:
				os << "Summary advert\n";
				os << "\tFollowers:\t\t" << static_cast<int>(vch.followers) << "\n";
				break;

			case MessageType::SUBSET_ADVERT:
				os << "Subset advert\n";
				os << "\tSequence number:\t\t" << static_cast<int>(vch.sequence_nr) << "\n";
				break;

			case MessageType::ADVERT_REQUEST:
				os << "Advert request\n";
				break;

			default:
				os << "Unknown type\n";
				break;
		}

		os << "\tManagement domain lengt:\t" << static_cast<int>(vch.domain_len) << "\n"
		   << "\tManagement domain:\t\t" << vch.domain_name << "\n"
		   << "\tConfiguration revision number:\t" << static_cast<int>(vch.version);

		return os;
	}

} /* namespace VTP3 */
