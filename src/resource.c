#include <resource.h>

resource *loadResource(const char *filename, item **itemList, size_t nItem){
	char buf[256] = {0};
	int32_t tmp;
	resource *res;
	FILE *f = fopen(filename, "r");
	if(f == NULL){
#ifdef DEBUG
		fprintf(stderr, "No Such file or directory\n");
#endif
		return NULL;
	}
	res = malloc(sizeof(resource));
	if(res == NULL){
#ifdef DEBUG
		fprintf(stderr, "No memory left\n");
#endif
		fclose(f);
		return NULL;
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "===RESOURCE===") != 0){
#ifdef DEBUG
		fprintf(stderr, "Bad item file syntax : Item file must start ===RESOURCE===\n");
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
	res->respawn = tmp;
	tmp = getFieldValue(buf, f);
	if(tmp == -1 && (errno == EINVAL || errno == ERANGE)){
		free(res);
		fclose(f);
		return NULL;
	}
	for(size_t i = 0; i < nItem; ++i){
		if(itemList[i]->id == tmp){
			res->item = itemList[i];
			fclose(f);
#ifdef VERBOSE
			fprintf(stderr, "resource {\n\tid = %d,\n\tflag = %d,\n\trespawn = %d\n\titem = %p\n}\n", res->id, res->flag, res->respawn, res->item);
#endif
			return res;
		}
	}
	free(res);
	fclose(f);
	return NULL;
}

resource **loadResources(const char *dir, size_t *n, item **itemList, size_t nItem){
	resource *tmp;
	resource **res = NULL, **tmpRealloc;
	int nResource = 0;
	DIR *item_dir;
	struct dirent *en;
	item_dir = opendir(dir);
	size_t dirlen = strlen(dir);
	char *file = malloc(sizeof(char) * (dirlen + 256));
	strcpy(file, dir);
	if(file[dirlen - 1] != '/'){
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
				tmp = loadResource(file, itemList, nItem);
				if(tmp != NULL){
					nResource++;
					tmpRealloc = realloc(res, nResource * sizeof(resource *));
					if(tmpRealloc == NULL){
#ifdef DEBUG
						fprintf(stderr, "No more memory available\n");
#endif
						freeResourceList(res, nResource - 1);
						free(res);
						free(file);
						free(tmp);
						closedir(item_dir);
						return NULL;
					}
					res = tmpRealloc;
					res[nResource - 1] = tmp;
				}
			}
		}
		closedir(item_dir);
	}
	free(file);
	*n = nResource;
	return res;
}

void freeResourceList(resource **r, size_t n){
	for(size_t i = 0; i < n; ++i){
		free(r[i]);
	}
	free(r);
}

resource *findResource(resource **r, size_t nResource, int32_t id){
for(int i = 0; i < nResource; ++i){
		if(r[i]->id == id){
			return r[i];
		}
	}
	return NULL;
}

