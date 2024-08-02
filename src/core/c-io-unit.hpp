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

#ifndef _C_IO_UNIT_HPP_
#define _C_IO_UNIT_HPP_
#include "c-memory.hpp"
#include "units/c-unit.hpp"

class CDummyUnit:public CUnit
{
public:
    void write(uint8_t addr, uint8_t byte) {};
    uint8_t read(uint8_t addr) { return dummy_byte() & 0x7F; };
};

class CIoUnit: public CMemory 
{
public:
	CIoUnit();
	virtual void reset() = 0;
    virtual void write(uint16_t addr, uint8_t byte);
    virtual uint8_t read(uint16_t addr);
 
    void assign_read(uint8_t addr, CUnit *unit);
    void assign_write(uint8_t addr, CUnit *unit);
    void assign_read_write(uint8_t addr, CUnit *unit);
    void assign_read(uint8_t startaddr, uint8_t endaddr, CUnit *unit);
    void assign_write(uint8_t startaddr, uint8_t endaddr, CUnit *unit);
    void assign_read_write(uint8_t startaddr, uint8_t endaddr, CUnit *unit);

protected:
	CUnit *dummy;
	CUnit *read_map[256];
	CUnit *write_map[256];
};

#endif // _C_IO_UNIT_HPP_
