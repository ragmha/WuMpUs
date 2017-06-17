#include <stdio.h>
#include <time.h>
#include <windows.h>
#define xaxis 5
#define yaxis 5
#define n_wumpus 3
#define n_pits 3
#define n_golds 3
#define n_arrows 3
#define filename "hall of fame.txt"

void delay(float t);
void deathmidi();
void goldmidi();

typedef struct
{
	char character;
	char wumpus;
	char pit;
	char gold;
}str;

void title()
{
	HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole,26);
	printf("        ***************************************************************        \n");
	printf("        *                                                             *        \n");
	printf("        *     W         W U    U   M     M   PPPPP  U    U  SSSSS     *        \n");
	printf("        *      W       W  U    U  M M   M M  P    P U    U S          *        \n");
	printf("        *      W   W   W  U    U  M  M M  M  PPPPPP U    U  SSSS      *        \n");
	printf("        *       W W W W   U    U M    M    M P      U    U      S     *        \n");
	printf("        *        W   W     UUUU  M         M P       UUUU  SSSSS      *        \n");
	printf("        *                                                             *        \n");
	printf("        ***************************************************************        \n\n");
}

void instruction()
{
	char option;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole,47);
	printf("        To show INSTRUCTION, press H.                                          \n");
	getchar();
	scanf("%c",&option);

	if(option == 'H' || 'h')
	{
		system("cls");
		printf("HOW TO PLAY:\n\n");
		printf("1. You will start the game from the entrance, in each cavern (square) you will be told what you hear, see and feel. You begin the game armed with three arrows.\n");
		printf("2. If you smell stench (a strong and unpleasant smell of the Wumpus), you are next to a cavern with Wumpus, If you think you can identify the cavern with the Wumpus, you can launch an arrow into an adjacent cavern to kill the Wumpus, and you get to collect back the arrow you launched, otherwise you will lose that arrow if you missed (launch arrow into a cavern without Wumpus).\n");
		printf("3. Be wary for if you enter the cavern with Wumpus, you will be eaten (Game over!)\n");
		printf("4. If you are one cavern away from a pit, you will feel a breeze.\n");
		printf("5. If you go into cavern with pit, you will fall into the pit, and you will lose one arrow and one gold (if any) that have at that time. \n");
		printf("6. It is possible to have a cavern with both Wumpus and pit together, because the Wumpus has suckers on its feet which prevents it from failing into a pit. However, you will still be eaten by the Wumpus in this case if you go into that cavern.\n");
		printf("7. There are treasures to be had as well, certain caverns contain gold to be collected. If you are one room away from a cavern with gold, you can see something glittering. You will win the game by collecting all the gold available in the cave.\n\n");

		printf("Press any button to start the game.\n");
	}
}

void readrecord()
{
	char id[20];
	int golds,arrows,wumpus;
	FILE *file;

	file = fopen(filename,"r+");
	printf("%-23s%-17s%-13s%-15s\n","Player Name","Gold Collected","Arrow Left","Wumpus Slain");
	while (fscanf(file,"%s%i%i%i%i",&id,&golds,&arrows,&wumpus)==4)
	{
		printf("%-23s%-17i%-13i%-15i\n",id,golds,arrows,wumpus);
	}
}

void writerecord(char id[20],int golds,int arrows,int wumpus)
{
	FILE *file_ptr;
	file_ptr = fopen(filename, "a+");
	if (file_ptr == NULL)
	{
		perror("The following error has occurred: ");
	}
	else
	{
		fprintf(file_ptr,"%23s%17i%13i%15i\n",id,golds,arrows,wumpus);
	}
}

void mapping(str border[yaxis][xaxis])
{
	int i,j;
	/*Clear screen once a action done*/
	system("cls");
	for(j=0;j<yaxis;j++)
	{
		printf("_______________________________\n");
		for(i=0;i<xaxis;i++)
		{
			/*This is to show whole structure in border.
			printf("| %c%c%c%c",border[j][i].character,border[j][i].wumpus,border[j][i].pit,border[j][i].gold);*/

			/*To hide all creatures and golds in other rooms.*/
			printf("| %c",border[j][i].character);

			if(border[j][i].character == '@')
				printf("%c",border[j][i].pit);
			else
				printf(" ");

			if(border[j][i].character == '@')
				printf("%c", border[j][i].wumpus);
			else
				printf(" ");

			if(border[j][i].character == '@')
				printf("%c", border[j][i].gold);
			else
				printf(" ");
		}
		printf("|\n");
	}
	printf("_______________________________\n");
}

void printmenu(int arrows)
{
	printf("*Menu:\n");
	printf("*Press A: Move to left\n");
	printf("*Press D: Move to right\n");
	printf("*Press W: Move to up\n");
	printf("*Press S: Move to down\n");
	printf("*Press F: Shooting mode(Remaining %iarrow(s))\n",arrows);
	printf("*Press Q: Quit\n");
	printf("Please enter your action (case sensitive): ");
}

int encounter(str border[yaxis][xaxis],int y, int dy, int x, int dx)
{
	if(border[y+dy][x+dx].wumpus == 'W')
	{
		printf("Sad! You have been buried by WUMPUS.\n");
		return 1;
	}
	else if(border[y+dy][x+dx].pit == 'O')
	{
		printf("Oppss!You fell into a pit and climb up.Die Hard!\n");
		printf("1 ARROW!! Lost in the pit.\n");
		return 2;
	}
	else if(border[y+dy][x+dx].gold == 'G')
	{
		printf("You earned a gold.\n");
		return 3;
	}
	else return 0;
}

void smell(str border[yaxis][xaxis])
{
	int i,j,x,y,wsmell=0,psmell=0,gsmell=0;
	for(j=0;j<yaxis;j++)
		for(i=0;i<xaxis;i++)
			if(border[j][i].character=='@')
			{
				x=i;y=j;
			}

	for(i=x-1;i<x+2;i++)
	{
		if(border[y][i].wumpus == 'W')
			wsmell = 1;

		if(border[y][i].pit == 'O')
			psmell = 1;

		if(border[y][i].gold == 'G')
			gsmell = 1;
	}

	for(j=y-1;j<y+2;j++)
	{
		if(border[j][x].wumpus == 'W')
			wsmell = 1;

		if(border[j][x].pit == 'O')
			psmell = 1;

		if(border[j][x].gold == 'G')
			gsmell = 1;
	}

	if(wsmell == 1)
		printf("You smell a stench!\n");
	if(psmell == 1)
		printf("You feel the breeze!\n");
	if(gsmell == 1)
		printf("You see something GLITTERING!\n");
}

int firing(str border[yaxis][xaxis],int arrows,int y, int dy, int x, int dx)
{
	if(arrows > 0)
	{
		if(border[y+dy][x+dx].wumpus == 'W')
		{
			border[y+dy][x+dx].wumpus = ' ';
			return 1;
		}
		else
		{
			printf("You missed! There is no wumpus.\n");
			return 2;
		}
	}
	else if(arrows < 0)
	{
		printf("You have ran out of arrow!\n");
		return 2;
	}
}

void movement(str border[yaxis][xaxis])
{
	int i,j,x,y,dx,dy,action,arrows=n_arrows,ggolds=0,gwumpus=0;
	char move,shooting,temp[10],id[20];
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	do
	{
		printf("Status:\n");
		printf("Earned gold(s): %i     Wumpus killed: %i\n",ggolds,gwumpus);
		printmenu(arrows);
		scanf("%c",&move);
		gets(temp);

		for(j=0;j<yaxis;j++)
			for(i=0;i<xaxis;i++)
				if(border[j][i].character == '@')
				{
					x=i;y=j;
				}

		switch(move)
		{
			case 'W':
				dx=0;dy=-1;
				action=1;
				break;
			case 'S':
				dx=0;dy=+1;
				action=1;
				break;
			case 'A':
				dx=-1;dy=0;
				action=1;
				break;
			case 'D':
				dx=+1;dy=0;
				action=1;
				break;
			case 'F':
				printf("Ready to fire.\n");
				printf("Fire direction (W-Up, S-Down, A-Left, D-Right): ");
				scanf("%c",&shooting);
				gets(temp);
				switch(shooting)
				{
					case 'W':
						dx=0;dy=-1;
						action=2;
						break;
					case 'S':
						dx=0;dy=+1;
						action=2;
						break;
					case 'A':
						dx=-1;dy=0;
						action=2;
						break;
					case 'D':
						dx=+1;dy=0;
						action=2;
						break;
					default:
						printf("Invlid input! Please try again.\n");
							break;
				}
			default:
				printf("Invalid input! Please try again.\n");
				break;
		}
		if(action == 1)
		{
			if(encounter(border,y,dy,x,dx) == 0)
			{
				border[y][x].character = ' ';
				border[y+dy][x+dx].character = '@';
			}
			else if(encounter(border,y,dy,x,dx) == 1)
			{
				border[y][x].character = ' ';
				mapping(border);
				deathmidi();
				SetConsoleTextAttribute(hConsole,0);
				printf("                            ");
				SetConsoleTextAttribute(hConsole,31);
				printf("Please enter Login ID\n\n");
				SetConsoleTextAttribute(hConsole,0);
				printf("                            ");
				SetConsoleTextAttribute(hConsole,31);
				scanf("%s",&id);
				printf("\n");
				writerecord(id,ggolds,arrows,gwumpus);
				getch();
				exit(1);
			}
			else if(encounter(border,y,dy,x,dx) == 2)
			{
				border[y][x].character = ' ';
				border[y+dy][x+dx].character = '@';
				arrows--;
				action = 4;
			}
			else if(encounter(border,y,dy,x,dx) == 3)
			{
				border[y][x].character = ' ';
				border[y+dy][x+dx].character = '@';
				/*border[y+dy][x+dx].gold = ' ';*/
				goldmidi();
				ggolds++;
				action = 3;
			}
		}
		else if(action == 2)
		{
			arrows--;
			if(firing(border,arrows,y,dy,x,dx) == 1)
			{
				gwumpus++;
				printf("\a");
			}
		}
		mapping(border);
		smell(border);
		if(ggolds == n_golds)
		{
			printf("N1!You have collected all the gold.\n");
			printf("YOU WIN THE GAME!\n");
			getch();

			SetConsoleTextAttribute(hConsole,0);
			printf("                            ");
			SetConsoleTextAttribute(hConsole,31);
			printf("Please enter Login ID\n\n");
			SetConsoleTextAttribute(hConsole,0);
			printf("                            ");
			SetConsoleTextAttribute(hConsole,31);
			scanf("%s",&id);
			printf("\n");
			writerecord(id,ggolds,arrows,gwumpus);

			exit(1);
		}
		if(action == 3)
			border[y+dy][x+dx].gold = ' ';
		else if(action == 4)
			border[y+dy][x+dx].pit = ' ';
	}while(move != 'Q' || shooting != 'Q');
	SetConsoleTextAttribute(hConsole,0);
	printf("                            ");
	SetConsoleTextAttribute(hConsole,31);
	printf("Please enter Login ID\n\n");
	SetConsoleTextAttribute(hConsole,0);
	printf("                            ");
	SetConsoleTextAttribute(hConsole,31);
	scanf("%s",&id);
	printf("\n");
	writerecord(id,ggolds,arrows,gwumpus);
	exit(1);
}

void delay(float t)
{
    time_t start;
    time_t current;
    time(&start);
    do
        time(&current);
    while (difftime(current,start) < t);
}

void deathmidi()
{
	Beep(493.88,100);
	Beep(698.46,100);
	Beep(698.46,100);
	Beep(698.46,200);
	Beep(659.26,200);
	Beep(587.33,200);
	Beep(523.25,100);
	Beep(329.63,200);
	Beep(329.63,100);
	Beep(261.63,100);
}

void goldmidi()
{
	Beep(659.26,100);
	Beep(783.99,100);
	Beep(1318.51,100);
	Beep(1046.50,100);
	Beep(1174.66,100);
	Beep(1567.98,100);
}

int main()
{
	int i,j,x,y,assign,id[20];
	char move,option;
	str border[yaxis][xaxis];
	HANDLE hConsole;
	srand((unsigned)time(NULL));

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	title();
	readrecord();
	instruction();
	getch();

	SetConsoleTextAttribute(hConsole,58);

	/*testing for printing border with nothing*/
	for(j=0;j<yaxis;j++)
		for(i=0;i<xaxis;i++)
		{
			border[j][i].character=' ';
			border[j][i].wumpus=' ';
			border[j][i].gold=' ';
			border[j][i].pit=' ';
		}

	/*assign character at bottom-left cornor*/
	border[4][0].character = '@';
	/*randomly place wumpus*/
	assign=0;
	do
	{
		/*generate a random coordinate for wumpus*/
		x = rand() % 5;
		y = rand() % 5;

		if(border[y][x].wumpus!='W' && border[y][x].character!='@' && border[y+1][x].character!='@' && border[y][x-1].character!='@')
			goto assign;
		else continue;

		assign:
			border[y][x].wumpus='W';
			assign++;
	}while(assign<n_wumpus);
	/*randomly place gold*/
	assign=0;
	do
	{
		x = rand() % 5;
		y = rand() % 5;

		if(border[y][x].gold!='G' && border[y][x].character!='@')
			goto assign1;
		else continue;

		assign1:
			border[y][x].gold='G';
			assign++;
	}while(assign<n_golds);
	/*randomly place pit*/
	assign=0;
	do
	{
		x = rand() % 5;
		y = rand() % 5;

		if(border[y][x].gold!='G' && border[y][x].pit!='O' && y!=4 && x!=0)
			goto assign2;
		else continue;

		assign2:
			border[y][x].pit='O';
			assign++;
	}while(assign<n_pits);

	mapping(border);
	smell(border);
	movement(border);
	exit(1);

	return 0;
}
