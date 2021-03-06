#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"noline.h"

char board[100][100];
int turn = 1; /*Keeps track of which turn it is;*/


/*Command-line Arguments*/
	
int dim; /*Denotes Side-length of Grid*/
int playerXtype;
int playerOtype;
char* Oin = "-";
char* Oout = "-";
char* Xin = "-";
char* Xout = "-";


void setup_board(int dim) {
	int x,y;
	for (x=0; x<dim; x++) {
		for (y=0; y<dim; y++) {
			board[x][y] = '.';
		}
	}
}


void print_board(void) {
	int i,x,y;
	
	for (i=0;i<dim;i++)
		printf("-");
	printf("\n");
	
	for (x=0; x<dim; x++) {
		for(y=0; y<dim; y++) {
			printf("%c", board[x][y]);
		}
		printf("\n");
	}
	for (i=0;i<dim;i++)
		printf("=");
	printf("\n");
}

void insert_number(char ch, int x, int y) {
	if (x > dim || y > dim) {
		/*Do Nothing*/
	}
	else {
		board[x][y] = ch;	
	}
}

char get_char(void) {
	if (turn%2==1) {
		return '0';
	}
	else {
		return 'X';
	}
}

/*Returns 1 if input is valid, else returns 0*/
int board_input_is_valid(int x, int y) {
	/*Checks that values are within scope for the board size*/	
	if ((x < dim) && (x >= 0) &&(y < dim) && (y >= 0)) {
		return 1;
	}else {
		return 0;
	}
}

/*Prints the relevant error message to stderr*/
void print_err(int exit_status) {
	switch(exit_status) {
		case 1:
			fprintf(stderr,"Usage: noline dim [playerXtype [playerOtype [Oin Oout Xin Xout]]]\n");
			break;
		case 2:
			fprintf(stderr,"Invalid board dimension.\n");
			break;
		case 3:
			fprintf(stderr,"Invalid player type.\n");
			break;
		case 4:
			fprintf(stderr,"Invalid files\n");
			break;
	}
}


/*Prints the relevant error message and quits the game*/
void end_game(int game_over_status, char player) {
	switch(game_over_status) {
		case 0:
			printf("The game is a draw.\n");
		case 1:
			printf("Player %c loses", player);
		case 2:
			printf("Player %c loses due to EOF", player);
	}
	exit(0);
}

void cmd_in(int argc, const char* argv[]) {
	int i, pt;

	for (i=0; i<argc; i++) {
		/*dim*/
		pt = atoi(argv[i]);
		
		
		if ((i==1)) {
			if ((pt%2==1) && (pt > 2)) {
				dim = atoi(argv[i]);
			}
			else {
				/*Invalid Board Dimension*/
				print_err(2);
				exit(0);
			}
		}
		
		/*PlayerXtype & Player O type*/
		if ((i==2) || (i==3)) {
			if ((pt == 0) || (pt == 1) || (pt == 2)) {
				if (i==2) {
					playerXtype = pt;
				}
				else {
					playerOtype = pt;
				}
			}
			else {
				print_err(3);
				exit(0);
			}
		}
		
		/*Oin*/
		if (i==3) {
			if (strcmp(argv[i],"-") == 0) {
				Oin = "stdin";
			}
			else {
				Oin = argv[i];
			}
		}
		
		/*Oout*/
		if (i==4) {
			if (strcmp(argv[i],"-") == 0) {
				Oout = "stdout";
			}
			else {
				Oout = argv[i];
			}
		}
		
		/*Xin*/
		if (i==6) {
			if (strcmp(argv[i],"-") == 0) {
				Xin = "stdin";
			}
			else {
				Xin = argv[i];
			}
		}
		/*Xout*/
		if (i==7) {
			if (strcmp(argv[i],"-") == 0) {
				Oout = "stdout";
			}
			else {
				Oout = argv[i];
			}
		}
	}
}

void print_args(void) {
	printf ("%d %d %d %s %s %s %s\n", dim, playerXtype, playerOtype, Oin, Oout, Xin, Xout);
}

/*Detects a horizontal line from selection and returns 1 if match, else returns 0*/
int horizontal_line(char ch, int x, int y) {
	/*if left edge*/
	if (x == 0) {
		printf("Left Edge\n");
		if ((board[x+1][y] == ch) && (board[x+2][y] == ch)) {
			return 1;
		}
	}
	/*if not edge*/
	else if ((x>0) && (x<dim)) {
		printf("Not Edge\n");
		if ((board[x-1][y] == ch) && (board[x+1][y] == ch)) {
			return 1;
		}
		if (x>1) {
			if ((board[x-1][y] == ch) && (board[x-2][y] == ch)) {
				return 1;
			}
		}
		if (x<dim-1) {
			if ((board[x+1][y] == ch) && (board[x+2][y] == ch)) {
				return 1;
			}
		}
	}
	/*if right edge*/
	else if (x == dim) {
		printf("Right Edge\n");
		if ((board[x-1][y] == ch) && (board[x-2][y] == ch)) {
			return 1;
		}
	}
	/*Otherwise*/
	return 0;
}


/*Main*/

int main(int argc, const char* argv[] ) {
	int nbytes = 100;
	/*----------------------*/
	
	char *input;
	int x, y;
	int args_assigned;
	
	/*	Args
	*	When run with any number of arguments other than 0, 1, 2, 3 or 7, the program should exit with an error
	*/
	if ((argc !=2) && (argc !=3) && (argc != 4) && (argc != 8)) {
		print_err(1);
		exit(0);
	}
	
	else {
		cmd_in(argc, argv);
		print_args();
		setup_board(dim);
		print_board();
		
		/*Main Game Loop*/
		while (1) {
			while (args_assigned != 2){
				input = (char *) malloc (nbytes + 1);
				getline(&input, &nbytes, stdin);
				args_assigned = sscanf (input, "%d %d", &x, &y);
			}
			
			if (board[x][y] != '0' && board[x][y] != 'X') {
				/*Validate Input*/
				if (board_input_is_valid(x,y) == 1){
					insert_number(get_char(), x, y);
					print_board();
					/*Check for lines*/
					
					printf("Horizontal line: %d\n", horizontal_line(get_char(),x ,y));
					
					if (horizontal_line(get_char(), x, y) == 1) {
						end_game(get_char(),1);
						
					}			
 					turn++;
				}
			}
			args_assigned = 0; /*reset for next sscanf*/
		}
	}
}
