#ifndef MALLOCWORLD_PERLIN_H
#define MALLOCWORLD_PERLIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

double fade(double);
double lerp(double, double, double);
double grad(int, double, double);
double noise (double x, double y, int *p);
double octaveNoise(double x, double y, int *p, int octaves, double persistence, double frequency);
int *generatePerm(int seed);
int **generateLevel(int h, int w, int level, int *p, int seed);
int applyRandom(int n, int level, int type);
int ***generateMap(int seed);
void freeMap(int ***map, int level, int h);

#endif //MALLOCWORLD_PERLIN_H
