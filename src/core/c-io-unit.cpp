/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "c-io-unit.hpp"

CIoUnit::CIoUnit() {
	dummy = new CDummyUnit();
	
	// Assign dummy unit to each address
	for (int addr=0; addr<256; addr++) {
		read_map[addr]  = dummy;
	    write_map[addr] = dummy;
	}
}

//
// Memory read and write methods
//

void CIoUnit::write(uint16_t addr, uint8_t byte) {
	write_map[addr&0xff]->write(addr&0xff, byte);
}

uint8_t CIoUnit::read(uint16_t addr) {
	return read_map[addr&0xff]->read(addr&0xff);
}

//
// Unit assignation
//

void CIoUnit::assign_read(uint8_t addr, CUnit *unit) {
	read_map[addr&0xff] = unit;
}

void CIoUnit::assign_write(uint8_t addr, CUnit *unit) {
	write_map[addr] = unit;
}

void CIoUnit::assign_read_write(uint8_t addr, CUnit *unit) {
	assign_read(addr, unit);
	assign_write(addr, unit);
}

void CIoUnit::assign_read(uint8_t startaddr, uint8_t endaddr, CUnit *unit) {
	for (int addr = startaddr; addr <= endaddr; addr++)
		assign_read(addr, unit);
}

void CIoUnit::assign_write(uint8_t startaddr, uint8_t endaddr, CUnit *unit) {
	for (int addr = startaddr; addr <= endaddr; addr++)
		assign_write(addr, unit);
}

void CIoUnit::assign_read_write(uint8_t startaddr, uint8_t endaddr, CUnit *unit) {
	for (int addr = startaddr; addr <= endaddr; addr++) {
		assign_read(addr, unit);
		assign_write(addr, unit);
	}
}

