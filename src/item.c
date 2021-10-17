#include <item.h>

bool checkFieldSyntax(const char *s){
	return s[0] == '{' && s[strlen(s) - 1] == '}';
}

int32_t getFieldValue(char *buf, FILE *stream){
	char *check;
	errno = 0;
	if(m_fgets(buf, 256, stream) == NULL || !checkFieldSyntax(buf)){
#ifdef DEBUG
		fprintf(stderr, "Bad item file syntax : Item field must be inside bracket ({<VALUE>})\n");
#endif
		errno = EINVAL;
		return -1;
	}
	int64_t res = strtoll(buf + 1, &check, 0);
	if(check != buf + strlen(buf) - 1 || res < INT32_MIN || res > INT32_MAX){
#ifdef DEBUG
		fprintf(stderr, "Bad item file syntax : Item field value must be an integer between %d and %d (given '%s'", INT32_MIN, INT32_MAX, buf + 1);
#endif
		errno = ERANGE;
		return -1;
	}
	return res;
}

int32_t *getCraft(char *buf, FILE *stream){
	char *id;
	int l, count = 1;
	int32_t *res;
	if(m_fgets(buf, 256, stream) == NULL || !checkFieldSyntax(buf)){
#ifdef DEBUG
		fprintf(stderr, "Bad item file syntax : Item field must be inside bracket ({<VALUE>})\n");
#endif
		return NULL;
	}
	l = strlen(buf);
	if(l <= 2){
		return NULL;
	}
	for(int i = 1; i < l - 1; i++){
		if(buf[i] == ','){
			count++;
		}
	}
	res = malloc(sizeof(int32_t) * (count + 1));
	id = strtok(buf+1, ",");
	count = 0;
	while(id != NULL){
		res[count] = atoll(id);
		id = strtok(NULL, ",");
		count++;
	}
	res[count] = 0;
	return res;
}

item *load_item(const char *filename){
	char buf[256] = {0};
	int32_t tmp;
	item *res;
	FILE *f = fopen(filename, "r");
	if(f == NULL){
#ifdef DEBUG
		fprintf(stderr, "No Such file or directory\n");
#endif
		return NULL;
	}
	res = malloc(sizeof(item));
	if(res == NULL){
#ifdef DEBUG
		fprintf(stderr, "No memory left\n");
#endif
		fclose(f);
		return NULL;
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "===ITEM===") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad item file syntax : Item file must start ===ITEM===\n");
#endif
		free(res);
		fclose(f);
		return NULL;
	}
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE) || tmp < 0){
		free(res);
		fclose(f);
		return NULL;
	}
	res->type = tmp;
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
	res->flag = tmp;
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	res->durability = tmp;
	res->craft = getCraft(buf, f);
#ifdef DEBUG
	fprintf(stderr, "item {\n\ttype = %d,\n\tid = %d,\n\tflag = %d,\n\tdurability = %d\n", res->type, res->id, res->flag, res->durability);
	int i = 0;
	if(res->craft != NULL){
		fprintf(stderr, "\tcraft : [");
		while(res->craft[i] != 0){
			fprintf(stderr, "%d", res->craft[i]);
			if(res->craft[i + 1] != 0){
				fprintf(stderr, ", ");
			}
			++i;
		}
		fprintf(stderr, "]\n");
	}
	fprintf(stderr, "}\n");
#endif
	fclose(f);
	return res;
}

item **load_items(const char *dir, size_t *n){
	item *tmp;
	item **res = NULL, **tmpRealloc;
	int nItem = 0;
	DIR *item_dir;
	struct dirent *en;
	item_dir = opendir(dir);
	size_t dirlen = strlen(dir);
	char *file = malloc(sizeof(char) * (dirlen + 256));
	strcpy(file, dir);
	if(file[dirlen] - 1 != '/'){
		file[dirlen] = '/';
		file[dirlen + 1] = '\0';
		dirlen++;
	}
	if(item_dir){
		while((en = readdir(item_dir)) != NULL){
			if(en->d_type == DT_REG){
				file[dirlen] = '\0';
				strcat(file, en->d_name);
#ifdef DEBUG
				fprintf(stderr, "looking in file %s\n", file);
#endif
				tmp = load_item(file);
				if(tmp != NULL){
					nItem++;
					tmpRealloc = realloc(res, nItem * sizeof(item*));
					if(tmpRealloc == NULL){
#ifdef DEBUG
						fprintf(stderr, "No more memory available\n");
#endif
						freeItemList(res, nItem - 1);
						free(res);
						free(file);
						free(tmp);
						closedir(item_dir);
						return NULL;
					}
					res = tmpRealloc;
					res[nItem - 1] = tmp;
				}
			}
		}
		closedir(item_dir);
	}
	free(file);
	*n = nItem;
	return res;
}

void freeItemList(item **items, int length){
	for(int i = 0; i < length; ++i)
		free(items[i]);
	free(items);
}