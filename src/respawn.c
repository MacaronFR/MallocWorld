#include <respawn.h>

void addMonsterRespawn(monster *m, respawn **list, int32_t x, int32_t y){
	respawn *new = malloc(sizeof(respawn));
	new->left = m->respawn;
	new->ptr.m = m;
	new->ptrType = MONSTER;
	new->x = x;
	new->y = y;
	new->next = *list;
	*list = new;
}

void addResourceRespawn(resource *r, respawn **list, int32_t x, int32_t y){
	respawn *new = malloc(sizeof(respawn));
	new->left = r->respawn;
	new->ptr.r = r;
	new->ptrType = RESOURCE;
	new->x = x;
	new->y = y;
	new->next = *list;
	*list = new;
}

void checkRespawn(respawn **list){
	respawn *tmp = NULL;
	respawn *prev = NULL;
	respawn *l = *list;
	while(l != NULL){
		l->left--;
		if(l->left <= 0){
			makeRespawn(l);
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

void makeRespawn(respawn *r){
	switch(r->ptrType){
		case MONSTER:
			printf("Respawn Monster id = %d name = \"%s\" at (%d. %d)\n", r->ptr.m->id, r->ptr.m->name, r->x, r->y);
			return;
		case RESOURCE:
			printf("Respawn Resource id = %d at (%d. %d)\n", r->ptr.r->id, r->x, r->y);
			return;
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