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

#ifndef _C_KEYBOARD_UNIT_HPP_
#define _C_KEYBOARD_UNIT_HPP_

#include "c-unit.hpp"

#include <cstdint>

//
// Keyboard
//

class CKeyboardUnit: public CUnit {
public:
	void reset();
    void write(uint8_t addr, uint8_t byte);
    uint8_t read(uint8_t addr);
	
    void press_key(uint8_t key);
	void release_key();
	bool key_waiting();

protected:
    uint8_t lastkey;
	bool keydown;
	bool keywaiting;

private:
    void access(uint8_t addr);
};


#endif // _C_KEYBOARD_UNIT_HPP_
