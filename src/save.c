#include <save.h>
#include <perlin.h>

level *loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *l){
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
		map[i].level = loadZone(f, i + 1, buf, 256, &(map[i].w), &(map[i].h));
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "=== PLAYER ===") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad map file syntax : Map file must have === PLAYER ===\n");
#endif
		fclose(f);
		freeMap(map, *l);
		*l = 0;
		return NULL;
	}
	return map;
}

void getSize(FILE *f, int *w, int *h){
	long start = ftell(f);
	long cur = 0;
	long prev = 0;
	*h = 0;
	*w = 0;
	/*while(cur != '\n'){
		cur = fgetc(f);
	}*/
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

int **loadZone(FILE *f, int zone, char *buf, int bufSize, int *w, int *h){
	int k;
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
	level = malloc(sizeof(int*) * (*h));
	for(int i = 0; i < *h; ++i){
		level[i] = malloc(sizeof(int) * (*w));
		for(int j = 0; j < *w; ++j){
			k = 0;
			buf[k] = fgetc(f);
			while(buf[k] != ' ' && buf[k] != '\n'){
				k++;
				buf[k] = fgetc(f);
			}
			buf[k] = '\0';
			level[i][j] = atoi(buf);
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