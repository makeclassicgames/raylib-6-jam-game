#include <stdlib.h>
#include "map.h"

void InitMap(Map *map, int width, int height)
{
    map->width = width;
    map->height = height;
    map->cells = (Cell **)malloc(width * sizeof(Cell *));
    for (int i = 0; i < width; i++)
    {
        map->cells[i] = (Cell *)malloc(height * sizeof(Cell));
    }

    // Initialize cells with random types
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int randomType = GetRandomValue(0, 3);
            map->cells[x][y].type = (CellType)randomType;
        }
    }
}

void UpdateMap(Map *map)
{
    TraceLog(LOG_INFO,"Updating map...");
}

void DrawMap(Map *map)
{
    for (int x = 0; x < map->width; x++)
    {
        for (int y = 0; y < map->height; y++)
        {
            Color cellColor;
            switch (map->cells[x][y].type)
            {
            case CELL_GRASS:
                cellColor = GREEN;
                break;
            case CELL_FOREST:
                cellColor = DARKGREEN;
                break;
            case CELL_WATER:
                cellColor = BLUE;
                break;
            case CELL_MOUNTAIN:
                cellColor = GRAY;
                break;
            }
            DrawPoly((Vector2){x * 32 + 16, y * 32 + 16}, 6, 16, 0, cellColor);
        }
    }
}
