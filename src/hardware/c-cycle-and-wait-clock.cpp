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


#include "c-cycle-and-wait-clock.hpp"
#include "time.h"
 //#include "sys/time.h"
#include <iostream>
#include <QElapsedTimer>

CCycleAndWaitClock::CCycleAndWaitClock(CProcessor* processor) : Clock(processor) {
    //    for (int index = 0; index < CLOCK_STAT_PERIODS; index++)
    //        periods[period_number] = nsPeriod;
    //    period_number = 0;
}

void CCycleAndWaitClock::set_frequency(float freq) {
    nsPeriod = (int)1000000000 / freq; // nanoseconds per tick
}

void CCycleAndWaitClock::run(int msec_time_slice)
{
    long nsTimeSlice = msec_time_slice * 1000000;
    int ticks_desired = nsTimeSlice / nsPeriod;

    //   qDebug("Run: TimeSlice: %d, TicksDesired: %d, nsPeriod: %d",
   //           msec_time_slice, ticks_desired, nsPeriod);

    QElapsedTimer time;
    time.restart();
    for (int idx = 0; idx < ticks_desired; idx++)
    {
        cycle();
    }
    //    auto etime = time.nsecsElapsed();
    //    if (etime > nsTimeSlice)
    //    {
    //        qWarning("Clock overrun for %d ticks.  Took %d ns, Limited to %d ns.",
    //                 ticks_desired,
    //                 etime, nsTimeSlice);
    //    }
    //    else
    //    {
    //        qWarning("Clock underrun for %d ticks.  Took %d ns, Limited to %d.",
    //                 ticks_desired,
    //                 etime, nsTimeSlice);

    //    }
    notifyUpdate();
}

////			printf("Periods (ns/cycle): min=%d, max=%d, mean=%d\nCurrent: period=%d, expc=%d, expw=%d\n", min_period, max_period, mean_period, period,expc,expw);
//        }

//    }
//}
