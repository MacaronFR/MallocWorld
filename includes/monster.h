#ifndef MALLOCWORLD_MONSTER_H
#define MALLOCWORLD_MONSTER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utils.h>

typedef struct s_monster{
	uint16_t life;
	uint8_t respawn;
	uint16_t strength;
	char *name;
	u_int32_t id;
} monster;

monster *createMonster(monster *m);
void deleteMonster(monster *m);
monster *loadMonster(const char *filename);

#endif //MALLOCWORLD_MONSTER_H
