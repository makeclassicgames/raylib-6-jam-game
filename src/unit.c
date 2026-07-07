#include <stdio.h>
#include <stdlib.h>
#include "unit.h"
#include "input.h"
#include "cjson/cJSON.h"

typedef struct{
    int hp;
    float damage;
    int speed;
    int attack_range;
    float armor;
}UnitProperties;

UnitProperties* unitProperties;

UnitProperties* LoadUnitProperties();

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
    unit->boundingBox = (Rectangle){(unit->position.x-1) * 32, (unit->position.y-1) * 32, 32, 32};
}

void UpdateUnit(Unit * unit){

    if(GetLastInputAction() == SELECT){
        if(CheckCollisionPointRec(GetMousePosition(), unit->boundingBox)){
            unit->selected = true;
        }else{
            unit->selected = false;
        }
    }
}

void DrawUnit(Unit* unit){

    Vector2 drawPosition = {(unit->position.x-1) * 32 + 16, (unit->position.y-1) * 32 + 16};
    switch (unit->type)
    {
    case SOLDIER:
        DrawCircleV(drawPosition, 10, RED);
        break;
    case TANK:
        DrawRectangleV(drawPosition, (Vector2){20, 20}, BLUE);
        break;
    default:
        break;
    }
    if(unit->selected){
        DrawRectangleLines(unit->boundingBox.x, unit->boundingBox.y, unit->boundingBox.width, unit->boundingBox.height, YELLOW);
    }
}

UnitProperties* LoadUnitProperties(){
  
    char * data = LoadFileText("unit_properties.json");

    cJSON* json = cJSON_Parse(data);
    free(data);

    if (json == NULL) {
        printf("Failed to parse JSON\n");
        return NULL;
    }
    int UnitNumber = cJSON_GetArraySize(json);
    UnitProperties* properties = (UnitProperties*)malloc(UnitNumber*sizeof(UnitProperties));
    for (int i = 0; i < UnitNumber; i++) {
        cJSON* unitJson = cJSON_GetArrayItem(json, i);
        properties[i].hp = cJSON_GetObjectItem(unitJson, "hp")->valueint;
        properties[i].damage = cJSON_GetObjectItem(unitJson, "damage")->valuedouble;
        properties[i].speed = cJSON_GetObjectItem(unitJson, "speed")->valueint;
        properties[i].attack_range = cJSON_GetObjectItem(unitJson, "attack_range")->valueint;
        properties[i].armor = cJSON_GetObjectItem(unitJson, "armor")->valuedouble;
    }

    cJSON_Delete(json);
    return properties;
}
