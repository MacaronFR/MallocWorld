#include <game.h>



void playerChooseSave() {

}


void playerWantMoov(player *player, direction direction) {
	//checkCase();
}
void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl) {
	int endFight = 0;
	while(endFight == 0) {
		endFight = playerTurnFight(player, monster);
		if(monster->life <= 0)
			endFight = 2;
		else {
			playerTakeDamage(player, monster->strength);
			if(playerIsDead(player))
				endFight = -1;
		}
	}
	switch(endFight) {
		case -1: {
			printc("Vous avez été découpé, brulé, broyé... Quelle mort tragique... :'(", 1, FOREGROUND_RED);
			//endGame();
		}
		case 1: {
			printc("Vous avez fuit le combat ?!  Tapette !!!", 1, FOREGROUND_YELLOW);
		}
		case 2: {
			addMonsterRespawn(monster,list,x,y,lvl);
			playerWinExp(player, monster->exp);
		}
		default: {
			printc("BUG dans la matrice : fight", 1, FOREGROUND_RED);
		}
	}
}


void inGame(player *player, level *map, storage *storage, item **listItem, resource **listResource, monster **listMonster) {
	bool end = false;
	while(!end) {
		cleanTerminal();
		tempPrintMap(map);
		printPlayer(player);
		switch (playerTurn(player, map, storage, listItem, listResource, listMonster)){
			case -1: {
				// joueur mort
				end = true;
				break;
			}
			case 0:{
				// le joueur à quitté la partie
				break;
			}
			case 1:{
				// tour de jeu normal
				break;
			}
			case 2:{
				// le joueur à gagné
				break;
			}
		}
	}
}
int playerTurn(player *player, level *map, storage *storage, item **listItem, resource **listResource, monster **listMonster) {
	printPlayerInterface();
	char *value = malloc(sizeof(char) * (10 + 1));
	fgets(value, 10, stdin);
	fflush(stdin);
	if (value[0] == NORTH) {

	} else if (value[0] == EAST) {

	} else if (value[0] == SOUTH) {

	} else if (value[0] == WEST) {

	} else if (value[0] == '5') {
		return 0;
	}
	else {
		printc("Un aventurier qui ne sait pas lire une rose des vents... nous voilà bien partie. (-_-) \n", 1, FOREGROUND_YELLOW);
	}
	free(value);
}

// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu() {
	cleanTerminal();
	printc(	    "\n / \\----------------------------------------,\n"
				   	" \\_,|                                       |\n"
				   	"    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Bienvenue dans Mallocworld !!!",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
		   			"    |                                       |\n"
		   			"    |                                       |\n"
					"    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				  "1 - Lancer une nouvelle partie",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc(												 "     |\n"
				    "    |                                       |\n"
				    "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "2 - Charger une partie",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(							 			 "             |\n"
				    "    |                                       |\n"
				    "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "3 - Credit",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(				   			 "                         |\n"
				    "    |                                       |\n"
				    "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "4 - Quitter le jeu",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "                 |\n"
				    "    |                                       |\n"
				    "    |                                       |\n"
				    "    |  ,-------------------------------------,\n"
				    "    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");
}
void tempPrintMap(level *map) {
	for(int i=0 ; i<map->h ; i++) {
		printMapLineSeparator(map->w);
		printc("|",1,FOREGROUND_BLUE);
		for (int j=0; j<map->w ; j++) {
			setTextDefault();
			int id = map->level[i][j];
			switch(id) {
				case -3:
				case -2:
					setText( 2, FOREGROUND_RED,BACKGROUND_CYAN);
					break;
				case -1:
					setText( 2, FOREGROUND_RED,BACKGROUND_RED);
					break;
				case 0:
					setText( 2, FOREGROUND_BLACK, BACKGROUND_BLACK);
					break;
				case 1:
					setText( 3, FOREGROUND_CYAN, FOREGROUND_INTENSITY,BACKGROUND_PURPLE);
					break;
				case 2:
					setText( 3, FOREGROUND_RED, FOREGROUND_INTENSITY,BACKGROUND_CYAN);
					break;
				case 3:
				case 6:
				case 9:
					setText( 3, FOREGROUND_RED,FOREGROUND_INTENSITY,BACKGROUND_GREEN);
					break;
				case 4:
				case 7:
				case 10:
					setText( 2, FOREGROUND_BLACK ,BACKGROUND_WHITE);
					break;
				case 5:
				case 8:
				case 11:
					setText(3, FOREGROUND_PURPLE,FOREGROUND_INTENSITY,BACKGROUND_YELLOW);
					break;
				case 99:
					setText( 2, FOREGROUND_BLACK,BACKGROUND_RED);
					break;
				default:
					setText( 1, FOREGROUND_RED);
					break;

			}
			printf("%2d",id);
			setTextDefault();
			printc("|", 1, FOREGROUND_BLUE);
		}
		printf("\n");
	}
	printMapLineSeparator(map->w);
	printf("\n\n");
}
void printPlayerInterface() {

	printc(	    "\n / \\-------------------------------------------,\n"
				   " \\_,|                                          |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("A votre tour, bouger vous le cul!",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc("     |\n"
		   "    |                                          |\n"
		   "    |                                          |\n"
		   "    |                    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				    "1 - NORTH",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(							  "             |\n"
		   "    |                                          |\n"
		   "    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("^",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc("                     |\n"
		   "    |      ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("WEST - 4    ",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc("<–+–>",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc("    2 - EAST",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc("       |\n"
		   "    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("v",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc("                     |\n"
		   "    |                                          |\n"
		   "    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("3 - SOUTH",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc("             |\n"
		   "    |                                          |\n"
		   "    |                                          |\n"
		   "    |                            ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("5 - Quitter",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc("   |\n"
		   "    |                                          |\n"
		   "    |  ,----------------------------------------,\n"
		   "    \\_/________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");


	/*
	// printMap();
	printc("\nSélectionner une direction : \n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	setText(2, FOREGROUND_BLUE, FOREGROUND_INTENSITY);
	printf("+---+-------+   +---+------+   +---+-------+   +---+------+\n");
	printf("| 1 - NORTH |   | 2 - EAST |   | 3 - SOUTH |   | 4 - WEST |\n");
	printf("+---+-------+   +---+------+   +---+-------+   +---+------+\n");
	setTextDefault();
	setText(1, FOREGROUND_BLUE);
	printf("+---+-------------+\n");
	printf("| 5 - Save & Quit |\n");
	printf("+---+-------------+\n");
	setTextDefault();*/
}
void printCredit() {
	printc("Jeux Mallocworld développé par :\n"
		   "	-	Denis TURBIEZ		@Macaron\n"
		   "	-	Basile PULIN		@Barlords\n"
		   "	-	Jean ....			@Jean\n",
		   2,FOREGROUND_GREEN,FOREGROUND_INTENSITY
	);
}


void printMapLineSeparator(int count) {
	setText(1,FOREGROUND_BLUE);
	for(int i=0 ; i<count ; i++) {
		printf("+--");
	}
	printf("+\n");
	setTextDefault();
}