#include <utils.h>

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

char *m_fgets(char *s, int size, FILE *stream){
	if(fgets(s, size, stream) == NULL){
		return NULL;
	}
	int l = strlen(s);
	if(s[l - 1] == '\n'){
		s[l-1] = 0;
	}
	if(s[l-2] == '\r'){
		s[l-2] = 0;
	}
	return s;
}