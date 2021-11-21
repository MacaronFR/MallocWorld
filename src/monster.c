#include <monster.h>

monster *createMonster(monster *m){
	monster *res = malloc(sizeof(monster));
	res->id = m->id;
	res->name = malloc(sizeof(char) * strlen(m->name) + 1);
	strcpy(res->name, m->name);
	res->life = m->life;
	res->respawn = m->respawn;
	res->strength = m->strength;
	res->zone = m->zone;
	return res;
}

void freeMonster(monster *m){
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
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->zone = tmp;
#ifdef VERBOSE
	fprintf(stderr, "monster {\n\tid = %d,\n\tlife = %d,\n\trespawn = %d\n\tstrength = %d\n\tname = %s\n\tzone = %d\n}\n", res->id, res->life, res->respawn, res->strength, res->name, res->zone);
#endif
	return res;
}

monster **loadMonsters(const char *dir, size_t *n){
	monster *tmp;
	monster **res = NULL, **tmpRealloc;
	size_t nResource = 0;
	DIR *monsterDir;
	struct dirent *en;
	monsterDir = opendir(dir);
	size_t dirlen = strlen(dir);
	char *file = malloc(sizeof(char) * (dirlen + 256));
	strcpy(file, dir);
	if(file[dirlen - 1] != '/'){
		file[dirlen] = '/';
		file[dirlen + 1] = '\0';
		dirlen++;
	}
	if(monsterDir){
		while((en = readdir(monsterDir)) != NULL){
			if(en->d_type == DT_REG){
				file[dirlen] = '\0';
				strcat(file, en->d_name);
#ifdef DEBUG
				fprintf(stderr, "looking in file %s\n", file);
#endif
				tmp = loadMonster(file);
				if(tmp != NULL){
					nResource++;
					tmpRealloc = realloc(res, nResource * sizeof(monster *));
					if(tmpRealloc == NULL){
#ifdef DEBUG
						fprintf(stderr, "No more memory available\n");
#endif
						freeMonsterList(res, nResource - 1);
						free(res);
						free(file);
						free(tmp);
						closedir(monsterDir);
						return NULL;
					}
					res = tmpRealloc;
					res[nResource - 1] = tmp;
				}
			}
		}
		closedir(monsterDir);
	}
	free(file);
	*n = nResource;
	return res;
}

void freeMonsterList(monster **m, size_t n){
	for(size_t i = 0; i < n; ++i){
		freeMonster(m[i]);
	}
	free(m);
}

monster *findMonster(monster **monsterList, size_t nMonster, int32_t id){
	for(int i = 0; i < nMonster; ++i){
		if(monsterList[i]->id == id){
			return monsterList[i];
		}
	}
	return NULL;
}

//TODO à m'explquer
monster *checkMonsterId(monster **listMonster, int id) {
	monster *pointeur = listMonster[0];
	while(pointeur != NULL) {
		if(pointeur->id == id)
			return createMonster(pointeur);
	}
	return NULL;
}