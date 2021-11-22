#include <game.h>



void playerChooseSave() {

}
void playerTurn(player *player, int ***map) {

	printPlayerInterface();
	char* value = malloc(sizeof (char) * (10 + 1));
	fgets(value,10,stdin);
	fflush(stdin);

	if(value[0] == '1') {

	}
	else if(value[0] == '2') {

	}
	else if(value[0] == '3') {

	}
	else if(value[0] == '4') {

	}
	else if(value[0] == '5') {

	}
	else {

	}
	free(value);
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
void recolte() {

}

void startGame() {
	cleanTerminal();
	printStartMenu();

	char* value = malloc(sizeof (char) * (10 + 1));
	fgets(value,10,stdin);
	fflush(stdin);
	if(value[0] == '1') {
		//start new game
	}
	else if(value[0] == '2') {
		// load game
	}
	else if(value[0] == '3') {
		// credit
	}
	else if(value[0] == '4') {
		return;
	}
	else {
		printc("Un aventurier qui ne sait pas lire... nous voilà bien partie. (-_-) \n",1,FOREGROUND_YELLOW);
	}
}
void initGame() {

}
void initNewGame() {

}
void endGame() {

}

// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu() {
	printc(	    "\n / \\----------------------------------------,\n"
				   " \\_,|                                       |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("Bienvenue dans Mallocworld !!!",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc("     |\n"
		   "    |                                       |\n"
		   "    |                                       |\n"
		   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("1 - Lancer une nouvelle partie",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc("     |\n"
		   "    |                                       |\n"
		   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("2 - Charger une partie",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc("             |\n"
		   "    |                                       |\n"
		   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("3 - Credit",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc("                         |\n"
		   "    |                                       |\n"
		   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("4 - Quitter le jeu",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc("                 |\n"
		   "    |                                       |\n"
		   "    |                                       |\n"
		   "    |  ,-------------------------------------,\n"
		   "    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
}
void printPlayerInterface() {

	printc(	    "\n / \\----------------------------------------,\n"
				   " \\_,|                                       |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("A votre tour, bouger vous le cul!",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc("     |\n"
		   "    |                                       |\n"
		   "    |                                       |\n"
		   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc("1 - NORTH",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc("     |\n"
		   "    |                    ^                   |\n"
		   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("4 - WEST    <-O->    2 - EAST",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc("             |\n"
		   "    |                    v                   |\n"
		   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc("3 - SOUTH",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc("                         |\n"
		   "    |                                       |\n"
		   "    |                                       |\n"
		   "    |  ,-------------------------------------,\n"
		   "    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);







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
	setTextDefault();
}
void printCredit() {
	printc("Jeux Mallocworld développé par :\n"
		   "	-	Denis TURBIEZ		@Macaron\n"
		   "	-	Basile PULIN		@Barlords\n"
		   "	-	Jean ....			@Jean\n",
		   2,FOREGROUND_GREEN,FOREGROUND_INTENSITY
	);
}