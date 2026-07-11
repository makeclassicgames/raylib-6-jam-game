#include <stdio.h>
#include <stdlib.h>
#include "unit.h"
#include "input.h"
#include "ui.h"
#include "cjson/cJSON.h"
#include "sprites.h"


#define JSON_UNIT_PATH "resources/units_properties.json"


typedef struct{
    int hp;
    float damage;
    int speed;
    int attack_range;
    int armor;
}UnitProperties;

UnitProperties* unitProperties;

UnitProperties* LoadUnitProperties();

void MoveUnit(void);
void AttackUnit(void);
void MergeUnits(void);
void WaitUnit(void);


Unit* selectedUnit = NULL;

void InitUnit(Unit* unit,UnitType unitType,int x,int y ,int owner){
    unitProperties = LoadUnitProperties();
    unit->type = unitType;
    unit->position= (Vector2){x,y};
    unit->owner = owner;

    UnitProperties properties = unitProperties[unitType];
    unit->hp = properties.hp;
    unit->damage = properties.damage;
    unit->speed = properties.speed;
    unit->attack_range = properties.attack_range;
    unit->armor = properties.armor;
    unit->selected = false;
    unit->moved = false;
    unit->attacked = false;
    unit->merged = false;
    unit->active = true;
    unit->defending = false;
    unit->max_hp = properties.hp;
    unit->boundingBox = (Rectangle){(unit->position.x-1) * 32, (unit->position.y-1) * 32, 32, 32};
}

void UpdateUnit(Unit * unit){

}

void DrawUnit(Unit* unit, int team){

    if(!unit->active){
        return;
    }
    Vector2 drawPosition = {(unit->position.x-1) * 32 + 8, (unit->position.y-1) * 32 + 8};
    drawSprite(unit->type, drawPosition, team);
   
}


char * GetUnitTypeName(UnitType type){
    
    switch(type){
        case SOLDIER:
            return "Soldier";
        case TANK:
            return "Tank";
        case PLANE:
            return "Plane";
        default:
            return "Unknown";
    }
}


UnitProperties* LoadUnitProperties(){
  
    char * data = LoadFileText(JSON_UNIT_PATH);

    cJSON* json = cJSON_Parse(data);
    free(data);

    if (json == NULL) {
        printf("Failed to parse JSON\n");
        return NULL;
    }
    int UnitNumber = cJSON_GetArraySize(json);

    UnitProperties* properties = (UnitProperties*)malloc(UnitNumber*sizeof(UnitProperties));

    for (int i = 0; i < UnitNumber; i++) {
        printf("loadesd\n");
        cJSON* unitJson = cJSON_GetArrayItem(json, i);
        properties[i].hp = cJSON_GetObjectItem(unitJson, "max_hp")->valueint;
        properties[i].damage = cJSON_GetObjectItem(unitJson, "damage")->valuedouble;
        properties[i].speed = cJSON_GetObjectItem(unitJson, "speed")->valueint;
        properties[i].attack_range = cJSON_GetObjectItem(unitJson, "attack_range")->valueint;
        properties[i].armor = cJSON_GetObjectItem(unitJson, "armor")->valueint;
    }
    cJSON_Delete(json);
    return properties;
}



