#ifndef UNIT_H
#define UNIT_H

#include <raylib.h>

#define MERGE_RANGE 3

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

void MoveUnit(Unit* unit, Vector2 targetPosition);
void resetUnit(Unit* unit);
int AttackUnit(Unit* attacker, Unit* target);
int MergeUnits(Unit* unit1, Unit* unit2, Unit* targetUnit);
void DefendUnit(Unit* unit);

#endif