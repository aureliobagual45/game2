#ifndef _MAPREADER_H_
#define _MAPREADER_H_

typedef struct MapInfo
{
    int mapId;
    char mapName[50];
    char mapCreator[50];
    char mapPath[100];
    int mapSize_Y;
    int mapSize_X;
} MapInfo;

MapInfo newmap(char direction[100]);

int mapReader(MapInfo **maps);

int loadMaps(MapInfo **maps);

#endif