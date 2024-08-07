#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "mapreader.h"
#include "map.h"

int initEngine(MapInfo **maps)
{
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();

    getch();

    int numMaps = loadMaps(maps);

    return numMaps;
}

int main()
{
    MapInfo *maps = NULL;

    int numMaps = initEngine(&maps);

    int menu;

    mvwprintw(stdscr, 10, 10, "mapas: %d", numMaps);
    getch();
    for (int i = 0; i < numMaps; i++)
    {
        mvprintw(10 + i, 10, "Map %d: ID = %d Name = %s Creator = %s Path = %s",
                  i + 1, maps[i].mapId, maps[i].mapName, maps[i].mapCreator, maps[i].mapPath);
    }

    getch();

    getmap(&maps, 2);

    getch();

    while (0)
    {
        // menu = gameMenu();

        switch (menu)
        {
        case 1:
            // playEntireGame();
            break;

        case 2:
            // mapList();
            break;

        case 3:
            // mapCreator(COLOR_PAIR(5));
            break;

        case 4:
            // playTutorial();
            break;
        }
    }

    free(maps);

    getch();

    endwin();
    return 0;
}