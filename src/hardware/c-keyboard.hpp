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

#pragma once

class CApple2eHardware;
class CApple2eCore;
class CGameUnit;
class CKeyboardUnit;

#include <cstdint>

class CKeyboard
{
public:
    CKeyboard(CApple2eHardware *hw);

    // Standard keys
    void press_key(uint8_t asciiKey);
	void release_key();

	// Special keys
	void press_reset(); // Actually meaning CTRL-RESET on the Apple keyboard

	void press_open_apple();
	void release_open_apple();
	void press_closed_apple();
	void release_closed_apple();

protected:
    CApple2eCore *core;
	CKeyboardUnit *keyboard;
	CGameUnit     *game;
};

