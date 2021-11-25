#include <player.h>
#include <inventory.h>
#include <item.h>
#include <monster.h>
#include <craft.h>
#include <terminalManager.h>
#include <perlin.h>
#include <resource.h>
#include <respawn.h>
#include <time.h>
#include <ctype.h>
#include <game.h>
#include <save.h>

void printFlag(int pos, char *class) {
	setText(1, FOREGROUND_YELLOW);
	if (pos == 0)
		printf("\n|-------------------| start TEST %s |-------------------\n", class);
	else if (pos == 1)
		printf("|-------------------| end TEST %s |-------------------\n", class);
	else {
		setText(1, FOREGROUND_RED);
		printf("ERROR 'printFlag' : pos can have value [0,1]");
		setTextDefault();
	}
	setTextDefault();
}

void printPerlin(int res, FILE *f) {
	fprintf(f, "%2d", res);
}

void testPlayer(player *player) {
	printFlag(0, "PLAYER");
	printPlayer(player);
	printFlag(1, "PLAYER");
}

void testInventory(inventory *inventory) {
	printFlag(0, "INVENTORY");
	printInventory(inventory);
	printFlag(1, "INVENTORY");
}

void testItem() {
	printFlag(0, "ITEM");
	//item *res = load_item("../test.mw");
	//if(res == NULL){
	//return 1;
	//}
	//free(res);
	size_t nItem;
	item **itemList = loadItems("..", &nItem);
	if (itemList != NULL) {
		if (checkCraftValidity(itemList, nItem)) {
			setText(1,FOREGROUND_GREEN);
			printf("testLoadItem : OK\n");
			setTextDefault();
		} else {
			setText(1,FOREGROUND_RED);
			printf("testLoadItem : NIKK\n");
			setTextDefault();
		}
		freeItemList(itemList, nItem);
	}

	printFlag(1, "ITEM");
}

void testMacaron(int argc, char **argv) {
	testItem();
	/*
	FILE *f = fopen("/home/macaron/Documents/res.csv", "w");
	int portal[4][2];
	int ***map = generateMap(atoi(argv[1]), portal);
	for(int k = 0; k < 3; ++k){
		for(int i = 0; i < 100; ++i){
			for(int j = 0; j < 100; ++j){
				fprintf(f, "%2d", map[k][i][j]);
				if(j != 99) fprintf(f, ", ");
			}
			fprintf(f, "\n");
		}
		for(int i = 0; i < 100; i++){
			fprintf(f, "-1");
			if(i != 99) fprintf(f, ", ");
		}
		fprintf(f, "\n");
	}
	freeMap(map, 3, 100);
	fclose(f);
	for(int i = 0; i < 4; ++i){
		printf("portal %d (%d, %d)\n", i, portal[i][0], portal[i][1]);
	}
	return 0;
	/*monster *tmp = loadMonster("../monster.mw");
	freeMonster(tmp);
	return 0;*/
}

int mainT(int argc, char **argv) {
	//int*** map = generateMap(123);
	/*player *player = createPlayer();
	playerTurnFight(player, NULL);*/




	/*
	inventory *inv = createInventory();
	initInventory(inv);
	storage *s = createStorage();
	size_t nItem;
	item **itemList = loadItems("../", &nItem);
	addItemInInventory(inv, copyItem(itemList[0]));
	addItemInInventory(inv, copyItem(itemList[0]));
	addItemInStorage(s, copyItem(itemList[0]));
	item *tmp = craftItem(inv, s, itemList[1]);
	fprintf(stderr, "item {\n\ttype = %d,\n\tid = %d,\n\tflag = %d,\n\tdurability = %d\n\tmaxStack = %d\n", tmp->type, tmp->id, tmp->flag, tmp->durability, tmp->maxStack);
	int i = 0;
	fprintf(stderr, "\tcraft : [");
	while(tmp->craft[i] != 0){
		fprintf(stderr, "%d", tmp->craft[i]);
		if(tmp->craft[i + 1] != 0){
			fprintf(stderr, ", ");
		}
		++i;
	}
	fprintf(stderr, "]\n\tname : \"%s\"\n}\n", tmp->name);
	addItemInStorage(s,tmp);
	tmp = craftItem(inv, s, itemList[1]);
	if(tmp == NULL){
		printf("Incraftable");
	}
	freeInventory(inv);
	freeStorage(s);
	freeItemList(itemList, nItem);
	return 0;*/

/*
	respawn *r = NULL;
	size_t nItem, nResource, nMonstre;
	item **itemList = loadItems("../", &nItem);
	resource **resourceList = loadResources("../", &nResource, itemList, nItem);
	monster **monsterList = loadMonsters("../", &nMonstre);
	int portal[4][2];
	int ***map = generateMap(atoi(argv[1]), portal);
	addMonsterRespawn(monsterList[0], &r, 0, 0, 0);
	addResourceRespawn(resourceList[0], &r, 0,1, 0);
	printf("%d %d\n", map[0][0][0], map[0][0][1]);
	checkRespawn(&r, map);
	printf("%d %d\n", map[0][0][0], map[0][0][1]);
	checkRespawn(&r, map);
	printf("%d %d\n", map[0][0][0], map[0][0][1]);
	checkRespawn(&r, map);
	printf("%d %d\n", map[0][0][0], map[0][0][1]);
	freeItemList(itemList, nItem);
	freeResourceList(resourceList, nResource);
	freeMonsterList(monsterList, nMonstre);
	freeRespawnList(r);
	freeMap(map, 3, 100);
	return 0;*/

	/*int portal[4][2];
	int l;
	size_t nItem, nResource, nMonster;
	item **itemList = loadItems("./items/", &nItem);
	resource **resourceList = loadResources("./resources/", &nResource, itemList, nItem);
	monster **monsterList = loadMonsters("./monsters/", &nMonster);
	storage *s = createStorage();
	player *p1 = createPlayer();
	respawn *r = NULL;
	level *map = loadSave("./saves/test.mw", &r, p1, s, portal, &l, itemList, nItem, resourceList, nResource, monsterList, nMonster);
	for(int i = 0; i < s->size; ++i){
		printf("%d, %d, %d\n", i, s->slots[i].item->id, s->slots[i].quantity);
	}
	saveGame("./saves/new.mw", map, r, p1, s, l);
	freePlayer(p1);
	freeMap(map, l);
	freeItemList(itemList, nItem);
	return 0;*/
}

int mainF() {
	int portal[4][2];
	player *p1 = createPlayer();
	p1->abs_coord.x = 0;
	p1->abs_coord.y = 0;
	p1->abs_coord.zone = 0;
	p1->relative_coord.x = 0;
	p1->relative_coord.y = 0;
	level *map = generateMap(1, portal);
	map[0].level[0][0] = 1;
	displayPlayerOnMap(p1, map);
	printf("\n");
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, EAST);
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, SOUTH);
	playerMove(p1, map, SOUTH);
	displayPlayerOnMap(p1, map);
	printf("\n");
	playerMove(p1, map, SOUTH);
	displayPlayerOnMap(p1, map);
	printf("\n");
	freeMap(map, 3);
	freePlayer(p1);
}

int main(int argc, char **argv){
	char filename[256];
	bool res = false;
	int r = 0, l;
	respawn *respawnList = NULL;//Liste de respawn
	size_t nItem, nResource, nMonster;
	item **listItem = loadItems("../items/", &nItem);
	if(listItem != NULL){
		printc("ListItem loaded!\n",1,FOREGROUND_GREEN);
		resource **listResource = loadResources("../resources/", &nResource, listItem, nItem);
		if(listResource != NULL){
			printc("ListResource loaded!\n",1,FOREGROUND_GREEN);
			monster **listMonster = loadMonsters("../monsters/", &nMonster);
			if(listMonster != NULL){
				printc("ListMonster loaded!\n",1,FOREGROUND_GREEN);
				player *player1 = createPlayer();
				if(player1 != NULL){
					printc("Player created!\n",1,FOREGROUND_GREEN);
					storage *storage = createStorage();
					if(storage != NULL){
						printc("Storage created!\n",1,FOREGROUND_GREEN);
						int portal[4][2];
						level *map;
						char* value = malloc(sizeof(char) * 255);
						printStartMenu();
						scanf("%s", value);
						if(tolower(value[0]) == '1'){
							srand(time(NULL));
							map = generateMap(rand(), portal);
							player1->abs_coord.x = portal[0][0];
							player1->abs_coord.y = portal[0][1] - 1;
							player1->abs_coord.zone = 0;
							map[player1->abs_coord.zone].level[player1->abs_coord.y][player1->abs_coord.x] = 1;
							l = 3;
							printc("Level & maps created!\n",1,FOREGROUND_GREEN);
							inGame(player1,map,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,&respawnList,l);
						} else if(tolower(value[0]) == '2'){
							map = loadSave("./saves/test.mw", &respawnList, player1, storage, portal, &l, listItem, nItem, listResource, nResource, listMonster, nMonster);
							for(int i = 0; i < 4; ++i){
								printf("%d, %d\n", portal[i][0], portal[i][1]);
							}						}
						else if(tolower(value[0]) == '3'){
							printCredit();
						}
						else{
							printc("J'ai venu, j'ai lu, j'ai pas comprendu",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
						}
						free(value);
						if(map != NULL){
							//game();
							do{
								if(res){
									printf("Erreur lors de la sauvegarde\n");
								}
								printf("sauvegarder à : ");
								scanf("%s", filename);
								res = saveGame(filename, map, respawnList, player1, storage, l);
							}while(!res);
							freeMap(map, 3);
							freeRespawnList(respawnList);
						}else r = -6;
						freeStorage(storage);
					}else r = -5;
					freePlayer(player1);
				}else r = -4;
				freeMonsterList(listMonster, nMonster);
			}else r = -3;
			freeResourceList(listResource, nResource);
		}else r = -2;
		freeItemList(listItem, nItem);
	}else r = -1;
	return r;
}