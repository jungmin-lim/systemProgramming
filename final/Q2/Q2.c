// 2017116186 jungmin lim

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PIPE_ENDS 2
#define oops(m, x) { perror(m); exit(x);}

int main( int argc, char* argv[] ) {
	int thepipe1[PIPE_ENDS];
	int thepipe2[PIPE_ENDS];
	int thepipe3[PIPE_ENDS];
	int thepipe4[PIPE_ENDS];
	int thepipe5[PIPE_ENDS];

	int pid1, pid2, pid3, pid4, pid5;
	int newfd;

	if( argc < 2 ) {
		fprintf( stderr, "Usage: %s <argument1> <argument2> ... <argument5>\n", argv[0] );
		exit(1);
	}

	if( argc > 2 ) {
		if( pipe( thepipe1 ) == -1 ) {
			oops( "cannot get a pipe 1", 1 );
		}
		if( ( pid1 = fork () ) == -1 ) {
			oops( "cannot fork 1", 2 );
		}

		if( pid1 > 0 ) {
			if( argc > 3 ) {
				if( pipe( thepipe2 ) == -1 ) {
					oops( "cannot get a pipe 2", 1 );
				}
				if( ( pid2 = fork() ) == -1 ) {
					oops( "cannot fork 2", 2 );
				}

				if( pid2 > 0 ) {
					if( argc > 4 ) {
						if( pipe( thepipe3 ) == -1 ) {
							oops( "cannot get a pipe 3", 1 );
						}
						if( ( pid3 = fork() ) == -1 ) {
							oops( "cannot fork 3", 2 );
						}

						if( pid3 > 0 ) {
							close( thepipe3[1] );
							if( dup2( thepipe3[0], 0 ) == -1 ) {
								oops( "couldn't redirect stdin 3", 3 );
							}
							close( thepipe3[0] );
							close( thepipe2[1] );
							close( thepipe2[0] );
							close( thepipe1[1] );
							close( thepipe1[0] );

							if( argc == 5 ) execlp( argv[4], argv[4], NULL );
							else execlp( argv[4], argv[4], argv[5], NULL );
							oops( argv[4], 5 );
						}

						close( thepipe3[0] );
						if( dup2( thepipe3[1], 1 ) == -1 ) {
							oops( "couldn't redirect stdout 3", 4 );
						}
						close( thepipe3[1] );
					}

					close( thepipe2[1] );
					if( dup2( thepipe2[0], 0 ) == -1 ) {
						oops( "couldn't redirect stdout 2", 4 );
					}
					close( thepipe2[0] );
					close( thepipe1[1] );
					close( thepipe1[0] );
					
					execlp( argv[3], argv[3], NULL );
					oops( argv[3], 5 );
				}

				close( thepipe2[0] );
				if( dup2(thepipe2[1] , 1 ) == -1 ) {
					oops( "couldn't redirect stdout 2", 4 );
				}
				close( thepipe2[1] );
			}

			close( thepipe1[1] );
			if( dup2( thepipe1[0], 0 ) == -1 ) {
				oops( "couldn't redirect stdout 1", 4 );
			}
			close( thepipe1[0] );

			execlp( argv[2], argv[2], NULL );
			oops( argv[2], 5 );
		}

		close( thepipe1[0] );
		if( dup2( thepipe1[1], 1 ) == -1 ) {
			oops( "couldn't redirect stdout 1 ", 4 );
		}
		close( thepipe1[1] );

		execlp(argv[1], argv[1], NULL );
		oops( argv[1], 5 );
	}

	else{
		execlp(argv[1], argv[1], NULL );
		oops( argv[1], 5 );
	}

	return 0;
}
