#include <game.h>

level *createGame(int portal[4][2], player *p1, item **listItem, size_t nItem, int *lv){
	srand(time(NULL));
	level *map = generateMap(rand(), portal);
	p1->abs_coord.x = portal[0][0];
	p1->abs_coord.y = portal[0][1] - 1;
	p1->abs_coord.zone = 0;
	addItemInInventory(p1->inventory, copyItem(getItem(listItem,nItem,1)));
	addItemInInventory(p1->inventory, copyItem(getItem(listItem,nItem,2)));
	addItemInInventory(p1->inventory, copyItem(getItem(listItem,nItem,3)));
	addItemInInventory(p1->inventory, copyItem(getItem(listItem,nItem,4)));
	map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 1;
	*lv = 3;
	printc("Level & maps created!\n",1,FOREGROUND_GREEN);
	return map;
}

void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int nbMap) {
	cleanTerminal();
	bool end = false;
	while(!end) {;
		switch (playerTurn(player, map, storage, listItem,nItem, listResource,nResource, listMonster,nMonster, respawnList)){
			case -2:
				gameOver();
				end = true;
				break;
			case -1:
				printc("Voulez vous sauvegarder votre progression ?    [y/n]\n",2,FOREGROUND_GREEN, FOREGROUND_INTENSITY);
				char choice[256];
				scanf("%s", choice);
				if(choice[0] == 'y') {
					bool res = false;
					char* filename = malloc(sizeof(char) * 255);
					do {
						printc("sauvegarder à : ", 2, FOREGROUND_GREEN, FOREGROUND_INTENSITY);
						scanf("%s", filename);
						res = saveGame(filename, map, *respawnList, player, storage, nbMap);
					} while (!res);
					free(filename);
					end = true;
				}
				break;
			case 1:
				printc("Un nouveau tour commence...\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
				checkRespawn(respawnList,map);
				break;
			case 2:
				winGame();
				end = true;
				break;
		}
	}
}
int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList) {
	int res = 0;
	while(res == 0) {
		displayPlayerOnMap(player,map);
		printPlayer(player);
		printInterfacePlayer();
		char* value = malloc(sizeof(char) * 255);
		fflush(stdin);
		scanf("%s", value);
		cleanTerminal();
		if (tolower(value[0]) == 'z') {
			res = tryMove(player,map,NORTH,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y-1);
		} else if (tolower(value[0]) == 'd') {
			res = tryMove(player,map,EAST,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x+1,player->abs_coord.y);
		} else if (tolower(value[0]) == 's') {
			res = tryMove(player,map,SOUTH,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y+1);
		} else if (tolower(value[0]) == 'q') {
			res = tryMove(player, map, WEST,storage, listItem, nItem, listResource, nResource, listMonster, nMonster, respawnList, player->abs_coord.x - 1, player->abs_coord.y);
		} else if(tolower(value[0]) == 'a') {
			res = playerUsePotion(player);
		} else if (tolower(value[0]) == 'o' || tolower(value[0]) == '0') {
			res = -1;
		} else {
			printc("Un aventurier qui ne sait pas lire une rose des vents... nous voilà bien partie. (-_-) \n", 1, FOREGROUND_YELLOW);
			res = 0;
		}
		free(value);
	}
	return res;



}
int tryMove(player *player, level *map, direction direction,storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int x, int y) {
	printf("tryMove() %d\n",direction);
	if(player->abs_coord.y-1 < 0 || player->abs_coord.x+1 >= map->w || player->abs_coord.y+1 >= map->h || player->abs_coord.x - 1 < 0) {//sortie de map
		printc("La Terre est plate mon gars, tu ne peux pas aller plus loin! ¯\\_(\"/)_/¯",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
		return 0;
	}

	int id, resCase, res;
	id = map[player->abs_coord.zone].level[y][x];
	resCase = checkCaseIdType(id,listResource,nResource,listMonster,nMonster);
	printf("resCase = %d\n", resCase);
	switch(resCase) {
		case -2:
			interactWithPNJ(player, storage, listItem, nItem);
			return 1;
		case -1: // Case infranchissable
			printc("Case invalide!!!\n",2, FOREGROUND_RED, FOREGROUND_INTENSITY);
			return 0;
		case 0: // Case vide
			playerMove(player,map,direction);
			return 1;
		case 2: // PNJ
			interactWithPNJ(player,storage,listItem,nItem);
			return 1;
		case 3: // Ressource
			res = tryRecolte(player, listItem, nItem, listResource, nResource, respawnList, id, x, y);
			if(res == 1) {
				playerMove(player,map,direction);
			}
			return res;
		case 20: // Monster
			res = fight(player, createMonster(findMonster(listMonster,nMonster,id)),respawnList,x,y);
			if(res == 1 || res == 2)
				playerMove(player,map,direction);
			return res;
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
int tryRecolte(player *player, item **listItem, size_t nItem, resource **listResource, size_t nResource, respawn **listRespawn, int id,  int x, int y) {
	printf("tryRecolte()\n");
	item **listTool = getItemCategory(player->inventory, TOOLS);
	resource *resource = findResource(listResource,nResource,id);
	item *tmp;
	int i = 0;
	if(listTool == NULL) {
		printc("Tu ne peux pas récupérer la ressource\n", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
		return 0;
	}
	while(listTool[i] != NULL) {
		//if(listTool[i]->durability > 0 && (listTool[i]->type & 1023) == (resource->item->type & 1023) && (listTool[i]->flag & resource->flag) != 0)
		if(listTool[i]->durability >= resource->item->durability && (listTool[i]->type & resource->item->type) != 0 && (listTool[i]->flag & resource->flag) != 0) {
			listTool[i]->durability -= resource->item->durability;
			tmp = copyItem(resource->item);
			if(tmp == NULL){
				free(listTool);
				return -1;
			}
			addItemInInventory(player->inventory, tmp);
			addResourceRespawn(resource,listRespawn,x,y,player->abs_coord.zone);
			free(listTool);
			return 1;
		}
		i++;
	}
	free(listTool);
	printc("Tu ne peux pas récupérer la ressource\n",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	return 0;
}
int fight(player *player, monster *monster, respawn **listRespawn, int32_t x, int32_t y) {
	printf("fight()\n");
	int res = 0;
	while(res == 0) {
		res = playerTurnFight(player, monster);
		if(monster->life <= 0)
			res = 1;
		else {
			playerTakeDamage(player, monster->strength);
			if(playerIsDead(player))
				res = -2;
		}
	}
	switch(res) {
		case -2: {
			printc("Vous avez été découpé, brulé, broyé... Quelle mort tragique... :'(", 1, FOREGROUND_RED);
			return -2;
		}
		case 1: {
			addMonsterRespawn(monster,listRespawn,x,y,player->abs_coord.zone);
			playerWinExp(player, monster->exp);
			if(monster->id == 99)
				return 2;
			return 1;
		}
		case 2: {
			printc("Vous avez fuit le combat ?!  Tapette !!!", 1, FOREGROUND_YELLOW);
			return 1;
		}
		default: {
			printc("BUG dans la matrice : fight", 1, FOREGROUND_RED);
		}
	}
}
void interactWithPNJ(player *player, storage *storage, item **listItem, size_t nItem) {
	item **listCraftableItem = getCraftableItem(listItem, nItem, player->abs_coord.zone);
	bool quit = false;
	while(!quit) {
		printInterfacePNJ();
		char* value = malloc(sizeof(char) * 255);
		fflush(stdin);
		scanf("%s", value);
		cleanTerminal();
		if (tolower(value[0]) == 'a') {
			goToStorage(player,storage);
		} else if (tolower(value[0]) == 'z') {
			goToCrafting(player,listCraftableItem);
		} else if (tolower(value[0]) == 'o') {
			quit = true;
		} else {
			printc("erreur interactWithPNJ() \n", 1, FOREGROUND_YELLOW);
		}
		free(value);
	}
	free(listCraftableItem);
}
void goToStorage(player *player, storage *storage) {
	bool quit = false;
	while(!quit) {
		printInterfaceStorage(storage);
		char* value = malloc(sizeof(char) * 255);
		fflush(stdin);
		scanf("%s", value);
		cleanTerminal();
		if (tolower(value[0]) == 'a') {

		} else if (tolower(value[0]) == 'z') {

		} else if (tolower(value[0]) == 'o') {
			quit = true;
		} else {
			printc("erreur goToStorage() \n", 1, FOREGROUND_YELLOW);
		}
		free(value);
	}
}
void goToCrafting(player *player, item **listCraftableItem) {
	bool quit = false;
	while(!quit) {
		printInterfaceCrafting(listCraftableItem);
		int value;
		fflush(stdin);
		scanf("%d", &value);
		cleanTerminal();
		if(value == 0)
			return;
		for(int i=0 ; listCraftableItem[i] != NULL ; i++) {
			if(listCraftableItem[i]->id == value) {
				// crafter l'item
			}
		}
		printc("id de d'item a crafter inconnu \n", 1, FOREGROUND_YELLOW);
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
	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
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
void printInterfacePlayer() {

	printc(	"\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
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
	printc(		   "   A - Potion               ",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(										"O - Quitter",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(													"   |\n"
		   		"    |                                          |\n"
		   		"    |  ,----------------------------------------,\n"
		   		"    \\_/________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");

}
void printInterfacePNJ() {
	cleanTerminal();
	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                       |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Bienvenue a l'atelier !   Tous vos objets ont ete repare",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
				   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Attention ou vous mettez les doigts !",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
				   "    |                                                    |\n"
				   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Que souhaitez vous faire ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
				   "    |                                                    |\n"
				   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "A - Acceder au stockage",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "Z - Acceder au catalogue de craft",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(				   			 "      |\n"
										"    |                                       |\n"
										"    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Quitter l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "                 |\n"
												"    |                                       |\n"
												"    |                                       |\n"
												"    |  ,-------------------------------------,\n"
												"    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printInterfaceStorage(storage *storage) {
	cleanTerminal();
	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                       |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Voici le stocake",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);

	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Que souhaitez vous faire ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "A - Deposer du matos",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "Z - Recuperer mon precieux",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(				   			 "      |\n"
										"    |                                       |\n"
										"    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Revenir a l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "                 |\n"
												"    |                                       |\n"
												"    |                                       |\n"
												"    |  ,-------------------------------------,\n"
												"    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printInterfaceCrafting(item **listCraftableItem) {
	cleanTerminal();
	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                       |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Voici l'espace de conception",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);

	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Quel objet souhaitez vous creer ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "     |\n"
															 "    |                                                    |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);

	for(int i=0 ; listCraftableItem[i] != NULL ; i++) {
		setText(2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
		printf("%d - %s",listCraftableItem[i]->id, listCraftableItem[i]->name);
		printc(											      "     |\n"
																 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	}

	printc(				   			 "                             |\n"
										"    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Revenir a l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "                 |\n"
												"    |                                       |\n"
												"    |                                       |\n"
												"    |  ,-------------------------------------,\n"
												"    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

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
