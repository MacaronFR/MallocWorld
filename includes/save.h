#ifndef MALLOCWORLD_SAVE_H
#define MALLOCWORLD_SAVE_H

#include <respawn.h>
#include <perlin.h>

level *loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *l, item **itemList, int nItem);
bool saveGame(const char *fileName, level *map, respawn **respawnList, player *player, storage *storage, int level);
void getSize(FILE *f, int *h, int *w);
int **loadZone(FILE *f, int zone, char *buf, int bufSize, int *x, int *y);
int getLevelNumber(FILE *f, char *buf, size_t bufSize);
bool loadPlayer(FILE *f, char *buf, size_t bufSize, player *p1, item **itemList, int nItem);
bool loadInventory(inventory *inv, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem);
bool loadStorage(storage *s, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem);
bool loadRespawn(respawn **r, char *buf, size_t bufSize, FILE *f, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster);

#endif //MALLOCWORLD_SAVE_H
