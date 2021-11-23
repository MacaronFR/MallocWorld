#ifndef MALLOCWORLD_SAVE_H
#define MALLOCWORLD_SAVE_H

#include <respawn.h>
#include <perlin.h>
#include <player.h>

typedef struct s_saveFile{
	char *name;
	unsigned int index;
	struct s_saveFile *next;
} saveFile;

level *loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *level, item **itemList, int nItem, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster);
bool saveGame(const char *fileName, level *map, respawn *respawnList, player *player, storage *storage, int level);
bool writeMap(level *map, int level, FILE *f);
bool writePlayer(player *p, FILE *f);
bool writeStorage(storage *s, FILE *f);
bool writeRespawn(respawn *r, FILE *f);
void getSize(FILE *f, int *h, int *w);
int **loadZone(FILE *f, int zone, char *buf, int bufSize, int *x, int *y, int portal[4][2]);
int getLevelNumber(FILE *f, char *buf, size_t bufSize);
bool loadPlayer(FILE *f, char *buf, size_t bufSize, player *p1, item **itemList, int nItem);
bool loadInventory(inventory *inv, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem);
bool loadStorage(storage *s, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem);
bool loadRespawn(respawn **r, char *buf, size_t bufSize, FILE *f, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster);
char *selectSave();

#endif //MALLOCWORLD_SAVE_H
