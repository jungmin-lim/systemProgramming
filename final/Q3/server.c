#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>

#define BUFSIZE 1024
#define   PORTNUM  13000 
#define   HOSTLEN  256
#define   oops(msg) { perror(msg) ; exit(1) ; }

void send_memory_usage(int sock);
void send_cpu_usage(int sock);

int main(int ac, char *av[]){
	struct  sockaddr_in saddr;
	struct	hostent	*hp;
	char	hostname[HOSTLEN]; 
	char msg[BUFSIZE];
	int	sock_id,sock_fd;
	int str_len;

	fprintf(stdout, "------ 2017116186 jungmin lim ------\n");
	sock_id = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sock_id == -1 ) 
	    oops( "socket" );

	bzero( (void *)&saddr, sizeof(saddr) ); 

	gethostname( hostname, HOSTLEN );
	hp = gethostbyname( hostname );

	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET ;

	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

	if ( listen(sock_id, 1) != 0 ) 
		oops( "listen" );

	while ( 1 ){
	    sock_fd = accept(sock_id, NULL, NULL);
	    if ( sock_fd == -1 )
		    oops( "accept" );

		while(1){
			str_len = read(sock_fd, msg, sizeof(msg));
			if(str_len <= 0){
				fprintf(stdout, "sock is closed!\n");
				close(sock_fd);
				exit(1);
			}
			msg[str_len] = '\0';

			if(!strcmp(msg, "memory")){
				send_memory_usage(sock_fd);
			}
			if(!strcmp(msg, "Q")){
				fprintf(stdout, "sock is closed\n");
				close(sock_fd);
				exit(1);
			}
			if(!strcmp(msg, "cpu")){
				fprintf(stdout, "received cpu\n");
				send_cpu_usage(sock_fd);
			}
		}
		close(sock_fd);

    }
	return 0;
}

void send_memory_usage(int sock){
	FILE* meminfo;
	char membuf[BUFSIZE], line[BUFSIZE];
	int total, avail, free, i = 0;

	meminfo = fopen("/proc/meminfo", "r");
	if(meminfo == NULL){
		oops("fopen");
	}

	while(i < 3){
		fgets(line, sizeof(line), meminfo);
		if(i == 0){
			sscanf(line, "MemTotal: %d kB", &total);
		}
		if(i == 1){
			sscanf(line, "MemFree: %d kB", &free);
		}
		if(i == 2){
			sscanf(line, "MemAvailable: %d kB", &avail);
		}
		i++;
	}
	fclose(meminfo);

	sprintf(membuf, "Memory utilization : %.6f%%\n", ((float)(total-avail) / (float)total) * 100.0);
	write(sock, membuf, strlen(membuf));

	return;
}

void send_cpu_usage(int sock){
	FILE* statinfo;
	char cpubuf[BUFSIZE], line[BUFSIZE];
	int prev_user, prev_nice, prev_system, prev_idle, prev_iowait, prev_irq, prev_softirq, prev_steal, prev_guest, prev_guest_nice;
	int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
	int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
	float CPU_Percentage;

	statinfo = fopen("/proc/stat", "r");
	if(statinfo == NULL){
		oops("fopen");
	}

	fgets(line, sizeof(line), statinfo);
	fclose(statinfo);

	sscanf(line, "cpu %d %d %d %d %d %d %d %d %d %d",
		&prev_user, &prev_nice, &prev_system, &prev_idle, &prev_iowait, &prev_irq, &prev_softirq, &prev_steal, &prev_guest, &prev_guest_nice);

	statinfo = fopen("/proc/stat", "r");
	if(statinfo == NULL){
		oops("fopen");
	}

	fgets(line, sizeof(line), statinfo);
	fclose(statinfo);

	sscanf(line, "cpu %d %d %d %d %d %d %d %d %d %d",
		&user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

	PrevIdle = prev_idle + prev_iowait;
	Idle = idle + iowait;

	PrevNonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
	NonIdle = user + nice + system + irq + softirq + steal;

	PrevTotal = PrevIdle + PrevNonIdle;
	Total = Idle + NonIdle;

	totald = Total - PrevTotal;
	idled = Idle - PrevIdle;

	CPU_Percentage = (float)(totald - idled)/totald;

	sprintf(cpubuf, "cpu utilization : %.6f%%\n", (float)user / (float)idle);
	write(sock, cpubuf, strlen(cpubuf));

	return;
}