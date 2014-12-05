/*----------------------------------------------------------------------------
 * Vlan8021Q.cpp
 *
 *  Created on: 5. 12. 2014
 *      Author: Lukáš Kvídera
 *	   Version: 0.0
 *		Jabber:	segfault@dione.zcu.cz
 ---------------------------------------------------------------------------*/
#include "Vlan8021Q.h"

using namespace VTP3;

#include <iostream>
#include <arpa/inet.h>
#include <stdexcept>

uint8_t Vlan8021Q::set_cfi(uint8_t cfi)
{
	if (cfi > 1 || cfi < 0)
		throw std::out_of_range("invalid cfi value");

	this->tci &= 0xffef;
	this->tci |= cfi << 4;
	return cfi;
}

uint8_t Vlan8021Q::set_pcp(uint8_t pcp)
{
	if (pcp > 7 || pcp < 0)
		throw std::out_of_range("invalid pcp value");

	this->tci &= 0xff1f;
	this->tci |= pcp << 5;
	return pcp;
}

uint16_t Vlan8021Q::set_vlan_id(uint16_t vlan)
{
	if (vlan > 0x0fff || vlan < 0)
		throw std::out_of_range("invalid vlan id");

	uint8_t *bytearray = reinterpret_cast<uint8_t*>(&vlan);

	uint16_t tmp = this->tci & 0x00f0;
	this->bytearray[1] = bytearray[0];
	this->bytearray[0] = bytearray[1];
	this->tci |= tmp;
	return vlan;
}



void Vlan8021Q::pack(Vlan8021Q const* other)
{
	this->tci = other->tci;
	this->length = htons( other->length );
}
