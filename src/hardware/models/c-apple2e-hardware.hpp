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

#include "../clock.hpp"
#include "../../core/c-memory.hpp"
#include "../../core/models/c-apple2e-core.hpp"
#include "../c-keyboard.hpp"
#include "../c-speaker.hpp"
#include "../video/c-video-output.hpp"
#include "../cards/c-card.hpp"
#include "../c-paddles.hpp"

#include <QObject>
#include <memory>

class CApple2eHardware: public QObject
{
    Q_OBJECT

public:
	CApple2eHardware(CMemory *mainRom, CMemory *internalRom);
	~CApple2eHardware();

    bool is_running();

    virtual CApple2eCore *get_core() { return core; };

    void insertCard(int slot, CCard *card);
	void removeCard(int slot);
    virtual Clock *get_clock() { return clock; };

    CKeyboard &getKeyboard() { return *keyboard; }
    CSpeaker &getSpeaker() { return *speaker; }
    CPaddles &getPaddles() { return *paddles; }
	// Devices
	CVideoOutput *colorvideo;
	CVideoOutput *bwvideo;

public slots:
    void start();
    void stop();
    void reset();

protected:
    std::unique_ptr<CPaddles> paddles;
    std::unique_ptr<CSpeaker> speaker;
    std::unique_ptr<CKeyboard> keyboard;
	CMemory *mainRom;
    CMemory *internalRom;
    Clock *clock;
    CApple2eCore *core;
};

