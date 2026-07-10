#include <math.h>
#include "physics.h"

float CalculateDistance(Vector2 pointA, Vector2 pointB){
    float distance = sqrtf(powf((pointB.x - (pointA.x - 1) * 32 - 16), 2) + powf((pointB.y - (pointA.y - 1) * 32 - 16), 2)) / 32.0f;
    return distance;
}