#include <game.h>
#include <unistd.h>

void setDir(char *s){
	char *command = malloc(strlen(s) + 1);
	strcpy(command, s);
	char *a = strrchr(command, '/');
	a[1] = 0;
	chdir(command);
	system("pwd");
}

int main(int argc, char **argv){
	setDir(argv[0]);
	int r = 0, l;
	int portal[4][2];
	level *map = NULL;
	respawn *respawnList = NULL;//Liste de respawn
	size_t nItem, nResource, nMonster;
	item **listItem = loadItems("../items/", &nItem);
	if(listItem != NULL && checkCraftValidity(listItem, nItem)){
		printc("ListItem loaded!\n",1,FOREGROUND_GREEN);
		resource **listResource = loadResources("../resources/", &nResource, listItem, nItem);
		if(listResource != NULL){
			printc("ListResource loaded!\n",1,FOREGROUND_GREEN);
			monster **listMonster = loadMonsters("../monsters/", &nMonster);
			if(listMonster != NULL){
				printc("ListMonster loaded!\n",1,FOREGROUND_GREEN);
				player *player1 = createPlayer(listItem,nItem);
				if(player1 != NULL){
					printc("Player created!\n",1,FOREGROUND_GREEN);
					storage *storage = createStorage();
					if(storage != NULL){
						printc("Storage created!\n",1,FOREGROUND_GREEN);
						char* value = malloc(sizeof(char) * 255);
						printStartMenu();
						scanf("%s", value);
						if(tolower(value[0]) == '1'){
							map = createGame(portal, player1, listItem, nItem, &l);
						} else if(tolower(value[0]) == '2'){
							char *save = selectSave();
							if(save != NULL){
								map = loadSave(save, &respawnList, player1, storage, portal, &l, listItem, nItem,
											   listResource, nResource, listMonster, nMonster);
								free(save);
							}else{
								printf("No save present\n");
							}
						}
						else if(tolower(value[0]) == '3'){
							printCredit();
						}
						else{
							printc("J'ai venu, j'ai lu, j'ai pas comprendu",2,FOREGROUND_RED,FOREGROUND_INTENSITY);
						}
						free(value);
						if(map != NULL){
							inGame(player1,map,storage,listItem,nItem,listResource,nResource,listMonster,nMonster,&respawnList,l, portal);
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