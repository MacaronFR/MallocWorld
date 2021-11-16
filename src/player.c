
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include <player.h>
#include <map.h>


//---------------------- Creation et Destruction ----------------------
player *createPlayer() {
    player *player1 = malloc(sizeof(player));
    initPlayer(player1);
    return player1;
}
void initPlayer(player *player) {
    inventory* inventory = createInventory();
    initInventory(inventory);
    player->inventory = inventory;
    player->level = 1;
    player->life = 100;
    player->maxLife = 100;
    player->exp = 0;
	player->stuff = NULL;
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
    printf("Lvl : %d\n", player->level);
    setTextDefault();
}
void printExp(player *player) {
    setText(1,FOREGROUND_GREEN);
    printf("Exp : %d\n", player->exp);
    setTextDefault();
}
void printLife(player *player) {
    setText(1,FOREGROUND_RED);
    printf("Life : %d / %d\n", player-> life, player->maxLife);
    setTextDefault();
}


//|--------------------------------------------| ACTION |--------------------------------------------|
//---------------------- Fight ----------------------
int playerTurnFight(player *player, monster *monster) {
	int value, res;
	while(res != 1) {
		setText(1, FOREGROUND_GREEN);
		printf("Que souhaitez vous faire ?\n");
		setText(2, FOREGROUND_BLUE, FOREGROUND_INTENSITY);
		printf("1 - Attaquer    ");
		printf("2 - Changer d'arme    ");
		printf("3 - Changer d'armure    ");
		printf("4 - Boire une potion    ");
		printf("5 - Prendre la fuite    ");

		scanf("%d", &value);
		switch (value) {
			case 1: {
				res = playerDoDamage(player, monster);
			}
			case 2: {
				res = playerSwitchWeapon(player);
			}
			case 3: {
				res = playerSwitchArmor(player);
			}
			case 4: {
				res = playerUsePotion(player);
			}
			case 5: {
				res = playerEscape(player);
			}
			default: {
				setText(1, FOREGROUND_YELLOW);
				printf("L'action spécifié est incorrecte\n");
				setTextDefault();
				res = -1;
			}
		}
	}
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
	item* tabItem[MAX_SLOTS_INVENTORY];
	inventoryContainCategory(player->inventory, WEAPONS, tabItem);
	if(tabItem[0] == NULL) {
		printf("Tu n'as pas d'autre arme, si tu n'es pas content démerde toi avec tes poings! \n");
		return -1;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value >= 0 && value < 10 && tabItem[value]!=NULL) {
		player->stuff->weapon = tabItem[value];
		return 1;
	}
	else if(value == 10) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return -1;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return -1;
	}
}
int playerSwitchArmor(player *player) {
	item* tabItem[MAX_SLOTS_INVENTORY];
	inventoryContainCategory(player->inventory, ARMORS, tabItem);
	if(tabItem[0] == NULL) {
		printf("T'as pas d'armure mon gars, va falloir que t'encaisse sans broncher! \n");
		return -1;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value >= 0 && value < 10 && tabItem[value]!=NULL) {
		player->stuff->armor = tabItem[value];
		return 1;
	}
	else if(value == 10) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return -1;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return -1;
	}
}
int playerUsePotion(player *player) {
	item* tabItem[MAX_SLOTS_INVENTORY];
	inventoryContainCategory(player->inventory, POTIONS, tabItem);
	if(tabItem[0] == NULL) {
		printf("Des popo, des po..., ah non... :'( \n");
		return -1;
	}
	int value = playerDoChoiceCategory(tabItem);
	if(value >= 0 && value < 10 && tabItem[value]!=NULL) {
		player->life += tabItem[value]->flag;
		return 1;
	}
	else if(value == 10) {
		printf("Réfléchi avant de lancer une action la prochaine fois -_-");
		return -1;
	}
	else {
		printf("Tu veux bien apprendre à lire ? ça me fera des vacances... -_-");
		return -1;
	}
}
int playerEscape(player *player) {
	printf("COURAGE FUYONS!!!!");
	return 2;
}
int playerDoChoiceCategory(item** tabItem) {
	printf("Fait ton choix :\n");
	for (int i = 0; tabItem[i] != NULL; i++) {
		printf("| %d - %s |", i, tabItem[i]->name);
	}
	printf("| 10 - Annuler |");
	int value;
	scanf("%d", &value);
	return value;
}

//---------------------- Map ----------------------
int playerMoov(player *player, direction direction) {}
int playerCraft(player *player) {}
int playerMine(player *player) {}
int playerChopWood(player *player) {}
int playerCutGrass(player *player) {}
int playerCraftItem(player *player, int id) {}



