/*
 * FileUtility.hpp
 *
 *  Created on: Dec 15, 2014
 *      Author: tomas
 */

#ifndef FILEUTILITY_HPP_
#define FILEUTILITY_HPP_

#include "VlanInfo.h"

#include <cstring>


namespace VTP3{

void writeVlansToFile(std::vector<std::shared_ptr<VlanInfo>> vlans);

}




#endif /* FILEUTILITY_HPP_ */
