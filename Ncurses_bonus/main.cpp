#include <ncurses.h>
#include <unistd.h>

WINDOW *create_win(int h, int w, int x, int y);

int main() {
    initscr();
    noecho();
    WINDOW* win = create_win(3, 10, 0, 0);
    wrefresh(win);
    sleep(1);
    mvwprintw(win, 1, 2, "Hello");
    wrefresh(win);
    sleep(1);
    WINDOW *win1 = create_win(10, 10, 0, 11);
    WINDOW *win2 = create_win(10, 10, 9, 20);
    wrefresh(win1);
    wrefresh(win2);
    while (1) {
        // werase(win);
        mvwprintw(win, 1, 2, "world");
        wrefresh(win);
        sleep(1);
        wrefresh(win);
        mvwprintw(win, 1, 2, "Hello");
        wrefresh(win);
        sleep(1);
    }
    endwin();
    return 0;
}

WINDOW *create_win(int h, int w, int x, int y) {
    WINDOW *localWin = newwin(h, w, x, y);
    box(localWin, 0, 0);
    wrefresh(localWin);
    return localWin;
}