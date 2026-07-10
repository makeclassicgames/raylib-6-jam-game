#include "input.h"

static Actions lastInputAction = NONE;

void HandleInput(void){
    lastInputAction = NONE; // Reset last input action at the beginning of each frame
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP)){
        lastInputAction = CONFIRM;
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsGestureDetected(GESTURE_DOUBLETAP)){
        lastInputAction = CANCEL;
    }
}
Actions GetLastInputAction(void){
    return lastInputAction;
}