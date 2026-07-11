#ifndef UNIT_H
#define UNIT_H

#include <raylib.h>

typedef enum{
    SOLDIER,
    TANK,
    PLANE
} UnitType;

typedef struct{
    UnitType type;
    Vector2 position;
    int hp;
    int max_hp;
    float damage;
    int speed;
    int attack_range;
    bool selected;
    int armor;
    int owner;
    bool moved;
    bool attacked;
    bool merged;
    bool defending;
    Rectangle boundingBox;
    bool active;
}Unit;

void InitUnit(Unit* unit,UnitType unitType,int x,int y ,int owner);
void UpdateUnit(Unit * unit);
void DrawUnit(Unit* unit, int team);
void UnloadUnit(Unit* unit);

char * GetUnitTypeName(UnitType type);

#endif