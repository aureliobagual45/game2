#include <curses.h>

int gameMenu()
{
    int y, x, y_center, x_center;
    getmaxyx(stdscr, y, x);
    x_center = (x / 2);
    y_center = (y / 2);

    WINDOW *creator = newwin(1, 20, y_center + 3, x_center + 1);
    wattron(creator, A_BOLD);
    wprintw(creator, "por Aurelio Sampaio");
    wattroff(creator, A_BOLD);
    wrefresh(creator);

    WINDOW *name = newwin(3, 20, y_center - 7, x_center - 10);
    wattron(name, A_BOLD);
    box(name, 0, 0);
    mvwprintw(name, 1, 1, "   BAGUAL  GAME   ");
    wattroff(name, A_BOLD);
    wrefresh(name);

    WINDOW *menu_win = newwin(5, 14, y_center - 3, x_center - 7);
    box(menu_win, 0, 0);
    keypad(menu_win, TRUE);

    char *options[] = {"  Map List  ", "Map Creator ", "    Exit    "};
    int highlight = 0, key;

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (i == highlight)
                wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, i + 1, 1, options[i]);
            wattroff(menu_win, A_REVERSE);
        }
        wrefresh(menu_win);

        key = wgetch(menu_win);

        switch (key)
        {
        case KEY_UP:
            if (highlight != 0)
                highlight--;
            break;

        case KEY_DOWN:
            if (highlight != 2)
                highlight++;
            break;
        }
        if (key == 10)
            break;
    }
    wclear(menu_win);
    wrefresh(menu_win);
    delwin(menu_win);
    refresh();
    return highlight;
}