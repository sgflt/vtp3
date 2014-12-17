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
#include <iostream>
#include <sstream>


namespace VTP3{

std::string vlan_file;
std::string domain_password;

void writeVlansToFile(std::vector<std::shared_ptr<VlanInfo>> vlans){
	std::ofstream ofs;
	ofs.open (vlan_file);
	ofs << domain_name << " " << domain_password << std::endl;
	for(std::vector<std::shared_ptr<VlanInfo>>::iterator it = vlans.begin() ; it != vlans.end() ; ++it)
		ofs << int((*it)->data.isl_vlan_id) << ";" << (*it)->name.get() << std::endl;
	ofs.close();
}

void loadDomainData(){
	std::string line;
	std::ifstream ifs (vlan_file);
	std::cout << "a" << vlan_file << std::endl;
	if(ifs.is_open()){
		std::cout << "a" << std::endl;
		if(getline(ifs, line)){
			std::vector<std::string> elems;
			std::stringstream ss(line);
			std::string item;
			while(std::getline(ss, item, ' '))
				if(!item.empty())
					elems.push_back(item);
			if(elems.size() > 0){
				domain_name = elems.at(0);
				if(elems.size() > 1)
					domain_password = elems.at(1);
			}

		}
		ifs.close();
	}


}

}

