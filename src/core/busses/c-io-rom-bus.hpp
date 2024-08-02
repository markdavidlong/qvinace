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

#ifndef _C_IO_ROM_BUS_HPP_
#define _C_IO_ROM_BUS_HPP_

#include "../c-memory.hpp"
#include "../c-observer.hpp"
#include "../units/c-io-rom-unit.hpp"

class CIoRomBus: public CMemory, public CObserver {
public:
	CIoRomBus(CIoRomUnit *ioRomUnit, CMemory *slotRom, CMemory *internalRom);
	~CIoRomBus();
    uint8_t read(uint16_t addr);
	void notify(CObservable * obs);
protected:
	CIoRomUnit *ioRomUnit;

	CMemory *slotRom;
	CMemory *internalRom;
	CMemory *c3Rom;

	CMemory *selectedRom;
	CMemory *selectedC3Rom;
};

#endif // _C_IO_ROM_BUS_HPP_
