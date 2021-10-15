#include <utils.h>

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