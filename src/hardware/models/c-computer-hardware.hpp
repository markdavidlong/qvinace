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


#ifndef _C_COMPUTER_HARDWARE_HPP_
#define _C_COMPUTER_HARDWARE_HPP_
#include "../../core/models/c-computer-core.hpp"
#include "../clock.hpp"

class CComputerHardware : public QObject
{
    Q_OBJECT

public:

    CComputerHardware(QObject *parent = 0);
    bool is_running();

    virtual CComputerCore *get_core() { return core; };
    virtual Clock *get_clock() { return clock; };

public slots:
    void start();
    void stop();
    void reset();

protected:
    CComputerCore *core;
    Clock *clock;
};

#endif // _C_COMPUTER_HARDWARE_HPP_
