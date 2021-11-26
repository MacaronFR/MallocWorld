#include <save.h>
#include <perlin.h>

level *loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *l, item **itemList, size_t nItem, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster){
	char buf[256];
	FILE *f = fopen(fileName, "r");
	if(f == NULL){
#ifdef DEBUG
		fprintf(stderr, "No such file or directory");
#endif
		return NULL;
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "=== MAP ===") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad map file syntax : Map file must start === MAP ===\n");
#endif
		fclose(f);
		return NULL;
	}
	*l = getLevelNumber(f, buf, 256);
	level *map = malloc(sizeof(level) * (*l));
	for(int i = 0; i < *l; ++i){
		map[i].level = loadZone(f, i + 1, buf, 256, &(map[i].w), &(map[i].h), portal, player);
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "=== PLAYER ===") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad save file syntax : Map file must have === PLAYER ===\n");
#endif
		fclose(f);
		freeMap(map, *l);
		*l = 0;
		return NULL;
	}
	if(!loadPlayer(f, buf, 256, player, itemList, nItem)){
		fclose(f);
		freeMap(map, *l);
		*l=0;
		return NULL;
	}
	if(!loadStorage(storage, buf, 256, f, itemList, nItem)){
		fclose(f);
		freePlayer(player);
		freeMap(map, *l);
		*l=0;
		return NULL;
	}
	loadRespawn(respawnList, buf, 256, f, resourceList, nResource, monsterList, nMonster);
	return map;
}

void getSize(FILE *f, int *w, int *h){
	long start = ftell(f);
	long cur = 0;
	long prev = 0;
	*h = 0;
	*w = 0;
	cur = 0;
	while((cur != '\n' && cur != '=') && (cur != '-' || prev != '-')){
		prev = cur;
		cur = fgetc(f);
		if(cur == ' '){
			(*w)++;
		}
	}
	if(*w == 0 && (cur == '-' || cur == '=')){
		fseek(f, start, SEEK_SET);
		return;
	}
	(*w)++;
	while(cur != '=' && (cur != '-' || prev != '-')){
		if(cur == '\n'){
			(*h)++;
		}
		prev = cur;
		cur = fgetc(f);
	}
	fseek(f, start, SEEK_SET);
}

int **loadZone(FILE *f, int zone, char *buf, int bufSize, int *w, int *h, int portal[4][2], player *p1){
	char zoneName[20];
	int **level;
	sprintf(zoneName, "-- ZONE %d --", zone);
	if(m_fgets(buf, bufSize, f) == NULL || strcmp(buf, zoneName) != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad map file syntax : zone must start with %s\n", zoneName);
#endif
		return NULL;
	}
	getSize(f, w, h);
	level = loadZoneArray(zone, *h, *w, portal, p1, f);
	return level;
}

int **loadZoneArray(int zone, int h, int w, int portal[4][2], player *p1, FILE *f){
	char buf[6];
	int k;
	int **level = malloc(sizeof(int*) * (h));
	for(int i = 0; i < h; ++i){
		level[i] = malloc(sizeof(int) * (w));
		for(int j = 0; j < w; ++j){
			k = 0;
			buf[k] = fgetc(f);
			while(buf[k] != ' ' && buf[k] != '\n'){
				k++;
				buf[k] = fgetc(f);
			}
			buf[k] = '\0';
			level[i][j] = atoi(buf);
			if(level[i][j] == -2 || level[i][j] == -3){
				if(zone == 2 && level[i][j] == -3){
					portal[3][1] = i;
					portal[3][0] = j;
				}else{
					portal[zone - 1][1] = i;
					portal[zone - 1][0] = j;
				}
			}
			if(level[i][j] == 1){
				p1->abs_coord.x = j;
				p1->abs_coord.y = i;
				p1->abs_coord.zone = zone - 1;
			}
		}
	}
	return level;
}

int getLevelNumber(FILE *f, char *buf, size_t bufSize){
	char zoneName[20];
	long start = ftell(f);
	int res = 0;
	sprintf(zoneName, "-- ZONE %d --", res + 1);
	do{
		m_fgets(buf, bufSize, f);
		if(strcmp(zoneName, buf) == 0){
			res++;
			sprintf(zoneName, "-- ZONE %d --", res + 1);
		}
	}while(strcmp(buf, "=== PLAYER ===") != 0);
	fseek(f, start, SEEK_SET);
	return res;
}

bool loadPlayer(FILE *f, char *buf, size_t bufSize, player *p1, item **itemList, int nItem){
	int32_t val1, val2, val3;
	val1 = getFieldValue(buf, f);
	if(val1 == -1 && (errno == EINVAL || errno == ERANGE)){
		fclose(f);
		return false;
	}
	p1->level = val1;
	m_fgets(buf, bufSize, f);
	val3 = sscanf(buf, "{%u}/{%u}", &val1, &val2);
	if(val3 != 2){
#ifdef DEBUG
		fprintf(stderr, "Bad Syntax, Player XP must be {XP_CURRENT}/{XP_NEXT}\n");
#endif
		fclose(f);
		return false;
	}
	p1->exp = val1;
	m_fgets(buf, bufSize, f);
	val3 = sscanf(buf, "{%u}/{%u}", &val1, &val2);
	if(val3 != 2){
#ifdef DEBUG
		fprintf(stderr, "Bad Syntax, Player HP must be {HP_CURRENT}/{HP_MAX}\n");
#endif
		return false;
	}
	p1->life = val1;
	p1->maxLife = val2;
	m_fgets(buf, bufSize, f);
	if(strcmp(buf, "-- INVENTORY --") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad Syntax, Save file Must Have '-- INVENTORY --' section\n");
#endif
		return false;
	}
	if(!loadInventory(p1->inventory, buf, bufSize, f, itemList, nItem)){
		return false;
	}
	return true;
}

bool loadInventory(inventory *inv, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem){
	int val1, val2, val3, val4;
	item *tmp, *newItem;
	for(int i = 0; i < MAX_SLOTS_INVENTORY; ++i){
		inv->slots[i].item = NULL;
		m_fgets(buf, bufSize, f);
		val4 = sscanf(buf, "{%d}@{%d}@{%d}", &val1, &val2, &val3);
		if(val4 != 3){
#ifdef DEBUG
			fprintf(stderr, "Bad Syntax : Inventory item must be {QTY}@{ID}@{DURABILITY}");
#endif
			for(int j = 0; j < i; ++j)
				freeInventorySlot(&(inv->slots[j]));
			return false;
		}
		tmp = getItem(itemList, nItem, val2);
		if(tmp == NULL && val2 != 0){
			for(int j = 0; j < i; ++j)
				freeInventorySlot(&(inv->slots[j]));
			return false;
		}
		for(int j = 0; j < val1; ++j){
			newItem = copyItem(tmp);
			if((newItem->type & TOOLS) != 0)
				newItem->durability = val3;
			addItemInInventory(inv, newItem);
		}
	}
	return true;
}

bool loadStorage(storage *s, char *buf, size_t bufSize, FILE *f, item **itemList, size_t nItem){
	int val1, val2, val3;
	item *tmp;
	if(m_fgets(buf, bufSize, f) == NULL || strcmp(buf, "-- STORAGE --") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad Syntax : Save file must have '-- STORAGE -- section");
#endif
		return false;
	}
	while(m_fgets(buf, bufSize, f) != NULL && strcmp(buf, "=== RESPAWN ===")){
		val3 = sscanf(buf, "{%d}@{%d}", &val1, &val2);
		if(val3 != 2){
#ifdef DEBUG
			fprintf(stderr, "Bad Syntax : Inventory item must be {QTY}@{ID}@{DURABILITY}");
#endif
			return false;
		}
		tmp = getItem(itemList, nItem, val2);
		if(tmp == NULL){
#ifdef DEBUG
			fprintf(stderr, "Unknown item (id : %d)", val2);
#endif
			return false;
		}
		for(int i = 0; i < val1; ++i){
			addItemInStorage(s, copyItem(tmp));
		}
	}
	return true;
}

bool loadRespawn(respawn **r, char *buf, size_t bufSize, FILE *f, resource **resourceList, size_t nResource, monster **monsterList, size_t nMonster){
	int val1, val2, val3, val4, val5, count;
	char type;
	resource *res;
	monster *m;
	while(m_fgets(buf, bufSize, f) != NULL){
		count = sscanf(buf, "{%c}@{%d}@{%d}@{%d}@{%d}@{%d}", &type, &val1, &val2, &val3, &val4, &val5);
		if(count != 6){
			freeRespawnList(*r);
			return false;
		}
		if(type == 'R'){
			res = findResource(resourceList, nResource, val1);
			if(res == NULL){
#ifdef DEBUG
				fprintf(stderr, "Unknown resource (id : %d)", val1);
#endif
				freeRespawnList(*r);
				return false;
			}
			addResourceRespawn(res, r, val3, val4, val5);
			r[0]->left = val2;
		}else if(type == 'M'){
			m = findMonster(monsterList, nMonster, val1);
			if(m == NULL){
#ifdef DEBUG
				fprintf(stderr, "Unknown resource (id : %d)", val1);
#endif
				freeRespawnList(*r);
				return false;
			}
			addMonsterRespawn(m, r, val3, val4, val5);
			r[0]->left = val2;
		}
	}
	return true;
}

bool saveGame(const char *fileName, level *map, respawn *respawnList, player *player, storage *storage, int level){
	FILE *f = fopen(fileName, "w");
	if(f == NULL){
		return false;
	}
	writeMap(map, level, f);
	writePlayer(player, f);
	writeStorage(storage, f);
	writeRespawn(respawnList, f);
	return true;
}

bool writeMap(level *map, int level, FILE *f){
	fprintf(f, "=== MAP ===\n");
	for(int i = 0; i < level; ++i){
		fprintf(f, "-- ZONE %d --\n", i+1);
		for(int j = 0; j < map[i].h; ++j){
			for(int k = 0; k < map[i].w; ++k){
				fprintf(f, "%d", map[i].level[j][k]);
				if(k != map[i].w - 1){
					fprintf(f, " ");
				}
			}
			fprintf(f, "\n");
		}
	}
}

bool writePlayer(player *p, FILE *f){
	fprintf(f, "=== PLAYER ===\n{%d}\n{%d}/{%d}\n{%d}/{%d}\n-- INVENTORY --\n", p->level, p->exp, 100 * p->level, p->life, p->maxLife);
	for(int i = 0; i < MAX_SLOTS_INVENTORY; ++i){
		if(p->inventory->slots[i].item != NULL){
			fprintf(f,"{%d}@{%d}@{%hhd}\n", p->inventory->slots[i].quantity, p->inventory->slots[i].item->id, p->inventory->slots[i].item->durability);
		}else{
			fprintf(f, "{0}@{0}@{0}\n");
		}
	}
}

bool writeStorage(storage *s, FILE *f){
	fprintf(f, "-- STORAGE --\n");
	for(int i = 0; i < s->size; ++i){
		fprintf(f, "{%d}@{%d}\n", s->slots[i].quantity, s->slots[i].item->id);
	}
}

bool writeRespawn(respawn *r, FILE *f){
	char type;
	uint32_t id;
	fprintf(f, "=== RESPAWN ===");
	while(r != NULL){
		if(r->ptrType == MONSTER){
			type = 'M';
			id = r->ptr.m->id;
		}else if(r->ptrType == RESOURCE){
			type = 'R';
			id = r->ptr.r->id;
		}
		fprintf(f, "\n{%c}@{%d}@{%zu}@{%d}@{%d}@{%d}",type, id, r->left, r->x, r->y, r->level);
		r = r->next;
	}
}

char *selectSave(){
	saveFile *list = NULL;
	saveFile *tmp;
	char *res;
	int i = 0, choice = -1;
	DIR *item_dir;
	struct dirent *en;
	item_dir = opendir("./saves/");
	if(!item_dir){
		//TODO le dossier save est casser
	}
	while((en = readdir(item_dir)) != NULL){
		if(en->d_type == DT_REG){
			tmp = list;
			list = malloc(sizeof(saveFile));
			list->next = tmp;
			list->name = malloc(sizeof(en->d_name));
			strcpy(list->name, en->d_name);
			list->index = i;
			++i;
		}
	}
	closedir(item_dir);
	tmp = list;
	while(tmp != NULL){
		printf("%d - %s\n", tmp->index, tmp->name);
		tmp = tmp->next;
	}
	while(choice < 0 || choice >= i){
		printf("quel sauvegarde ?\n");
		fflush(stdin);
		scanf("%d", &choice);
	}
	while(list != NULL){
		if(list->index != choice){
			free(list->name);
		}else{
			res = list->name;
		}
		tmp = list->next;
		free(list);
		list = tmp;
	}
	return res;
}