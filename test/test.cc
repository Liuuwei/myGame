//
// Created by Liuwei on 2023/9/11.
//
#include <ncurses.h>
#include <unistd.h>

int main() {
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    mvaddch(5 ,5, ACS_CKBOARD);
    refresh();

    sleep(1);

    endwin();
}