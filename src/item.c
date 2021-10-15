#include <item.h>

bool checkFieldSyntax(const char *s){
	size_t l = strlen(s);
	return s[0] == '{' && s[l - 1] == '}';
}

int32_t getFieldValue(char *buf, FILE *stream){
	char *check;
	errno = 0;
	if(m_fgets(buf, 256, stream) == NULL || !checkFieldSyntax(buf)){
		fprintf(stderr, "Bad item file syntax : Item field must be inside bracket ({<VALUE>})\n");
		errno = EINVAL;
		return -1;
	}
	int64_t res = strtoll(buf + 1, &check, 0);
	if(check != buf + strlen(buf) - 1 || res < INT32_MIN || res > INT32_MAX){
		fprintf(stderr, "Bad item file syntax : Item field value must be an integer between %d and %d (given '%s'", INT32_MIN, INT32_MAX, buf + 1);
		errno = ERANGE;
		return -1;
	}
	return res;
}

item *loadItem(const char *file){
	char buf[256] = {0};
	int32_t tmp;
	item *res;
	FILE *f = fopen(file, "r");
	if(f == NULL){
		fprintf(stderr, "No Such file or directory\n");
		return NULL;
	}
	res = malloc(sizeof(item));
	if(res == NULL){
		fprintf(stderr, "No memory left\n");
		fclose(f);
		return NULL;
	}
	if(m_fgets(buf, 256, f) == NULL || strcmp(buf, "===ITEM===") != 0){
		fprintf(stderr, "Bad item file syntax : Item file must start ===ITEM===\n");
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
	printf("item {\n\ttype = %d,\n\tid = %d,\n\tflag = %d,\n\tdurability = %d\n}\n", res->type, res->id, res->flag, res->durability);
	fclose(f);
	return res;
}