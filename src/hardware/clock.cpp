#include "clock.hpp"

Clock::Clock(CProcessor* processor, QObject* parent) :QObject(parent)
{
    this->processor = processor;
    m_master_timer_period_ms = 1 /* msec */;
    m_master_timer = new QTimer();
    m_master_timer->setTimerType(Qt::PreciseTimer);
    m_master_timer->setInterval(m_master_timer_period_ms);
    QObject::connect(m_master_timer, &QTimer::timeout,
        this, &Clock::handleMasterTimer);

    timers = 0;
}

Clock::~Clock() {
    m_master_timer->stop();
    delete m_master_timer;
    clockTimer* timer = timers;
    clockTimer* next;
    while (timer) {
        next = timer->next;
        delete timer;
        timer = next;
    }
}

void Clock::start()
{
    qDebug() << "Starting clock.";
    m_master_timer->start();
}

void Clock::stop()
{
    m_master_timer->stop();
}

void Clock::cycle() {
    processor->cycle();
    cyclecount++;

    // Clock timer management
    timer = timers;
    while (timer)
    {
        if (!timer->remaining--)
        {
            timer->remaining = timer->period; // Reset... timer
            (*timer->callback)(timer->ptr);   //  ...and call callback
        }
        timer = timer->next; // Next timer
    }
}

void Clock::set_timer(int period, void (*callback)(void*), void* ptr)
{
    // Create a new timer
    clockTimer* timer = new clockTimer();
    timer->remaining = period;
    timer->period = period;
    timer->callback = callback;
    timer->ptr = ptr;

    // And place it in the timers list
    timer->next = timers;
    timers = timer;
}
