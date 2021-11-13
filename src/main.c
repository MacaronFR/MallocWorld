#include <map.h>
#include <player.h>
#include <inventory.h>
#include <item.h>
#include <monster.h>
#include <terminalManager.h>

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

int testMacaron(int argc, char **argv) {
	/*item *res = load_item("../test.mw");
	if(res == NULL){
		return 1;
	}
	free(res);*/
	size_t nItem;
	item **itemList = load_items("..", &nItem);
	if (itemList != NULL) {
		if (checkCraftValidity(itemList, nItem)) {
			printf("OK");
		} else {
			printf("NIKK");
		}
		freeItemList(itemList, nItem);
	}
	/*
	FILE *f = fopen("/home/macaron/Documents/res.csv", "w");
	int ***map = generateMap(atoi(argv[1]));
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
	return 0;*/
	/*monster *tmp = loadMonster("../monster.mw");
	deleteMonster(tmp);
	return 0;*/
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
			printf("testLoadItem : OK\n");
		} else {
			printf("testLoadItem : NIKK\n");
		}
		freeItemList(itemList, nItem);
	}


	printFlag(1, "ITEM");
}


int main(int argc, char **argv) {
	player *player = createPlayer();
	//testPlayer(player);
	//testInventory(player->inventory);

	testItem();

	return 0;
}