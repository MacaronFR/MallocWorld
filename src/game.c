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

void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int nbMap, int portal[4][2]) {
	cleanTerminal();
	bool end = false;
	while(!end) {;
		switch (playerTurn(player, map, storage, listItem,nItem, listResource,nResource, listMonster,nMonster, respawnList, portal)){
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
					strcpy(filename, "./saves/");
					do {
						printc("sauvegarder à : ", 2, FOREGROUND_GREEN, FOREGROUND_INTENSITY);
						scanf("%s", filename + 8);
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

int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int portal[4][2]) {
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
			res = tryMove(player,map,NORTH,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y-1, portal);
		} else if (tolower(value[0]) == 'd') {
			res = tryMove(player,map,EAST,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x+1,player->abs_coord.y, portal);
		} else if (tolower(value[0]) == 's') {
			res = tryMove(player,map,SOUTH,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,respawnList,player->abs_coord.x,player->abs_coord.y+1, portal);
		} else if (tolower(value[0]) == 'q') {
			res = tryMove(player, map, WEST,storage, listItem, nItem, listResource, nResource, listMonster, nMonster, respawnList, player->abs_coord.x - 1, player->abs_coord.y, portal);
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

int tryMove(player *player, level *map, direction direction,storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int x, int y, int portal[4][2]) {
	monster *m;
	if(	direction == NORTH && player->abs_coord.y-1 < 0 ||
	direction == EAST && player->abs_coord.x+1 >= map->w ||
	direction == SOUTH && player->abs_coord.y+1 >= map->h ||
	direction == WEST && player->abs_coord.x - 1 < 0) {//sortie de map
		printc("La Terre est plate mon gars, tu ne peux pas aller plus loin! ¯\\_(\"/)_/¯",1,FOREGROUND_YELLOW);
		return 0;
	}

	int id, resCase, res;
	id = map[player->abs_coord.zone].level[y][x];
	resCase = checkCaseIdType(id,listResource,nResource,listMonster,nMonster);
	printf("resCase = %d\n", resCase);
	switch(resCase) {
		case -3:
		case -2:
			return teleport(id, player, map, portal);
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
			m = createMonster(findMonster(listMonster,nMonster,id));
			res = fight(player, m, respawnList,x,y, listMonster, nMonster);
			freeMonster(m);
			if(res == 1 || res == 2)
				playerMove(player,map,direction);
			return res;
	}
}

bool teleport(int32_t id, player *p1, level *map, int portal[4][2]){
	if(id == -3 && p1->abs_coord.zone == 2){
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 0;
		p1->abs_coord.zone = 1;
		p1->abs_coord.x = portal[3][0];
		p1->abs_coord.y = portal[3][1] - 1;
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 1;
		return true;
	}else if(id == -3 && p1->abs_coord.zone == 1 && p1->level > 7){
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 0;
		p1->abs_coord.zone = 2;
		p1->abs_coord.x = portal[2][0];
		p1->abs_coord.y = portal[2][1] - 1;
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 1;
		return true;
	}else if(id == -2 && p1->abs_coord.zone == 1){
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 0;
		p1->abs_coord.zone = 0;
		p1->abs_coord.x = portal[0][0];
		p1->abs_coord.y = portal[0][1] - 1;
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 1;
		return true;
	}else if(id == -2 && p1->abs_coord.zone == 0 && p1->level > 7){
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 0;
		p1->abs_coord.zone = 1;
		p1->abs_coord.x = portal[1][0];
		p1->abs_coord.y = portal[1][1] - 1;
		map[p1->abs_coord.zone].level[p1->abs_coord.y][p1->abs_coord.x] = 1;
		return true;
	}
	return false;
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
			if((tmp->type & 4) != 0) {
				int count = (int)(rand()%4);
				for(int j=0 ; j<=count ; j++){
					addItemInInventory(player->inventory, copyItem(tmp));
				}
				freeItem(tmp);
			}else{
				addItemInInventory(player->inventory, tmp);
			}
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
int fight(player *player, monster *m, respawn **listRespawn, int32_t x, int32_t y, monster **monsterList, size_t nMonster){
	int res = 0;
	uint16_t monsterMaxLife = m->life;
	playerChooseStuff(player);
	while(res == 0) {
		res = playerTurnFight(player, m, monsterMaxLife);
		if(m->life <= 0)
			res = 1;
		else if(res != 3){
			playerTakeDamage(player, m->strength);
			if(playerIsDead(player))
				res = -2;
		}
	}
	free(player->stuff);
	player->stuff = NULL;
	switch(res) {
		case -2: {
			printc("Vous avez été découpé, brulé, broyé... Quelle mort tragique... :'(\n", 1, FOREGROUND_RED);
			return -2;
		}
		case 1: {
			addMonsterRespawn(findMonster(monsterList, nMonster, m->id), listRespawn, x, y, player->abs_coord.zone);
			playerWinExp(player, m->exp);
			if(m->id == 99)
				return 2;
			return res;
		}
		case 3: {
			printc("Vous avez fuit le combat ?!  Tapette !!!\n", 1, FOREGROUND_YELLOW);
			return res;
		}
		default: {
			printc("BUG dans la matrice : fight\n", 1, FOREGROUND_RED);
		}
	}
}
void interactWithPNJ(player *player, storage *storage, item **listItem, size_t nItem) {
	printf("interactWithPNJ");
	repairInventory(listItem,nItem,player->inventory);
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
			goToCrafting(player,storage,listCraftableItem);
		} else if (tolower(value[0]) == 'o' || tolower(value[0]) == '0') {
			quit = true;
		} else {
			printc("Connait pas ce truc, c'est quoi ? \n", 1, FOREGROUND_RED);
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

		} else if (tolower(value[0]) == 'o' || tolower(value[0]) == '0') {
			quit = true;
		} else {
			printc("Y a pas 36 000 choix, tu pose ou tu prend c'est tout.\n", 1, FOREGROUND_YELLOW);
		}
		free(value);
	}
}

void goToCrafting(player *player,storage *storage, item **listCraftableItem) {
	int value;
	int verif = 1;
	item *craft;
	while(verif == 1) {
		printStorage(storage);
		printInterfaceCrafting(listCraftableItem, storage);
		fflush(stdin);
		verif = scanf("%d", &value);
		cleanTerminal();
		printf("%d, %d\n", verif, value);
		if(verif != 1 || listCraftableItem[value] == NULL){
			printc("Ça marche pas visiblement\n", 1, FOREGROUND_YELLOW);
			continue;
		}
		craft = craftItem(player->inventory, storage, listCraftableItem[value]);
		if(craft == NULL){
			printc("Tu as pas les ressources\n", 1, FOREGROUND_YELLOW);
			return;
		}
		printc("C'est fait", 1, FOREGROUND_GREEN);
		addItemInInventory(player->inventory, craft);
	}
}

void gameOver() {
	printc("GAME OVER !!!\n",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
}
void winGame() {
	printc("VOUS AVEZ GAGNE !!!\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
}

//// --------------------------------- AFFICHAGE ---------------------------------
void printCredit() {
	printc("Jeux Mallocworld développé par :\n"
		   "	-	Denis TURBIEZ		@Macaron\n"
		   "	-	Basile PULIN		@Barlords\n"
		   "	-	Jean-Jaures OKA		@Jean\n",
		   2,FOREGROUND_GREEN,FOREGROUND_INTENSITY
	);
}
void printStartMenu() {
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
										"    |                                       |\n"
										"    |                 ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "0 - Quitter le jeu",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
				    "    |                                       |\n"
				    "    |                                       |\n"
				    "    |  ,-------------------------------------,\n"
				    "    \\_/_____________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");
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
	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Bienvenue a l'atelier !   Tous vos objets ont ete repare",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(											      "         |\n"
				   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Attention ou vous mettez les doigts !",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(											             "                            |\n"
				   "    |                                                                     |\n"
				   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Que souhaitez vous faire ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "                                       |\n"
															 "    |                                                                     |\n"
				   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "A - Acceder au stockage",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(											      "                                          |\n"
															 "    |                                                                     |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "Z - Acceder au catalogue de craft",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(				   			 "                                |\n"
										"    |                                                                     |\n"
										"    |                                                                     |\n"
										"    |                                            ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Quitter l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                     |\n"
												"    |                                                                     |\n"
												"    |  ,-------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printInterfaceStorage(storage *storage) {

	printStorage(storage);

	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Voici le stocage, ne ramassez pas tous ce que vous trouvez SVP    (-_-')",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);

	printc(											      "         |\n"
															 "    |                                                                                     |\n"
															 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Que souhaitez vous faire ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "                                                       |\n"
															 "    |                                                                                     |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "A - Deposer du matos",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(											      "                                                             |\n"
															 "    |                                                                                     |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				   "Z - Recuperer mon precieux",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(				   			 "                                                       |\n"
										"    |                                                                                     |\n"
										"    |                                                                                     |\n"
										"    |                                                          ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Revenir a l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                                     |\n"
												"    |                                                                                     |\n"
												"    |  ,-----------------------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printInterfaceCrafting(item **listCraftableItem, storage *storage) {

	printStorage(storage);

	printc(	    "\n /¯\\¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\\n"
				   " \\_,|                                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Voici l'espace de conception.",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(						  "                                                    |\n"
									 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(						  "Bon ce n'est pas comme la cite de l'innovation mais on fait comme on peux",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);

	printc(											      "        |\n"
															 "    |                                                                                     |\n"
															 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Quel objet souhaitez vous creer ?",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(											      "                                                |\n"
															 "    |                                                                                     |\n"
															 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);

	for(int i=0 ; listCraftableItem[i] != NULL ; i++) {
		setText(2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
		printf("%2d - %20s",i, listCraftableItem[i]->name);
		printc(											      "                                                        |\n"
																 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	}

	printc(				   			 "                                                                                 |\n"
										"    |                                                                                     |\n"
										"    |                                                          ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "O - Revenir a l'atelier",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                                     |\n"
												"    |                                                                                     |\n"
												"    |  ,-----------------------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

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
