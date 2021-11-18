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
} respawn;

void addMonsterRespawn(monster *m, respawn **list, int32_t x, int32_t y);
void addResourceRespawn(resource *r, respawn **list, int32_t x, int32_t y);
void checkRespawn(respawn **list);
void makeRespawn(respawn *r);
void freeRespawnList(respawn *r);

#endif //MALLOCWORLD_RESPAWN_H
