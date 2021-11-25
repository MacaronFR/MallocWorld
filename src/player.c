#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include <player.h>
#include <perlin.h>


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
	addItemInInventory(player1->inventory, getItem(listItem,nItem,1));
	addItemInInventory(player1->inventory, getItem(listItem,nItem,2));
	addItemInInventory(player1->inventory, getItem(listItem,nItem,3));
	addItemInInventory(player1->inventory, getItem(listItem,nItem,4));
    return player1;
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
	if(player->stuff != NULL) {
		int reduction = (player->stuff->armor->flag / 100);
		player->life -= amount - (amount * reduction);
	}
}
void playerWinExp(player *player, uint16_t exp) {}
void playerLevelUp(player *player) {}

//---------------------- Affichage ----------------------
void printPlayer(player *player) {
    printf(" @      | ");   printLevel(player);
    printf("/|\\     | ");  printExp(player);
    printf("/ \\     | ");  printLife(player);
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
void printLife(player *player) {
    setText(1,FOREGROUND_RED);
    printf("Life : ");
	printBar(player->life, player->maxLife,50);
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
//|--------------------------------------------| ACTION |--------------------------------------------|
//---------------------- Fight ----------------------
int playerTurnFight(player *player, monster *monster) {
	cleanTerminal();
	printPlayer(player);
	int choice = 0;
	int res = -1;
	char* value = malloc(sizeof (char) * (10 + 1));
	while(res == -1) {
		printc("Que souhaitez vous faire ?\n", 2, FOREGROUND_GREEN, FOREGROUND_INTENSITY);
		setText(2, FOREGROUND_BLUE, FOREGROUND_INTENSITY);
		printf("+---+----------+   +---+----------------+   +---+------------------+   +---+------------------+   +---+------------------+\n");
		printf("| 1 - Attaquer |   | 2 - Changer d'arme |   | 3 - Changer d'armure |   | 4 - Boire une potion |   | 5 - Prendre la fuite |\n");
		printf("+---+----------+   +---+----------------+   +---+------------------+   +---+------------------+   +---+------------------+\n");
		setTextDefault();

		fgets(value,10,stdin);
		fflush(stdin);
		if(value[0] == '1') {
			res = playerDoDamage(player, monster);
		}
		else if(value[0] == '2') {
			printf("vu\n");
			res = playerSwitchWeapon(player);
		}
		else if(value[0] == '3') {
			res = playerSwitchArmor(player);
		}
		else if(value[0] == '4') {
			res = playerUsePotion(player);
		}
		else if(value[0] == '5') {
			res = playerEscape(player);
		}
		else {
			printc("L'action spécifié est incorrecte\n", 1, FOREGROUND_YELLOW);
			res = -1;
		}
	}
	free(value);
	return res-1;
}
int playerDoDamage(player *player, monster *monster) {
	if (player->stuff != NULL) {
		int amount = (player->stuff->weapon->flag);
		monster->life -= amount;
	} else {
		monster->life -= 1;
	}
	return 1;
}
int playerSwitchWeapon(player *player) {
	item** tabItem = getItemCategory(player->inventory, WEAPONS);
	if(tabItem == NULL) {
		printf("Tu n'as pas d'autre arme, si tu n'es pas content démerde toi avec tes poings! \n");
		return -1;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value > 0 && value < 11 && tabItem[value-1]!=NULL) {
		player->stuff->weapon = tabItem[value-1];
		return 1;
	}
	else if(value == 11) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return -1;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return -1;
	}
}
int playerSwitchArmor(player *player) {
	item** tabItem = getItemCategory(player->inventory, ARMORS);
	if(tabItem == NULL) {
		printf("T'as pas d'armure mon gars, va falloir que t'encaisse sans broncher!   (°v`(O=('-'Q)\n");
		return -1;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value > 0 && value < 11 && tabItem[value-1]!=NULL) {
		player->stuff->armor = tabItem[value-1];
		return 1;
	}
	else if(value == 11) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return -1;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return -1;
	}
}
int playerUsePotion(player *player) {
	item** tabItem = getItemCategory(player->inventory, POTIONS);
	if(tabItem == NULL) {
		printf("Des popo, des po..., ah non... :'( \n");
		return 0;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value > 0 && value < 11 && tabItem[value-1]!=NULL) {
		player->life += tabItem[value-1]->flag;
		return 1;
	}
	else if(value == 11) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return 0;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return 0;
	}
}
int playerEscape(player *player) {
	printf("COURAGE FUYONS!!!!");
	return 2;
}
int playerDoChoiceCategory(item** tabItem) {
	printf("Fait ton choix :\n");
	for (int i = 0; tabItem[i] != NULL; i++) {
		printf("| %d - %s |", i+1, tabItem[i]->name);
	}
	printf("| 11 - Annuler |");
	char* value = malloc(sizeof (char) * (2 + 1));
	fgets(value,2,stdin);
	return atoi(value);
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
int playerCraft(player *player) {}
int playerMine(player *player) {}
int playerChopWood(player *player) {}
int playerCutGrass(player *player) {}
int playerCraftItem(player *player, int id) {}

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
	starty = p->abs_coord.y - p->relative_coord.y;
	endx = p->abs_coord.x - p->relative_coord.x + 10;
	endx = endx < map->w ? endx : map->w;
	endy = p->abs_coord.y - p->relative_coord.y + 10;
	endy = endy < map->h ? endy : map->h;


	for(int i = starty; i < endy; ++i){
		printc("+--+--+--+--+--+--+--+--+--+--+\n|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
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
	printc("+--+--+--+--+--+--+--+--+--+--+\n",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf("\n\n");
}