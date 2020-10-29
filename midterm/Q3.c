// 2017116186 jungmin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

time_t now;
struct tm t;

void calendar();
int main(int argc, char *argv[]){

	now = time(NULL);
	t = *localtime(&now);


	// init
	initscr();
	start_color();
	use_default_colors();
	clear();

	// color pair
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(4, COLOR_RED, COLOR_WHITE);

	attrset(COLOR_PAIR(1));

	// name
	move(0, 0);
	addstr("2017116186 Jungmin Lim                                          ");

	// command
	move(1, 0);
	addstr("[Q] Quit  [<-] Previos month  [Space Bar] Today  [->] Next month");

	refresh();

	sleep(1);
	endwin();

	return 0;
}

void calendar(){
}
