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

#include "c-apple2e-hardware.hpp"
#include "core/models/c-apple2e-core.hpp"
#include "hardware/video/c-main-video-output.hpp"
#include "hardware/c-cycle-and-wait-clock.hpp"

CApple2eHardware::CApple2eHardware(CMemory *lcRom, CMemory*intRom) {
	// Apple core
	core = new CApple2eCore(lcRom, intRom);
    clock = new CCycleAndWaitClock(core->get_processor());
	((CCycleAndWaitClock*)clock)->set_frequency(1000000.0); // Set frequency to 1MHz.

	// Peripherals
    keyboard   = std::make_unique<CKeyboard>(this);
    paddles    = std::make_unique<CPaddles>(clock, core->iou->game);
    speaker    = std::make_unique<CSpeaker>(clock, core->iou->speaker);

    colorvideo = new CMainVideoOutput(core->get_memory(), core->get_memory(), core->iou->textMode, core->iou->graphicMode, true);
    bwvideo    = new CMainVideoOutput(core->get_memory(), core->get_memory(), core->iou->textMode, core->iou->graphicMode, true);

	// First reset
	reset();
}

CApple2eHardware::~CApple2eHardware() {
	delete bwvideo;
    delete colorvideo;
	delete clock;
	delete core;
}

void CApple2eHardware::insertCard(int slot, CCard *card) {
	get_core()->insertCard(slot, card->get_unit(), card->get_rom(), card->get_romExt());
}

void CApple2eHardware::removeCard(int slot) {
	get_core()->removeCard(slot);
}

void CApple2eHardware::start() {
    qDebug() << "CComputerHardware::start()";
    clock->start();
}

void CApple2eHardware::stop() {
    clock->stop();
}

void CApple2eHardware::reset() {
    core->reset();
}

bool CApple2eHardware::is_running() {
    return clock->is_running();
};

