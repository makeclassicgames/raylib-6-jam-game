#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

typedef enum{
    NONE,
    CONFIRM,
    CANCEL
}Actions;

void HandleInput(void);
Actions GetLastInputAction(void);

#endif