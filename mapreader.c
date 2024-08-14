#include "mapreader.h"
#include <curses.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WINDOWS
#define STAT _stat
#else
#define STAT stat
#endif

MapInfo newMapInfo(char map_path[100])
{
    FILE *file = fopen(map_path, "r");
    MapInfo new_map = {0};

    fscanf(file, "%d\n", &(new_map.mapId));
    fgets(new_map.mapName, sizeof(new_map.mapName), file);
    new_map.mapName[strcspn(new_map.mapName, "\n")] = 0;
    sprintf(new_map.mapPath, map_path);
    fscanf(file, "%d ", &(new_map.mapSize_Y));
    fscanf(file, "%d\n", &(new_map.mapSize_X));
    fscanf(file, "%d ", &(new_map.start_Y));
    fscanf(file, "%d\n", &(new_map.start_X));

    fclose(file);

    return new_map;
}

int mapInfoReader(MapInfo **maps)
{
    struct dirent *input;
    DIR *directory = opendir("maps");

    int mapQuantity = 0;

    if (directory == NULL)
        return -1;

    while ((input = readdir(directory)) != NULL)
    {
        char filename[100] = {"maps/"};

        strcat(filename, input->d_name);
        struct STAT buffer = {0};

        int err = STAT(filename, &buffer);

        if (!err)
        {
            if (S_ISREG(buffer.st_mode))
                mapQuantity++;
        }
        else
            printf("error: %d (%s)\n", err, strerror(errno));
    }

    *maps = malloc(mapQuantity * (sizeof(MapInfo)));

    closedir(directory);
    directory = opendir("maps");

    for (int i = 0; (input = readdir(directory)) != NULL; i++)
    {
        char filename[100] = {"maps/"};

        strcat(filename, input->d_name);
        struct STAT buffer = {0};

        if (!STAT(filename, &buffer))
        {
            if (!S_ISREG(buffer.st_mode))
            {
                i--;
                continue;
            }
        }

        if (strstr(input->d_name, ".txt") != NULL)
        {
            char map_path[100] = {0};
            snprintf(map_path, sizeof(map_path), "maps/%s", input->d_name);

            MapInfo tempMap = newMapInfo(map_path);

            (*maps)[i] = tempMap;
        }
    }

    closedir(directory);

    return mapQuantity;
}

void insertionSort(MapInfo *maps, int n)
{
    int i, j;
    MapInfo key;

    for (i = 1; i < n; i++)
    {
        key = maps[i];
        j = i - 1;

        while (j >= 0 && maps[j].mapId > key.mapId)
        {
            maps[j + 1] = maps[j];
            j = j - 1;
        }
        maps[j + 1] = key;
    }
}

int loadMapsInfo(MapInfo **maps)
{
    int numMaps = mapInfoReader(maps);

    if (numMaps < 0)
    {
        printf("Error at loading maps info.\n");
        endwin();
        return -1;
    }

    insertionSort(*maps, numMaps);

    return numMaps;
}