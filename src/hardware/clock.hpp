#ifndef CLOCK_H
#define CLOCK_H


#include "../core/c-processor.hpp"
#include <QObject>
#include <QTimer>
#include <QDebug>

struct clockTimer {
    int period;               // Number of cycles between two calls
    int remaining;            // Cycles remaining before next call
    void (*callback)(void*); // Pointer to the callback function
    void* ptr;                // Void pointer given as first argument to the callback
    clockTimer* next;         // Next timer in the chained list
};

class Clock : public QObject
{
    Q_OBJECT

public:
    Clock(CProcessor* processor, QObject* parent = 0);
    ~Clock();
    virtual void run(int time_slice_ms) = 0;
    void start();
    void stop();
    void cycle();
    bool is_running() { return m_master_timer->isActive(); }
    void set_timer(int period, void (*callback)(void*), void* ptr);
    unsigned int get_cyclecount() { return cyclecount; } //  Get processor clock cycle count (Clocks must update this counter correctly)

protected:
    unsigned int cyclecount;

private slots:
    void handleMasterTimer() { /*qDebug() << "Calling run fn";*/ run(m_master_timer_period_ms); }

private:
    CProcessor* processor;
    clockTimer* timers;

    // Instantiated members for performance purpose
    clockTimer* timer;
    QTimer* m_master_timer;

    int m_master_timer_period_ms;
};

#endif // CLOCK_H
