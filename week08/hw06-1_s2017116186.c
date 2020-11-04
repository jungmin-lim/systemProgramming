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
double top_row, bot_row;
double l_edge, r_edge;
double rowdir, coldir;
char symbol[8];

int bar1, bar2;
int score1, score2;
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

void update_score(){
	sprintf(scorebuf, "<score> %d:%d", score1, score2);
	
	if(score1 >= 15){
		sprintf(scorebuf, "<score> %d:%d, player1 wins!", score1, score2);
		move(bot_row+1, l_edge+1);
		addstr(scorebuf);
		refresh();
		sleep(3);
		endwin();
		exit(1);
	}
	else if (score2 >= 15){
		sprintf(scorebuf, "<score> %d:%d player2 wins!", score1, score2);
		move(bot_row+1, l_edge+1);
		addstr(scorebuf);
		refresh();
		sleep(3);
		endwin();
		exit(1);
	}
	move(bot_row+1, l_edge+1);
	addstr(scorebuf);

	refresh();
}


void move_ball(int signum){
    signal(SIGALRM, move_ball);
    move(row, col);
    addstr(" ");
	row += rowdir;
    col += coldir;
    move((int)row, (int)col);
    addstr(symbol);
    refresh();

    if(coldir < 0 && col < l_edge-coldir+2) {
		coldir = -coldir;
		if(!(row < bar1+5 && row >= bar1)){
			score2++;
			move((int)row, (int)col);
			addstr(" ");
			update_score();
			sleep(2);
			row = bar1+2;
			col = l_edge+2;
		}
	}
	else if(coldir > 0 && col > r_edge-coldir-3){
		coldir = -coldir;
		if(!(row < bar2+5 && row >= bar2)){
			score1++;
			move((int)row, (int)col);
			addstr(" ");
			update_score();
			sleep(2);
			row = bar2+2;
			col = r_edge-2;
		}
	}

	if(rowdir < 0 && row < top_row-rowdir+1) rowdir = -rowdir;
    else if(rowdir > 0 && row > bot_row-rowdir-1) rowdir = -rowdir;

    return;
}

void move_bar(int player, int dir){
	if(player){
		if(dir > 0 && bar1 < bot_row - 5){
			move(bar1, l_edge+1);
			addstr(" ");
			bar1 = bar1 + dir;
			move(bar1+4, l_edge+1);
			addstr("|");
		}
		else if(dir < 0 && bar1 > top_row + 1){
			move(bar1+4, l_edge+1);
			addstr(" ");
			bar1 = bar1 + dir;
			move(bar1, l_edge+1);
			addstr("|");
		}
	}
	else{
		if(dir > 0 && bar2 < bot_row - 5){
			move(bar2, r_edge-1);
			addstr(" ");
			bar2 = bar2 + dir;
			move(bar2+4, r_edge-1);
			addstr("|");
		}
		else if(dir < 0 && bar2 > top_row + 1){
			move(bar2+4, r_edge-1);
			addstr(" ");
			bar2 = bar2 + dir;
			move(bar2, r_edge-1);
			addstr("|");
		}
	}

	refresh();
}
		
int main(int argc, char *argv[]){
    int delay;
    int ndelay;
    int c, i;
    void move_ball(int);
	void quit_handler(int);

	if(argc != 8){
		fprintf(stderr, "Usage: %s <symbol> <ball_start_col> <ball_start_row> \
				<top_row> <bot_row> <l_edge> <r_edge>\n", argv[0]);
		exit(-1);

	}

	strcpy(symbol, argv[1]);
	col = atoi(argv[2]);
	row = atoi(argv[3]);

	top_row = atoi(argv[4]);
	bot_row = atoi(argv[5]);

	l_edge = atoi(argv[6]);
	r_edge = atoi(argv[7]);

	bar1 = (top_row + bot_row) / 2;
	bar2 = (top_row + bot_row) / 2;

	score1 = 0; score2 = 0;

    initscr();
    crmode();
    noecho();
    clear();

	for(i = l_edge; i <= r_edge; ++i){
		move(top_row, i);
		addstr("-");
		move(bot_row, i);
		addstr("-");
	}

	for(i = top_row+1; i < bot_row; ++i){
		move(i, l_edge);
		addstr("|");
		move(i, r_edge);
		addstr("|");
	}

	update_score();

    coldir = 1; rowdir = 1;
    delay = 200;

    move(row, col);
    addstr(symbol);

	for(i = 0; i < 4; ++i){
		move(bar1+i, l_edge+1);
		addstr("|");
		move(bar2+i, r_edge-1);
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
			move_bar(1, -1);
		}

		if(c == 's') {
			move_bar(1, 1);
		}

		if(c == 'k') {
			move_bar(0, 1);
		}

		if(c == 'l') {
			move_bar(0, -1);
		}
    }
    endwin();
    return 0;
}
