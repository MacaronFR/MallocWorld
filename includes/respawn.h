#ifndef MALLOCWORLD_RESPAWN_H
#define MALLOCWORLD_RESPAWN_H

#include <monster.h>
#include <resource.h>

typedef union u_object{
	resource *r;
	monster *m;
} object;

typedef enum e_type{
	RESOURCE,
	MONSTER
} type;

typedef struct s_respawn{
	object ptr;
	size_t left;
	type ptrType;
	struct s_respawn *next;
	uint32_t x;
	uint32_t y;
	int8_t level;
} respawn;

void addMonsterRespawn(monster *m, respawn **list, int32_t x, int32_t y, int8_t level);
void addResourceRespawn(resource *r, respawn **list, int32_t x, int32_t y, int8_t level);
void checkRespawn(respawn **list, int ***map);
void makeRespawn(respawn *r, int ***map);
void freeRespawnList(respawn *r);

#endif //MALLOCWORLD_RESPAWN_H
