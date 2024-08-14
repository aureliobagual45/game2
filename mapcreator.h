#ifndef _MAPCREATOR_H_
#define _MAPCREATOR_H_

typedef struct
{
    int height;
    int width;
} Dimensions;

typedef struct
{
    int y;
    int x;
} Start;

typedef enum
{
    LEVEL,
    CORRIDOR,
    BOSSFIGHT,
    UNDEFINED_TYPE = -1
} LevelType;

typedef struct Level
{
    char **grid;
    char name[50];
    char filename[50];
    char path[60];
    Dimensions dimension;
    Start start;
    LevelType type;
} Level;

Dimensions get_dimensions(LevelType type);

int mapCreator(int num_maps);

#endif