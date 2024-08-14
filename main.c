#include <stdlib.h>
#include <curses.h>
#include "maplist.h"
#include "mapreader.h"
#include "mapcreator.h"
#include "map.h"
#include "menu.h"

int initEngine(MapInfo **maps)
{
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    start_color();

    int num_maps = loadMapsInfo(maps);

    return num_maps;
}

int main()
{
    MapInfo *maps = NULL;

    int num_maps = initEngine(&maps);
    int menu;
    bool running = TRUE;

    while (running)
    {
        menu = gameMenu();

        switch (menu)
        {
        case 0:
            mapList(maps, num_maps);
            num_maps = loadMapsInfo(&maps);
            break;

        case 1:
            mapCreator(num_maps);
            num_maps = loadMapsInfo(&maps);
            break;

        case 2:
            running = FALSE;
            break;
        }
    }

    free(maps);

    getch();

    endwin();
    return 0;
}