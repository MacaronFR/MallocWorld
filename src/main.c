#include <player.h>
#include <inventory.h>
#include <item.h>
#include <monster.h>
#include <craft.h>
#include <terminalManager.h>
#include <perlin.h>
#include <resource.h>
#include <respawn.h>

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
	item **itemList = load_items("..", &nItem);
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

int main(int argc, char **argv) {
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

int main2(int argc, char **argv) {
	/*int*** map = generateMap(123);
	player *player = createPlayer();*/
	//testPlayer(player);
	/*player *player = createPlayer();
	testPlayer(player);
	//testInventory(player->inventory);

	testItem();
	*/
	/*
	inventory *inv = createInventory();
	initInventory(inv);
	storage *s = createStorage();
	size_t nItem;
	item **itemList = load_items("../", &nItem);
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
	item **itemList = load_items("../", &nItem);
	resource **resourceList = loadResources("../", &nResource, itemList, nItem);
	monster **monsterList = loadMonsters("../", &nMonstre);
	addMonsterRespawn(monsterList[0], &r, 0, 0);
	addResourceRespawn(resourceList[0], &r, 0,1);
	checkRespawn(&r);
	checkRespawn(&r);
	freeItemList(itemList, nItem);
	freeResourceList(resourceList, nResource);
	freeMonsterList(monsterList, nMonstre);
	freeRespawnList(r);
	return 0;*/
}