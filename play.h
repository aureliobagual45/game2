#ifndef _PLAY_H_
#define _PLAY_H_
#include <curses.h>
#include "mapreader.h"

struct
{
    int x;
    int y;
    bool alive;
    bool win;
} player;

int play(MapInfo maps);

#endif