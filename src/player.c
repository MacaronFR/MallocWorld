#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include <player.h>
#include <perlin.h>
#include <ctype.h>


//---------------------- Creation et Destruction ----------------------
player *createPlayer(item **listItem, size_t nItem) {
    player *player1 = malloc(sizeof(player));
	if(player1 == NULL){
		return NULL;
	}
	inventory* inventory = createInventory();
	if(inventory == NULL){
		free(player1);
		return NULL;
	}
	player1->inventory = inventory;
	player1->level = 1;
	player1->life = 100;
	player1->maxLife = 100;
	player1->exp = 0;
	player1->stuff = NULL;
	player1->relative_coord.y = 4;
	player1->relative_coord.x = 4;
    return player1;
}

stuff *createStuff() {
	stuff *s = malloc(sizeof(stuff));
	s->armor = NULL;
	s->weapon = NULL;
}

void freePlayer(player *player) {
    freeInventory(player->inventory);
    free(player);
}

//---------------------- Etat du player ----------------------
bool playerIsDead(player *player) {
    return player->life <= 0;
}
void playerTakeDamage(player *player, uint16_t amount) {
	if(player->stuff->armor != NULL) {
		float reduction = (player->stuff->armor->damage / 100.f);
		player->life -=  (amount - (int)(amount * reduction));
	}
	else {
		player->life -= amount;
	}
}
void playerWinExp(player *player, uint16_t exp) {
	player->exp += exp;
	if(player->level < 10 && player->exp >= 100*player->level) {
		playerLevelUp(player);
	}
}
void playerLevelUp(player *player) {
	player->exp = player->exp % (100 * player->level);
	player->level++;

	if(player->level <= 5)
		player->maxLife += 10*(player->level - 1);
	else if(player->level <= 8)
		player->maxLife += 50;
	else
		player->maxLife += 75;
	player->life = player-> maxLife;
}


//|--------------------------------------------| ACTION |--------------------------------------------|
//---------------------- Fight ----------------------
void playerChooseStuff(player *player) {
	stuff *stuff = createStuff();
	item **listArmors = getItemCategory(player->inventory,ARMORS);
	if(listArmors != NULL) {
		printChooseArmor(player,listArmors);
		int res;
		fflush(stdin);
		scanf("%d", &res);
		cleanTerminal();
		int index = indexSlotInInventory(player->inventory, res, 0);
		if (index != -1) {
			stuff->armor = player->inventory->slots[index].item;
		}
		free(listArmors);
	}
	item **listWeapons = getItemCategory(player->inventory,WEAPONS);
	if(listWeapons != NULL) {
		printChooseWeapon(player,listWeapons);
		int res;
		fflush(stdin);
		scanf("%d", &res);
		cleanTerminal();
		int index = indexSlotInInventory(player->inventory, res, 0);
		if(index != -1) {
			stuff->weapon = player->inventory->slots[index].item;
		}
		free(listWeapons);
	}
	player->stuff = stuff;
}
void printChooseArmor(player *player, item **listArmors) {
	printPlayer(player);

	printc(	    "\n /??\\??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????\\\n"
				   " \\_,|                                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Choisi ton armure :",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(						  "                                                              |\n"
									 "    |                                                                                     |\n"
									 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	for(int i=0 ; listArmors[i] != NULL ; i++) {
		setText(2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
		printf("%2d  - %20s  | Resistance : %3d  ",listArmors[i]->id, listArmors[i]->name, listArmors[i]->damage);
		printc(											      "                                 |\n"
																 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	}

	printc(				   			 "                                                                                 |\n"
										"    |                                                                                     |\n"
										"    |                                                                ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "0 - Ne rien faire",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                                     |\n"
												"    |                                                                                     |\n"
												"    |  ,-----------------------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printChooseWeapon(player *player, item **listWeapons) {
	printPlayer(player);

	printc(	    "\n /??\\??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????\\\n"
				   " \\_,|                                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Choisi ton arme :",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(						  "                                                                |\n"
									 "    |                                                                                     |\n"
									 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	for(int i=0 ; listWeapons[i] != NULL ; i++) {
		setText(2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
		printf("%2d  - %20s  | Degat : %3d  | Durabilit?? : %3d",listWeapons[i]->id, listWeapons[i]->name, listWeapons[i]->damage, listWeapons[i]->durability);
		printc(											      "                    |\n"
																 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	}

	printc(				   			 "                                                                                 |\n"
										"    |                                                                                     |\n"
										"    |                                                                ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "0 - Ne rien faire",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                                     |\n"
												"    |                                                                                     |\n"
												"    |  ,-----------------------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}
void printChoosePotion(player *player, item **listPotion) {
	printPlayer(player);

	printc(	    "\n /??\\??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????\\\n"
				   " \\_,|                                                                                     |\n"
				   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "Choisi ton arme :",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(						  "                                                                |\n"
									 "    |                                                                                     |\n"
									 "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	for(int i=0 ; listPotion[i] != NULL ; i++) {
		setText(2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
		printf("%2d  - %20s  | Soin : %3d ",listPotion[i]->id, listPotion[i]->name, listPotion[i]->damage);
		printc(											      "                    |\n"
																 "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	}

	printc(				   			 "                                                                                 |\n"
										"    |                                                                                     |\n"
										"    |                                                                ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(		 		  "0 - Ne rien faire",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	printc(									 "    |\n"
												"    |                                                                                     |\n"
												"    |                                                                                     |\n"
												"    |  ,-----------------------------------------------------------------------------------,\n"
												"    \\_/___________________________________________________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);

	printf("\n\n");
}

int playerTurnFight(player *player, monster *monster, uint16_t monsterMaxLife) {
	int res = -1;
	while(res == -1) {
		printMonster(monster,monsterMaxLife);
		printFightIcon();
		printPlayer(player);
		playerInterfaceFight();
		char* value = malloc(sizeof (char) * 255);
		fflush(stdin);
		scanf("%s", value);
		cleanTerminal();
		if(tolower(value[0])  == 'a') {
			res = playerDoDamage(player, monster);
		}
		else if(value[0] == 'z') {
			res = playerSwitchWeapon(player);
		}
		else if(value[0] == 'e') {
			res = playerSwitchArmor(player);
		}
		else if(value[0] == 'r') {
			res = playerUsePotion(player);
		}
		else if(value[0] == 't') {
			res = playerEscape(player);
		}
		else {
			printc("\nL'action sp??cifi?? est incorrecte\n", 1, FOREGROUND_YELLOW);
			res = -1;
		}
		free(value);
	}
	return res;
}
int playerDoDamage(player *player, monster *monster) {
	if (player->stuff->weapon != NULL && player->stuff->weapon->durability > 0) {
		int amount = (player->stuff->weapon->damage);
		if(amount >= monster->life)
			monster->life = 0;
		else {
			monster->life -= player->stuff->weapon->damage;
		}
		player->stuff->weapon->durability--;
	} else {
		monster->life -= 1;
	}
	return 0;
}
int playerSwitchWeapon(player *player) {
	item** tabItem = getItemCategory(player->inventory, WEAPONS);
	if(tabItem == NULL) {
		printc("\nTu n'as pas d'autre arme, si tu n'es pas content d??merde toi avec tes poings! \n",1,FOREGROUND_YELLOW);
		return -1;
	}
	printChooseWeapon(player, tabItem);
	int value;
	fflush(stdin);
	scanf("%d", &value);
	cleanTerminal();
	int index = indexSlotInInventory(player->inventory,value,0);
	if(index != -1) {
		player->stuff->weapon = tabItem[index];
		return 0;
	}
	else if(value == 0) {
		printf("\nR??fl??chi avant de lancer une action la prochaine fois (-_-)\n");
		return -1;
	}
	else {
		printf("\nTu veux bien apprendre ?? lire ? ??a me fera des vacances... (-_-)\n");
		return -1;
	}
}
int playerSwitchArmor(player *player) {
	item** tabItem = getItemCategory(player->inventory, ARMORS);
	if(tabItem == NULL) {
		printf("\nT'as pas d'armure mon gars, va falloir que t'encaisse sans broncher!   (??v`(O=('-'Q)\n");
		return -1;
	}
	printChooseArmor(player, tabItem);
	int value;
	fflush(stdin);
	scanf("%d", &value);
	cleanTerminal();
	int index = indexSlotInInventory(player->inventory,value,0);
	if(index != -1) {
		player->stuff->armor = tabItem[index];
		return 0;
	}
	else if(value == 0) {
		printf("\nR??fl??chi avant de lancer une action la prochaine fois (-_-)\n");
		return -1;
	}
	else {
		printf("\nTu veux bien apprendre ?? lire ? ??a me fera des vacances... (-_-)\n");
		return -1;
	}
}
int playerUsePotion(player *player) {
	item** tabItem = getItemCategory(player->inventory, POTIONS);
	if(tabItem == NULL) {
		printc("\nDes popo, des po..., ah non... :'( \n",1,FOREGROUND_YELLOW);
		return -1;
	}
	printChoosePotion(player, tabItem);
	int value;
	fflush(stdin);
	scanf("%d", &value);
	cleanTerminal();
	int index = indexSlotInInventory(player->inventory,value,0);
	if(index != -1) {
		player->life += tabItem[index]->damage;
		item *item = retrieveItemInInventory(player->inventory,value);
		free(item);
		return 0;
	}
	else if(value == 0) {
		printf("\nR??fl??chi avant de lancer une action la prochaine fois (-_-)\n");
		return -1;
	}
	else {
		printf("\nTu veux bien apprendre ?? lire ? ??a me fera des vacances... (-_-)\n");
		return -1;
	}
}
int playerEscape(player *player) {
	int res = (rand()%100);
	if(res < 30) {
		printc("\nCOURAGE FUYONS!!!!\n",1,FOREGROUND_YELLOW);
		return 3;
	}
	printc("\nHEY! Reviens la toi!\n",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
	return 0;

}

//---------------------- Map ----------------------
void playerMove(player *player, level *map, direction direction) {
	switch (direction) {
		case NORTH: {
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 0;
			player->abs_coord.y--;
			player->relative_coord.y--;
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 1;
			break;
		}
		case EAST: {
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 0;
			player->abs_coord.x++;
			player->relative_coord.x++;
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 1;
			break;
		}
		case SOUTH: {
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 0;
			player->abs_coord.y++;
			player->relative_coord.y++;
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 1;
			break;
		}
		case WEST: {
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 0;
			player->abs_coord.x--;
			player->relative_coord.x--;
			map[player->abs_coord.zone].level[player->abs_coord.y][player->abs_coord.x] = 1;
			break;
		}
		default:
			printc("error matrice : playerMoov",1,FOREGROUND_RED);
	}
}


//---------------------- AFFICHAGE ----------------------
void displayPlayerOnMap(player *p, level *map){
	int startx, starty, endx, endy;
	if(p->relative_coord.x < 3 && p->abs_coord.x >= 3){
		p->relative_coord.x++;
	}else if(p->relative_coord.x > 6 && p->abs_coord.x < map->w - 3){
		p->relative_coord.x--;
	}
	if(p->relative_coord.y < 3 && p->abs_coord.y >= 3){
		p->relative_coord.y++;
	}else if(p->relative_coord.y > 6 && p->abs_coord.y < map->h - 3){
		p->relative_coord.y--;
	}
	startx = p->abs_coord.x - p->relative_coord.x;
	startx = startx < 0 ? 0 : startx;
	starty = p->abs_coord.y - p->relative_coord.y;
	starty = starty < 0 ? 0 : starty;
	endx = p->abs_coord.x - p->relative_coord.x + 10;
	endx = endx < map->w ? endx : map->w;
	endy = p->abs_coord.y - p->relative_coord.y + 10;
	endy = endy < map->h ? endy : map->h;
	printf("\n");
	for(int i = starty; i < endy; ++i){
		printc("    +--+--+--+--+--+--+--+--+--+--+\n    |",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
		for(int j = startx; j < endx; ++j){
			setTextDefault();
			int id = map[p->abs_coord.zone].level[i][j];
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
			printc("|", 2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
		}
		printf("\n");
	}
	printc("    +--+--+--+--+--+--+--+--+--+--+\n",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf("\n\n");
}
void printPlayer(player *player) {
	printf("\n     @      | ");   printLevel(player);
	printf("    /|\\     | ");  printExp(player);
	printf("    / \\     | ");  printLife(player->life, player->maxLife);
	printInventory(player->inventory);
}
void printLevel(player *player) {
	setText(1,FOREGROUND_PURPLE);
	printf("Lvl  : %d\n", player->level);
	setTextDefault();
}
void printExp(player *player) {
	setText(1,FOREGROUND_GREEN);
	printf("Exp  : ");
	printBar(player->exp, player->level*100.f, 50);
	setTextDefault();
}
void printLife(uint16_t current, uint16_t max) {
	setText(1,FOREGROUND_RED);
	printf("Life : ");
	printBar(current, max,50);
	setTextDefault();
}
void printBar(float value, float max, int size) {
	char bar[size + 3];
	bar[0] = '[';
	int i;
	for(i=0;i<size;i++){
		if((i*(100/size)) <= (value/max)*100)
			bar[i+1]='=';
		else
			bar[i+1]='_';
	}
	bar[i+1]=']';
	bar[i+2]='\0';
	printf("%s %d/%d\n",bar, (int)value, (int)max);
}

void playerInterfaceFight() {

	printc(	"\n /??\\????????????????????????????????????????????????????????????????????????????????????\\\n"
			   " \\_,|                                          |\n"
			   "    |    ", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				"Combat en cours ...",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
	printc(							  				"                   |\n"
													   "    |                                          |\n"
													   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"Que souhaitez vous faire ?      ",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printc(													"      |\n"
			   "    |                                          |\n"
			   "    |                                          |\n"
			   "    |    ",2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printc(				   "A - Attaquer",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(							  				"                          |\n"
			   "    |                                          |\n"
			   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"Z - Changer d'arme",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(									"                    |\n"
			   "    |                                          |\n"
			   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"E - Changer d'armure    ",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(												"              |\n"
		       "    |                                          |\n"
			   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"R - Boire une potion",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(									"                  |\n"
			   "    |                                          |\n"
			   "    |    ",2,FOREGROUND_YELLOW,FOREGROUND_INTENSITY);
	printc(				"T - Prendre la fuite",2,FOREGROUND_CYAN,FOREGROUND_INTENSITY);
	printc(									"                  |\n"
			   "    |                                          |\n"
			   "    |                                          |\n"
			   "    |  ,----------------------------------------,\n"
			   "    \\_/________________________________________/", 2, FOREGROUND_YELLOW, FOREGROUND_INTENSITY);
	printf("\n\n");

}
void printMonster(monster *monster, uint16_t monsterMaxLife) {
	printf("\n     @      | ");   setText(1,FOREGROUND_PURPLE); printf("%s\n",monster->name);  setTextDefault();
	printf("    /|\\     | ");  printLife(monster->life, monsterMaxLife);
	printf("    / \\     | ");  printc("Degat :",2,FOREGROUND_RED,FOREGROUND_INTENSITY) ; printf("%d\n", monster->strength);
}
void printFightIcon() {
	printf("\n             <==========={}xxx)=(^o^)=(xxx{}===========>\n");
}