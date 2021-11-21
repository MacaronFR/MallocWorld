#ifndef MALLOCWORLD_SAVE_H
#define MALLOCWORLD_SAVE_H

#include <respawn.h>
#include <perlin.h>

level *loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *level);
bool saveGame(const char *fileName, level *map, respawn **respawnList, player *player, storage *storage, int level);
void getSize(FILE *f, int *h, int *w);
int **loadZone(FILE *f, int zone, char *buf, int bufSize, int *x, int *y);
int getLevelNumber(FILE *f, char *buf, size_t bufSize);

#endif //MALLOCWORLD_SAVE_H
