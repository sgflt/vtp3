/*
 * FileUtility.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: tomas
 */


#include "FileUtility.hpp"
#include "VlanInfo.h"
#include "Constants.hpp"

#include <fstream>

namespace VTP3{


void writeVlansToFile(std::vector<std::shared_ptr<VlanInfo>> vlans){


	std::fstream fs;
	fs.open (getVlansFileName(), std::fstream::out | std::fstream::trunc);

	fs << getDomainName() << " " << getDomainPassword();

	for(std::vector<std::shared_ptr<VlanInfo>>::iterator it = vlans.begin() ; it != vlans.end() ; ++it){
		fs << int((*it)->data.isl_vlan_id) << ";" << (*it)->name.get() << std::endl;
	}

	fs.close();


}


}

