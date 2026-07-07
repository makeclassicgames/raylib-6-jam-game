#ifndef MAP_H
#define MAP_H

#include "raylib.h"

typedef enum{
    CELL_GRASS,
    CELL_FOREST,
    CELL_WATER,
    CELL_MOUNTAIN
} CellType;

typedef struct {
    CellType type;
}Cell;

typedef struct {
    int width;
    int height;
    Cell** cells;
} Map;

void InitMap(Map* map, int width, int height);

void UpdateMap(Map* map);
void DrawMap(Map* map);
void UnloadMap(Map* map);

#endif // MAP_H