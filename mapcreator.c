#include "mapcreator.h"
#include "mapreader.h"
#include <curses.h>
#include <stdlib.h>

struct Level level;
int cursor_x, cursor_y;
bool running, aux;

Dimensions getDimensions(LevelType type)
{
    switch (type)
    {
    case LEVEL:
        return (Dimensions){10, 15};
    case CORRIDOR:
        return (Dimensions){18, 22};
    case BOSSFIGHT:
        return (Dimensions){24, 30};
    case UNDEFINED_TYPE:
        return (Dimensions){0, 0};
    }
    return (Dimensions){0, 0};
}

void reset()
{
    level.grid = NULL;
    level.type = UNDEFINED_TYPE;
    cursor_x = 0;
    cursor_y = 0;
    running = TRUE;
    aux = FALSE;
}

void handleInput()
{
    int key = getch();

    switch (key)
    {
    case KEY_UP:
        if (cursor_y > 0)
            cursor_y--;
        break;
    case KEY_DOWN:
        if (cursor_y < level.dimension.height - 1)
            cursor_y++;
        break;
    case KEY_LEFT:
        if (cursor_x > 0)
            cursor_x -= 1;
        break;
    case KEY_RIGHT:
        if (cursor_x < level.dimension.width - 1)
            cursor_x += 1;
        break;
    case KEY_BACKSPACE:
    case ' ':
        level.grid[cursor_y][cursor_x] = ' ';
        break;
    case '@':
    case '2':
        if (aux)
            level.grid[level.start.y][level.start.x] = ' ';
        aux = TRUE;
        level.start.y = cursor_y;
        level.start.x = cursor_x;
        level.grid[cursor_y][cursor_x] = '@';
        break;
    case '#':
    case '3':
        level.grid[cursor_y][cursor_x] = '#';
        break;
    case '$':
    case '4':
        level.grid[cursor_y][cursor_x] = '$';
        break;
    case '%':
    case '5':
        level.grid[cursor_y][cursor_x] = '%';
        break;
    case 'S':
    case 's':
        running = false;
        break;
    }
}

void drawGrid(WINDOW *w)
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, COLOR_MAGENTA, COLOR_YELLOW);

    werase(w);
    box(w, 0, 0);

    for (int y = 0; y < level.dimension.height; y++)
    {
        for (int x = 0; x < level.dimension.width; x++)
        {
            switch (level.grid[y][x])
            {
            case ' ':
                wattron(w, COLOR_PAIR(1));
                mvwprintw(w, y + 1, x * 2 + 1, "  ");
                wattroff(w, COLOR_PAIR(1));
                break;

            case '@':
                wattron(w, COLOR_PAIR(5));
                mvwprintw(w, y + 1, x * 2 + 1, "@@");
                wattroff(w, COLOR_PAIR(5));
                break;

            case '#':
                wattron(w, COLOR_PAIR(2));
                mvwprintw(w, y + 1, x * 2 + 1, "##");
                wattroff(w, COLOR_PAIR(2));
                break;

            case '$':
                wattron(w, COLOR_PAIR(3));
                mvwprintw(w, y + 1, x * 2 + 1, "$$");
                wattroff(w, COLOR_PAIR(3));
                break;

            case '%':
                wattron(w, COLOR_PAIR(4));
                mvwprintw(w, y + 1, x * 2 + 1, "%%%%");
                wattroff(w, COLOR_PAIR(4));
                break;

            default:
                mvwprintw(w, y + 1, x + 1, "%c", level.grid[y][x]);
                break;
            }
        }
    }
    wattron(w, A_REVERSE);
    wattron(w, COLOR_PAIR(5));
    mvwprintw(w, cursor_y + 1, cursor_x * 2 + 1, "[]");
    wattroff(w, COLOR_PAIR(5));
    wattroff(w, A_REVERSE);
    wrefresh(w);
}

void saveMap(char *filename, int num_maps)
{
    FILE *file = fopen(filename, "w");

    num_maps++;
    fprintf(file, "%d\n", num_maps);
    fprintf(file, "%s\n", level.name);
    fprintf(file, "%d %d\n", level.dimension.height, level.dimension.width);
    fprintf(file, "%d %d\n", level.start.y, level.start.x);

    for (int y = 0; y < level.dimension.height; ++y)
    {
        for (int x = 0; x < level.dimension.width; ++x)
            fprintf(file, "%c", level.grid[y][x]);
        if (y < level.dimension.height - 1)
            fprintf(file, "\n");
    }

    fclose(file);
}

int mapCreator(int num_maps)
{
    reset();

    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2);
    y_center = (y / 2);

    WINDOW *mapcreator_win = newwin(20, 40, y_center - 10, x_center - 20);
    box(mapcreator_win, 0, 0);
    keypad(mapcreator_win, TRUE);

    int k;
    box(mapcreator_win, 0, 0);
    while (level.type < 0 || level.type > 2)
    {
        mvwprintw(mapcreator_win, 1, 1, "Choose the level size:");
        mvwprintw(mapcreator_win, 2, 1, "(1) Tiny");
        mvwprintw(mapcreator_win, 3, 1, "(2) Normal");
        mvwprintw(mapcreator_win, 4, 1, "(3) Big");
        k = wgetch(mapcreator_win);
        if (k == 27)
        {
            wclear(mapcreator_win);
            wrefresh(mapcreator_win);
            delwin(mapcreator_win);
            refresh();
            return 0;
        }
        level.type = k - 49;
    }
    wclear(mapcreator_win);
    wrefresh(mapcreator_win);

    level.dimension = getDimensions(level.type);

    ////

    WINDOW *cursor_win = newwin(level.dimension.height + 2, level.dimension.width * 2 + 2, y_center - (level.dimension.height / 2) - 1, x_center - level.dimension.width - 1);
    box(mapcreator_win, 0, 0);

    level.grid = malloc(level.dimension.height * sizeof(char *));
    for (int i = 0; i < level.dimension.height; i++)
    {
        level.grid[i] = malloc(level.dimension.width * sizeof(char));

        for (int j = 0; j < level.dimension.width; j++)
            level.grid[i][j] = ' ';
    }

    while (running)
    {
        handleInput();
        drawGrid(cursor_win);
    }

    echo();
    curs_set(TRUE);
    wclear(cursor_win);
    wrefresh(cursor_win);

    mvwprintw(mapcreator_win, 1, 1, "Enter map name:");
    wmove(mapcreator_win, 2, 1);
    wgetnstr(mapcreator_win, level.name, 50);

    wclear(mapcreator_win);
    box(mapcreator_win, 0, 0);
    wrefresh(mapcreator_win);
    mvwprintw(mapcreator_win, 1, 1, "Enter map file name:");
    wmove(mapcreator_win, 2, 1);
    wgetnstr(mapcreator_win, level.filename, 50);
    for (int i = 0; level.filename[i] != '\0'; i++)
    {
        if (level.filename[i] == ' ')
        {
            level.filename[i] = '_';
        }
    }
    sprintf(level.path, "maps/%s.txt", level.filename);

    noecho();
    curs_set(FALSE);

    saveMap(level.path, num_maps);

    for (int i = 0; i < level.dimension.height; i++)
        free(level.grid[i]);
    free(level.grid);

    wclear(cursor_win);
    wrefresh(cursor_win);
    wclear(mapcreator_win);
    wrefresh(mapcreator_win);
    delwin(cursor_win);
    delwin(mapcreator_win);
    refresh();
    return 0;
}