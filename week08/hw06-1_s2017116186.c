// 2017116186 jungmin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>

#define BLANK   "  "

double row, col; 
int bar1, bar2;
double top_row, bot_row;
double l_edge, r_edge;
double rowdir, coldir;
int score1, score2;
char symbol[8];
char scorebuf[64];

void quit_handler(int s){
	// do nothing
}

int set_ticker(int n_msecs){
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void move_ball(int signum){
    signal(SIGALRM, move_ball);
    move(row, col);
    addstr(BLANK);
	row += rowdir;
    col += coldir;
    move((int)row, (int)col);
    addstr(symbol);
    refresh();

    if(coldir < 0 && col <= l_edge) coldir = -coldir;
	else if(coldir > 0 && col+1 >= r_edge) coldir = -coldir;

	if(rowdir < 0 && row <= top_row) rowdir = -rowdir;
    else if(rowdir > 0 && row >= bot_row) rowdir = -rowdir;

    return;
}

void move_bar(int player, int dir){
	if(player){
		if(dir > 0){
			move(bar1, 0);
			addstr(" ");
			bar1 = bar1 + dir;
			move(bar1+4, 0);
			addstr("|");
		}
		else{
			move(bar1+4, 0);
			addstr(" ");
			bar1 = bar1 + dir;
			move(bar1, 0);
			addstr("|");
		}
	}
	else{
		if(dir > 0){
			move(bar2, 41);
			addstr(" ");
			bar2 = bar2 + dir;
			move(bar2+4, 41);
			addstr("|");
		}
		else{
			move(bar2+4, 41);
			addstr(" ");
			bar2 = bar2 + dir;
			move(bar2, 41);
			addstr("|");
		}
	}

	refresh();
}

void update_score(){
	sprintf(scorebuf, "<score> %d:%d", score1, score2);
	move(0, 0);
	addstr(scorebuf);

	refresh();
}

int main(int argc, char *argv[]){
    int delay;
    int ndelay;
    int c, i;
    void move_ball(int);
	void quit_handler(int);

	// init
	strcpy(symbol, "o");
	row = 5; col = 0; 
	bar1 = 4; bar2 = 4;
	top_row = 1; bot_row = 10;
	l_edge = 1; r_edge = 40;
	score1 = 0; score2 = 0;

    initscr();
    crmode();
    noecho();
    clear();

    coldir = 1; rowdir = 1;
    delay = 200;

    move(row, col);
    addstr(symbol);

	for(i = 0; i < 4; ++i){
		move(bar1+i, 0);
		addstr("|");
		move(bar2+i, 41);
		addstr("|");
	}

    signal(SIGALRM, move_ball);
	signal(SIGQUIT, quit_handler);
    set_ticker(delay);

    while(true){
        ndelay = 0;
        c = getch();
        if(c == 'Q') break;
		if(c == 'q') {
			if(rowdir < 0 && rowdir < -0.10)	 rowdir += 0.10;
			else if(rowdir > 0 && rowdir > 0.10) rowdir -= 0.10;
		}
		if(c == 'w') {
			if(coldir < 0 && coldir < -0.10)  	 coldir += 0.10;
			else if(coldir > 0 && coldir > 0.10) coldir -= 0.10;
		}
		if(c == 'e') {
			if(rowdir < 0)	 	rowdir -= 0.10;
			else if(rowdir > 0) rowdir += 0.10;
	
		}
		if(c == 'r') {
			if(coldir < 0)	 	coldir -= 0.10;
			else if(coldir > 0) coldir += 0.10;
		
		}
		if(c == 'a') {
			if(bar1 >1) move_bar(1, -1);
		}
		if(c == 's') {
			if(bar1 < 10) move_bar(1, 1);
		}
		if(c == 'k') {
			if(bar2 >1) move_bar(0, -1);
		}
		if(c == 'l') {
			if(bar2 < 10) move_bar(0, 1);
		}
    }
    endwin();
    return 0;
}
