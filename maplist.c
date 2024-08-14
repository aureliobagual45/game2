#include "maplist.h"
#include "play.h"
#include "mapcreator.h"
#include <stdlib.h>
#include <curses.h>

void mapList(MapInfo *maps, int num_maps)
{
    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2);
    y_center = (y / 2);

    WINDOW *w = newwin(22, 52, y_center - 11, x_center - 26);
    keypad(w, TRUE);

    int highlight = 0, line = 0, key = 0, n;

    while (1)
    {
        box(w, 0, 0);

        for (int i = 0; i < 20; i++)
        {
            if (i == highlight)
                wattr_on(w, A_REVERSE, NULL);

            if (i < num_maps)
                mvwprintw(w, 1 + i, 1, "%-*s", 50, maps[line + i].mapName);

            wattr_off(w, A_REVERSE, NULL);
        }

        key = wgetch(w);

        switch (key)
        {
        case KEY_UP:
            if (highlight > 0)
                highlight--;
            else if (line > 0)
                line--;
            break;

        case KEY_DOWN:
            if (highlight < num_maps - 1)
            {
                if (highlight < 19)
                    highlight++;
                else if (line < num_maps - 20)
                    line++;
            }
            break;

        case 10:
            wclear(w);
            wrefresh(w);
            n = line + highlight;
            bool running = TRUE;
            while (running)
            {
                running = play(maps[n]);
            }
            break;
        }

        if (key == 27)
            break;
    }

    wclear(w);
    wrefresh(w);
    delwin(w);
    refresh();
}