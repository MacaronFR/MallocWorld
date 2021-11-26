#include <respawn.h>

void addMonsterRespawn(monster *m, respawn **list, int32_t x, int32_t y, int8_t level){
	respawn *new = malloc(sizeof(respawn));
	new->left = m->respawn;
	new->ptr.m = m;
	new->ptrType = MONSTER;
	new->x = x;
	new->y = y;
	new->next = *list;
	new->level = level;
	*list = new;
}

void addResourceRespawn(resource *r, respawn **list, int32_t x, int32_t y, int8_t level){
	respawn *new = malloc(sizeof(respawn));
	new->left = r->respawn;
	new->ptr.r = r;
	new->ptrType = RESOURCE;
	new->x = x;
	new->y = y;
	new->next = *list;
	new->level = level;
	*list = new;
}

void checkRespawn(respawn **list, level *map){
	respawn *tmp = NULL;
	respawn *prev = NULL;
	respawn *l = *list;
	while(l != NULL){
		l->left--;
		if(l->left <= 0 && makeRespawn(l, map)){
			if(prev == NULL){
				*list = l->next;
			}else{
				prev->next = l->next;
			}
			tmp = l;
			l = l->next;
			free(tmp);
		}else{
			prev = l;
			l = l->next;
		}
	}
}

bool makeRespawn(respawn *r, level *map){
	if(map[r->level].level[r->y][r->x] != 0){
		return false;
	}
	switch(r->ptrType){
		case MONSTER:
			map[r->level].level[r->y][r->x] = r->ptr.m->id;
			printf("Respawn Monster id = %d name = \"%s\" at (%d. %d)\n", r->ptr.m->id, r->ptr.m->name, r->x, r->y);
			return true;
		case RESOURCE:
			map[r->level].level[r->y][r->x] = r->ptr.r->id;
			printf("Respawn Resource id = %d at (%d. %d)\n", r->ptr.r->id, r->x, r->y);
			return true;
	}
}

void freeRespawnList(respawn *r){
	respawn *tmp;
	while(r != NULL){
		tmp = r->next;
		free(r);
		r = tmp;
	}
}