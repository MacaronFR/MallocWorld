#include <game.h>
#include <unistd.h>

void setDir(char *s){
	char *command = malloc(strlen(s) + 1);
	strcpy(command, s);
	char *a = strrchr(command, '/');
	a[1] = 0;
	chdir(command);
	system("pwd");
	free(command);
}

level* startMenu(respawn **respawnList, player *player, storage *storage, int portal[4][2], int *l, item **itemList, size_t nItem, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster) {
	while(1) {
		printStartMenu();
		int value;
		fflush(stdin);
		scanf("%d", &value);
		cleanTerminal();
		if(value == 1){
			return createGame(portal, player, itemList, nItem, l);
		} else if(value == 2){
			char *save = selectSave();
			if(save != NULL){
				level *map = loadSave(save, respawnList, player, storage, portal, l, itemList, nItem,
								resourceList, nResource, monsterList, nMonster);
				free(save);
				return map;
			}else{
				printc("No save present\n",1,FOREGROUND_YELLOW);
			}
		}
		else if(value == 3){
			printCredit();
		}
		else if(value == 0){
			printc("A une prochaine !",2,FOREGROUND_PURPLE,FOREGROUND_INTENSITY);
			return NULL;
		}
		else{
			printc("J'ai venu, j'ai lu, j'ai pas comprendu",1,FOREGROUND_YELLOW);
		}
	}
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
						cleanTerminal();
						map = startMenu(&respawnList, player1, storage, portal, &l, listItem, nItem,listResource, nResource, listMonster, nMonster);
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