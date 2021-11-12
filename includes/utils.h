#ifndef MALLOCWORLD_UTILS_H
#define MALLOCWORLD_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

char *m_fgets(char *s, int size, FILE *stream);
int32_t getFieldValue(char *buf, FILE *stream);
bool checkFieldSyntax(const char *s);

#endif //MALLOCWORLD_UTILS_H
