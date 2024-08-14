#include "map.h"
#include "mapreader.h"
#include <stdlib.h>
#include <curses.h>

char **loadMap(MapInfo map_info)
{
    char temp;
    FILE *file = fopen(map_info.mapPath, "r");

    char **map = malloc(map_info.mapSize_Y * sizeof(char *));
    for (int i = 0; i < map_info.mapSize_Y; i++)
        map[i] = malloc(map_info.mapSize_X * sizeof(char));

    for (int z = 0; z < 4; z++)
        while ((temp = fgetc(file)) != '\n');

    for (int j = 0; j < map_info.mapSize_Y; j++)
    {
        for (int i = 0; i < map_info.mapSize_X; i++)
        {
            map[j][i] = fgetc(file);
        }
        temp = fgetc(file);
    }

    fclose(file);

    return map;
}