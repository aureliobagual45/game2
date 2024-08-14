#ifndef _MAPREADER_H_
#define _MAPREADER_H_

typedef struct MapInfo
{
    int mapId;
    char mapName[50];
    char mapPath[100];
    int mapSize_Y;
    int mapSize_X;
    int start_Y;
    int start_X;
} MapInfo;

int loadMapsInfo(MapInfo **maps);

#endif