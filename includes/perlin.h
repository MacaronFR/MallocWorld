#ifndef MALLOCWORLD_PERLIN_H
#define MALLOCWORLD_PERLIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_level{
	int32_t h, w;
	int **level;
} level;

double fade(double);
double lerp(double, double, double);
double grad(int, double, double);
double noise (double x, double y, int *p);
double octaveNoise(double x, double y, int *p, int octaves, double persistence, double frequency);
int *generatePerm(int seed);
int **generateLevel(int h, int w, int level, int *p, int seed, int maxMonstre, int portal[4][2]);
int applyRandom(int n, int level, int max_monstre);
level *generateMap(int seed, int portal[4][2]);
void freeMap(level *map, int level);
bool checkZone(int **map, int x, int y, int length);

#endif //MALLOCWORLD_PERLIN_H
