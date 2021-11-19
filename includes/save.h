#ifndef MALLOCWORLD_SAVE_H
#define MALLOCWORLD_SAVE_H

#include <respawn.h>

int ***loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *h, int *w);
bool saveGame(const char *fileName, int ***map, respawn **respawnList, player *player, storage *storage);
void getSize(FILE *f, int *h, int *w);
int **loadZone(FILE *f, int zone, char *buf, int bufSize, int x, int y);

#endif //MALLOCWORLD_SAVE_H
