// 2017116186 jungmin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#define LEFTEDGE    10
#define RIGHTEDGE   30
#define ROW         10

int main(int argc, char *argv[]){
    char message[]  = "hello";
    char blank[]    = "      "; 
    int dir         = 1;
    int pos         = LEFTEDGE;

    initscr();
    clear();
    while(1){
        move(ROW, pos);
        addstr(message);
        move(LINES-1, COLS-10);
        refresh();
        sleep(1);

        move(ROW, pos);
        addstr(blank);
        pos += dir;

        if(pos >= RIGHTEDGE){
            dir -= 1;
        }
        if(pos <= LEFTEDGE){
            dir += 1;
        }
    }
    return 0;
}
