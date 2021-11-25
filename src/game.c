#include <game.h>
#include <ctype.h>

void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList, int nbMap) {
	cleanTerminal();
	bool end = false;
	while(!end) {
		cleanTerminal();
		displayPlayerOnMap(player,map);
		printPlayer(player);
		switch (playerTurn(player, map, storage, listItem,nItem, listResource,nResource, listMonster,nMonster, respawnList)){
			case -1: {
				gameOver();
				end = true;
				break;
			}
			case 0:{
				printc("Voulez vous sauvegarder votre progression ?    [y/n]\n",2,FOREGROUND_GREEN, FOREGROUND_INTENSITY);
				char choice[256];
				scanf("%s", choice);
				if(choice[0] == 'y') {
					bool res = false;
					char filename[256];
					do {
						printc("sauvegarder à : ",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
						scanf("%s", filename);
						res = saveGame(filename, map, respawnList, player, storage, nbMap);
					} while (!res);
					end = true;
				}
				break;
			}
			case 1:{
				printc("Un nouveau tour commence...\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
				checkRespawn(respawnList,map);
				break;
			}
			case 2:{
				winGame();
				end = true;
				break;
			}
		}
	}
}
int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList) {
	bool end = false;
	displayPlayerOnMap(player,map);
	printPlayer(player);
	printPlayerInterface();
	char* value = malloc(sizeof(char) * 255);
	fflush(stdin);
	scanf("%s", value);
	if (tolower(value[0]) == 'z') {
		tryMove(player,map,NORTH,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y-1);
	} else if (tolower(value[0]) == 'd') {
		tryMove(player,map,EAST,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x+1,player->abs_coord.y);
	} else if (tolower(value[0]) == 's') {
		tryMove(player,map,SOUTH,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y+1);
	} else if (tolower(value[0]) == 'q') {
		tryMove(player, map, WEST, listItem, nItem, listResource, nResource, listMonster, nMonster, respawnList, player->abs_coord.x - 1, player->abs_coord.y);
	} else if(tolower(value[0]) == 'a') {
		playerUsePotion(player);
	} else if (tolower(value[0]) == 'o' || tolower(value[0]) == '0') {
		free(value);
		return 0;
	} else {
		printc("Un aventurier qui ne sait pas lire une rose des vents... nous voilà bien partie. (-_-) \n", 1, FOREGROUND_YELLOW);
	}
	free(value);
}
int tryMove(player *player, level *map, direction direction, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList, int x, int y) {
	printf("tryMove()\n");
	if(player->abs_coord.y-1 < 0 || player->abs_coord.x+1 >= map->w || player->abs_coord.y+1 >= map->h || player->abs_coord.x - 1 < 0) //sortie de map
		return 0;

	int id, resCase, resFight;
	id = map[player->abs_coord.zone].level[player->abs_coord.y-1][player->abs_coord.x];
	resCase = checkCaseIdType(id,listResource,nResource,listMonster,nMonster);
	printf("%d", resCase);
	switch(resCase) {
		case -1:
			printc("Case invalide!!!\n",2, FOREGROUND_RED, FOREGROUND_INTENSITY);
			return 0;
		case 0:
			playerMove(player,map,direction);
			return true;
		case 3: // Ressource
			return tryRecolte(player, listItem, nItem, listResource, nResource, id);
		case 20: // Monster
			resFight = fight(player, createMonster(findMonster(listMonster,nMonster,id)),&respawnList,x,y,player->abs_coord.zone);
			if(resFight == -1 || resFight == 0)
				return false;
			return true;
	}
}
int checkCaseIdType(int id, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster) {
	printf("checkCaseIdType()\n");
	resource *res1 = findResource(listResource,nResource,id);
	if(res1 != NULL)
		return 3;
	monster *res2 = findMonster(listMonster,nMonster,id);
	if(res2 != NULL)
		return 20;
	return id;
}
bool tryRecolte(player *player, item **listItem, size_t nItem, resource **listResource, size_t nResource, int id) {
	printf("tryRecolte()\n");
	item **listTool = getItemCategory(player->inventory, TOOLS);
	resource *resource = findResource(listResource,nResource,id);
	int i = 0;
	if(listTool == NULL)
		return NULL;
	while(listTool[i] != NULL) {
		//if(listTool[i]->durability > 0 && (listTool[i]->type & 1023) == (resource->item->type & 1023) && (listTool[i]->flag & resource->flag) != 0)
		if(listTool[i]->durability > 0 && (listTool[i]->type & resource->item->type) != 0 && (listTool[i]->flag & resource->flag) != 0) {
			int8_t res;
			if((resource->flag & 1) != 0) {
				res = listTool[i]->durability - (getItem(listItem, nItem, listTool[i]->id)->durability * (10 / 100));
				if(res < 0)
					continue;
			} else if((resource->flag & 2) != 0) {
				res = listTool[i]->durability - (getItem(listItem, nItem, listTool[i]->id)->durability * (20 / 100));
				if(res < 0)
					continue;
			} else if((resource->flag & 4) != 0) {
				res = listTool[i]->durability - (getItem(listItem, nItem, listTool[i]->id)->durability * (40 / 100));
				if(res < 0)
					continue;
			}
			listTool[i]->durability = res;
			addItemInInventory(player->inventory, getItem(listItem,nItem, findResource(listResource,nResource,id)->item->id));
			free(listTool);
			return true;
		}
	}
	free(listTool);
	printc("Tu ne peux pas récupérer la ressource\n",2, FOREGROUND_RED, FOREGROUND_INTENSITY);
	return NULL;
}

int fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl) {
	printf("fight()\n");
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
			return -1;
		}
		case 1: {
			printc("Vous avez fuit le combat ?!  Tapette !!!", 1, FOREGROUND_YELLOW);
			return 0;
		}
		case 2: {
			addMonsterRespawn(monster,list,x,y,lvl);
			playerWinExp(player, monster->exp);
			return 1;
		}
		default: {
			printc("BUG dans la matrice : fight", 1, FOREGROUND_RED);
		}
	}
}

void gameOver() {
	printc("GAME OVER !!!\n",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
}
void winGame() {
	printc("VOUS AVEZ GAGNE !!!\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
}

//// --------------------------------- AFFICHAGE ---------------------------------
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
			int id = map[0].level[i][j];
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
void printMapLineSeparator(int count) {
	setText(1,FOREGROUND_BLUE);
	for(int i=0 ; i<count ; i++) {
		printf("+--");
	}
	printf("+\n");
	setTextDefault();
}
void printPlayerInterface() {

	printc(	"\n / \\-------------------------------------------,\n"
				" \\_,|                                          |\n"
				"    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				"A votre tour, bouger vous le cul!",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(													"     |\n"
		   		"    |                                          |\n"
		   		"    |                                          |\n"
		   		"    |                    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				    			"Z - NORTH",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(							  				"             |\n"
		   		"    |                                          |\n"
		   		"    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(								"^",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc(									"                     |\n"
		   		"    |      ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"WEST - Q    ",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(							"<–+–>",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc(									"    D - EAST",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(												"       |\n"
		   		"    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(								"v",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printc(									"                     |\n"
		   		"    |                                          |\n"
		   		"    |                    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(								"S - SOUTH",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(											"             |\n"
		   		"    |                                          |\n"
		   		"    |                                          |\n"
		   		"    |", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(		   "A - Potion                   ",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(										"O - Quitter",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(													"   |\n"
		   		"    |                                          |\n"
		   		"    |  ,----------------------------------------,\n"
		   		"    \\_/________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");

}
void printCredit() {
	printc("Jeux Mallocworld développé par :\n"
		   "	-	Denis TURBIEZ		@Macaron\n"
		   "	-	Basile PULIN		@Barlords\n"
		   "	-	Jean-Jaures OKA		@Jean\n",
		   2,FOREGROUND_GREEN,FOREGROUND_INTENSITY
	);
}
