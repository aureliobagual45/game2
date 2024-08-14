#include "play.h"
#include "map.h"
#include <stdlib.h>
#include <curses.h>

int gameOver(WINDOW *ww)
{
    wclear(ww);
    wrefresh(ww);

    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2);
    y_center = (y / 2);

    WINDOW *w = newwin(6, 20, y_center - 3, x_center - 10);
    nodelay(w, FALSE);
    nodelay(stdscr, FALSE);

    int k = 0, choice = 0;
    while (!(k == 27 || k == 10))
    {
        box(w, 0, 0);
        mvwprintw(w, 1, 1, "    GAME  OVER    ");
        mvwprintw(w, 3, 1, "ENTER to try again");
        mvwprintw(w, 4, 1, "  ESC to give up  ");
        wrefresh(w);

        k = wgetch(w);

        if (k == 27)
            choice = FALSE;
        else if (k == 10)
            choice = TRUE;
    }

    nodelay(stdscr, TRUE);
    wclear(w);
    wrefresh(w);
    delwin(w);
    refresh();
    return choice;
}

int gameWon(WINDOW *ww)
{
    wclear(ww);
    wrefresh(ww);

    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2);
    y_center = (y / 2);

    WINDOW *w = newwin(6, 21, y_center - 3, x_center - 11);
    nodelay(w, FALSE);
    nodelay(stdscr, FALSE);

    int k = 0, choice = 0;
    while (!(k == 27 || k == 10))
    {
        box(w, 0, 0);
        mvwprintw(w, 1, 1, "     YOU   WIN     ");
        mvwprintw(w, 3, 1, "ENTER to play again");
        mvwprintw(w, 4, 1, "    ESC to quit    ");
        wrefresh(w);

        k = wgetch(w);

        if (k == 27)
            choice = FALSE;
        else if (k == 10)
            choice = TRUE;
    }

    nodelay(stdscr, TRUE);
    wclear(w);
    wrefresh(w);
    delwin(w);
    refresh();
    return choice;
}

void input(char **map)
{
    int key = getch();

    int new_y = player.y;
    int new_x = player.x;

    switch (key)
    {
    case KEY_UP:
        if (map[player.y - 1][player.x] != '#')
            new_y--;
        break;

    case KEY_DOWN:
        if (map[player.y + 1][player.x] != '#')
            new_y++;
        break;

    case KEY_LEFT:
        if (map[player.y][player.x - 1] != '#')
            new_x--;
        break;

    case KEY_RIGHT:
        if (map[player.y][player.x + 1] != '#')
            new_x++;
        break;

    case 27:
        player.alive = FALSE;
        break;
    }
    if (map[new_y][new_x] == '$')
        player.alive = FALSE;
    else if (map[new_y][new_x] == '%')
        player.win = TRUE;
    else
    {
        player.y = new_y;
        player.x = new_x;
    }
}

void output(WINDOW *w, MapInfo map_info, char **map)
{
    for (int i = 0; i < map_info.mapSize_Y; i++)
    {
        for (int j = 0; j < map_info.mapSize_X; j++)
        {
            switch (map[i][j])
            {
            case ' ':
                wattron(w, COLOR_PAIR(1));
                break;

            case '#':
                wattron(w, COLOR_PAIR(2));
                break;

            case '$':
                wattron(w, COLOR_PAIR(3));
                break;

            case '%':
                wattron(w, COLOR_PAIR(4));
                break;
            }
            mvwprintw(w, i, j * 2, "%c%c", map[i][j], map[i][j]);

            wattroff(w, COLOR_PAIR(1));
            wattroff(w, COLOR_PAIR(2));
            wattroff(w, COLOR_PAIR(3));
            wattroff(w, COLOR_PAIR(4));
        }
    }
    wattron(w, COLOR_PAIR(5));
    mvwprintw(w, player.y, player.x * 2, "[]");
    wattroff(w, COLOR_PAIR(5));
    wrefresh(w);
}

int play(MapInfo map_info)
{
    player.alive = TRUE;
    player.win = FALSE;
    player.y = map_info.start_Y;
    player.x = map_info.start_X;
    char **map = loadMap(map_info);
    map[player.y][player.x] = ' ';

    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2) - map_info.mapSize_X;
    y_center = (y / 2) - (map_info.mapSize_Y / 2);

    WINDOW *w = newwin(map_info.mapSize_Y, map_info.mapSize_X * 2, y_center, x_center);
    nodelay(w, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, COLOR_MAGENTA, COLOR_YELLOW);

    while (player.alive && !player.win)
    {
        input(map);

        output(w, map_info, map);
    }

    int running;
    if (!player.alive)
        running = gameOver(w);
    else
        running = gameWon(w);

    for (int i = 0; i < map_info.mapSize_Y; i++)
        free(map[i]);
    free(map);

    wclear(w);
    wrefresh(w);
    delwin(w);
    refresh();
    return running;
}