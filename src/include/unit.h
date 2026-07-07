#ifndef UNIT_H
#define UNIT_H

#include <raylib.h>

typedef enum{
    SOLDIER,
    TANK
} UnitType;

typedef struct{
    UnitType type;
    Vector2 position;
    int hp;
    float damage;
    int speed;
    int attack_range;
    bool selected;
    float armor;
    int owner;
    bool moved;
    bool attacked;
    bool merged;
    Rectangle boundingBox;
}Unit;

void InitUnit(Unit* unit,UnitType unitType,int x,int y ,int owner);
void UpdateUnit(Unit * unit);
void DrawUnit(Unit* unit);
void UnloadUnit(Unit* unit);

#endif