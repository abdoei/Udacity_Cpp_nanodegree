#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "util.hpp"
#include "SysInfo.hpp"
#include "ProcessesContainer.hpp"

using namespace std;

char *getCString(std::string str)
{
    char *cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    return cstr;
}

void writeSysInfoToConsole(SysInfo sys, WINDOW *sys_win)
{
    sys.setAttributes();

    mvwprintw(sys_win, 2, 2, "OS: %s", getCString(sys.getOsName()));
    mvwprintw(sys_win, 3, 2, "Kernel version: %s", getCString( sys.getKernelVersion()));
    mvwprintw(sys_win, 5, 2, "%s",getCString("CPU:    "));
    wattron(sys_win, COLOR_PAIR(2));
    wprintw(sys_win, "%s", getCString(Util::getProgressBar(sys.getCpuPercent())));
    wattroff(sys_win, COLOR_PAIR(2));
    mvwprintw(sys_win, 6, 2, "%s", getCString(("Other cores:")));
    wattron(sys_win, COLOR_PAIR(2));
    std::vector<std::string> val = sys.getCoresStats();

    for (int i = 0; i < val.size(); i++)
    {
        mvwprintw(sys_win, (7 + i), 2, "%s", getCString(val[i]));
    }

    wattroff(sys_win, COLOR_PAIR(2));
    mvwprintw(sys_win, 11, 2, "%s", getCString(("Memory: ")));
    wattron(sys_win, COLOR_PAIR(2));
    wprintw(sys_win, "%s", getCString(Util::getProgressBar(sys.getMemPercent())));
    wattroff(sys_win, COLOR_PAIR(2));
    mvwprintw(sys_win, 12, 2, "Total Processes: %s", getCString((sys.getTotalProc())));
    mvwprintw(sys_win, 13, 2, "Running Processes: %s", getCString((sys.getRunningProc())));
    mvwprintw(sys_win, 14, 2, "Up Time: %s", getCString((Util::convertToTime(sys.getUpTime()))));
    wrefresh(sys_win);
}

void getProcessListToConsole(std::vector<string> processes, WINDOW *win) {
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 2, "PID:");
    mvwprintw(win, 1, 12, "User:");
    mvwprintw(win, 1, 22, "CPU[%%]:  ");
    mvwprintw(win, 1, 31, "RAM[MB]:");
    mvwprintw(win, 1, 40, "Uptime:");
    mvwprintw(win, 1, 49, "CMD:");
    wattroff(win, COLOR_PAIR(2));

    for (int i = 0; i < 10; i++) {
        mvwprintw(win, 2 + i, 2, "%s", getCString(processes[i]));
    }
}

void printMain(SysInfo sys, ProcessesContainer procs)
{
    initscr();     /* Start curses mode 		  */
    noecho();      // not printing input values
    cbreak();      // Terminating on classic ctrl + c
    start_color(); // Enabling color change of text
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax); // getting size of window measured in lines and
                                  // columns (column one char length)
    WINDOW *sys_win = newwin(17, xMax - 1, 0, 0);
    WINDOW *proc_win = newwin(15, xMax - 1, 18, 0);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    int counter = 0;

    while (1)
    {
        try
        {
            box(sys_win, 0, 0);
            box(proc_win, 0, 0);
            procs.refreshList();
            std::vector<std::string> processes = procs.getList();
            writeSysInfoToConsole(sys, sys_win);
            getProcessListToConsole(processes, proc_win);
            wrefresh(sys_win);
            wrefresh(proc_win);
            refresh();
            sleep(1);
            // if (counter == (processes.size() - 1))
            // {
            //     counter = 0;
            // }
            // else
            // {
            //     counter++;
            // }
        }
        catch (...)
        {
        }
    }
    endwin();
}

int main(int argc, char *argv[])
{
    // Object which contains list of current processes, Container for Process Class
    ProcessesContainer procs;
    // Object which containts relevant methods and attributes regarding system details
    SysInfo sys;
    // std::string s = writeToConsole(sys);
    printMain(sys, procs);
    return 0;
}