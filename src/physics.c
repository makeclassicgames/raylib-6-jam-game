#include <math.h>
#include "physics.h"

float CalculateDistance(Vector2 pointA, Vector2 pointB){
    float distance = sqrtf(powf((pointA.x - (pointB.x - 1) * 32 - 16), 2) + powf((pointA.y - (pointB.y - 1) * 32 - 16), 2)) / 32.0f;
    return distance;
}