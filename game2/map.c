#include "map.h"
#include "mapreader.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int getmap(MapInfo **maps, int n)
{
    char temp;
    FILE *file = fopen(maps[n]->mapPath, "r");

    char **map = malloc(maps[n]->mapSize_Y * sizeof(char *));
    for (int i = 0; i < maps[n]->mapSize_Y; i++)
        map[i] = malloc(maps[n]->mapSize_X * sizeof(char));

    getch();

    for (int z = 0; z < 4; z++)
        while ((temp = fgetc(file)) != '\n');

    for (int j = 0; j < maps[n]->mapSize_Y; j++)
    {
        for (int i = 0; i < maps[n]->mapSize_X; i++)
        {
            map[j][i] = fgetc(file);
        }
        temp = fgetc(file);
    }

    for (int j = 0; j < maps[n]->mapSize_Y; j++)
    {
        for (int i = 0; i < maps[n]->mapSize_X; i++)
        {
            printf("%c", map[j][i]);
        }
        printf("\n");
    }

    return 0;
}