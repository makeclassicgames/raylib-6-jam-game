#include <stdlib.h>
#include "timer.h"

void InitTimer(Timer* timer,int maxTime,bool repeat,bool autoStart,void (*callback)()){
    timer->elapsedTime = 0;
    timer->maxTime = maxTime;
    timer->started = autoStart;
    timer->repeat = repeat;
    timer->callback = callback;
}
void UpdateTimer(Timer* timer){
    if(timer->started){
        timer->elapsedTime++;
        if(timer->elapsedTime >= timer->maxTime){
            if(timer->callback != NULL){
                timer->callback();
            }
            if(timer->repeat){
                timer->elapsedTime = 0;
            }else{
                timer->started = false;
            }
        }
    }
}
void StopTimer(Timer* timer){
    timer->started = false;
}
void StartTimer(Timer* timer){
    timer->started = true;
    timer->elapsedTime = 0;
}