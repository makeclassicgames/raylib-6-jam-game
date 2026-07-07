#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

typedef enum{
    NONE,
    SELECT,
    MOVE
}Actions;

void HandleInput(void);
Actions GetLastInputAction(void);

#endif