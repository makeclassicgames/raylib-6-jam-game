#ifndef TIMER_H
#define TIMER_H
#include <raylib.h>

typedef struct{
    int elapsedTime;
    int maxTime;
    bool started;
    bool repeat;
    void (*callback)();
}Timer;

void InitTimer(Timer* timer,int maxTime,bool repeat, bool autoStart,void (*callback)());
void UpdateTimer(Timer* timer);
void StopTimer(Timer* timer);
void StartTimer(Timer* timer);

#endif