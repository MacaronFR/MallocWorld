#include <save.h>

int ***loadSave(const char *fileName, respawn **respawnList, player *player, storage *storage, int portal[4][2], int *h, int *w){
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
	int ***map = malloc(sizeof(int **) * 1);
	getSize(f, h, w);
	map[0] = loadZone(f, 1, buf, 256, *w, *h);
	return map;
}

void getSize(FILE *f, int *w, int *h){
	long start = ftell(f);
	long cur = 0;
	bool prev = false;
	*h = 0;
	*w = 0;
	while(cur != '\n'){
		cur = fgetc(f);
	}
	cur = 0;
	while(cur != '\n'){
		cur = fgetc(f);
		if(cur == ' '){
			(*w)++;
		}
	}
	(*w)++;
	while(cur != '-' || !prev){
		if(cur == '\n'){
			(*h)++;
		}
		if(cur != '-' && prev){
			prev = false;
		}
		if(cur == '-' && !prev){
			prev = true;
		}
		cur = fgetc(f);
	}
	fseek(f, start, SEEK_SET);
}

int **loadZone(FILE *f, int zone, char *buf, int bufSize, int x, int y){
	int k;
	char zoneName[14];
	int **level;
	sprintf(zoneName, "-- ZONE %d --", zone);
	if(m_fgets(buf, bufSize, f) == NULL || strcmp(buf, zoneName) != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad map file syntax : first zone must start with %s\n", zoneName);
#endif
		return NULL;
	}
	level = malloc(sizeof(int*) * y);
	for(int i = 0; i < y; ++i){
		level[i] = malloc(sizeof(int) * x);
		for(int j = 0; j < x; ++j){
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