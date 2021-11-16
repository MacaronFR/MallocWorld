#include <monster.h>

monster *createMonster(monster *m){
	monster *res = malloc(sizeof(monster));
	res->id = m->id;
	res->name = malloc(sizeof(char) * strlen(m->name) + 1);
	strcpy(res->name, m->name);
	res->life = m->life;
	res->respawn = m->respawn;
	res->strength = m->strength;
	return res;
}

void deleteMonster(monster *m){
	free(m->name);
	free(m);
}

monster *loadMonster(const char *fn){
	char buf[256] = {0};
	monster *res;
	FILE *f = fopen(fn, "r");
	int32_t tmp;
	if(f == NULL){
#ifdef DEBUG
		fprintf(stderr, "No such file or directory\n");
#endif
		return NULL;
	}
	res = malloc(sizeof(monster));
	if(res == NULL){
#ifdef DEBUG
		fprintf(stderr, "Out of memory\n");
#endif
		fclose(f);
		return NULL;
	}
	if(fgets(buf, 255, f) == NULL || strncmp(buf, "===MONSTER===", 13) != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad file syntax : Must start with \"===MONSTER===\"\n");
#endif
		free(res);
		fclose(f);
		return NULL;
	}
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->id = tmp;
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->life = tmp;
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->respawn = tmp;
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->strength = tmp;
	if(m_fgets(buf, 256, f) == NULL || !checkFieldSyntax(buf)){
#ifdef DEBUG
		fprintf(stderr, "Name required");
#endif
		free(res);
		fclose(f);
		return NULL;
	}
	res->name = malloc(sizeof(char) * strlen(buf) - 1);
	strncpy(res->name, buf + 1, strlen(buf) - 2);
	res->name[strlen(buf) - 2] = 0;
#ifdef VERBOSE
	fprintf(stderr, "monster {\n\tid = %d,\n\tlife = %d,\n\trespawn = %d\n\tstrength = %d\n\tname = %s\n}\n", res->id, res->life, res->respawn, res->strength, res->name);
#endif
	return res;
}