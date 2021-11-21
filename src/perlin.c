#include <perlin.h>

void freeMap(level *map, int level){
	for(int i = 0; i < level; ++i){
		for(int j = 0; j < map[i].h; ++j){
			free(map[i][j]);
		}
		free(map[i].level);
	}
	free(map);
}

level *generateMap(int seed, int portal[4][2]){
	level *map = malloc(sizeof(level) * 3);
	int *p;
	for(int i = 0; i < 3; i++){
		p = generatePerm(seed * ((i + 1)* seed));
		map[i].level = generateLevel(100, 100, i + 1, p, seed, 5 * (i+1), portal);
		map[i].h = 100;
		map[i].w = 100;
		free(p);
	}
	return map;
}

int **generateLevel(int h, int w, int level, int *p, int seed, int maxMonstre, int portal[4][2]){
	int **map = malloc(sizeof(int*) * h);
	int x, y;
	for(int i = 0; i < h; ++i){
		map[i] = malloc(sizeof(int) * w);
		for(int j = 0; j < w; ++j){
			map[i][j] = (int)(octaveNoise(i / 100., j / 100., p, 4, .45, 4) * 10);
		}
	}
	srand(seed);
	for(int i = 0; i < h; ++i){
		for(int j = 0; j < w; ++j){
			map[i][j] = applyRandom(map[i][j], level, maxMonstre);
		}
	}
	do{
		x = (rand() % 98) + 1;
		y = (rand() % 98) + 1;
	}while(!checkZone(map, x, y, 1));
	map[x][y] = (level == 3) ? -3 : -2;
	map[x + 1][y - 1] = 2;
	portal[level - 1][0] = x;
	portal[level - 1][1] = y;
	if(level == 2){
		do{
			x = (rand() % 98) + 1;
			y = (rand() % 98) + 1;
		}while(!checkZone(map, x, y, 1));
		map[x][y] = -2;
		portal[3][0] = x;
		portal[3][1] = y;
	}
	return map;
}

bool checkZone(int **map, int x, int y, int length){
	for(int i = -length; i < length; ++i){
		for(int j = -length; j < length; ++j){
			if(map[x + i][y + j] != 0){
				return false;
			}
		}
	}
	return true;
}

int applyRandom(int n, int level, int maxMonstre){
	int p, v;
	if(rand()%100 < 3){
		return -1;
	}
	switch(n){
		case 9:
		case 8:
		case 7:
		case 6: p = 90; v = (level == 3) ?  11 : (level == 2) ? 8 : 5;break;
		case 5: p = 75; v = (level >= 2) ? 8 : 5;break;
		case 4: p = 50; v = 5;break;
		case 3: p = 60; v = (level == 3) ?  9 : (level == 2) ? 6 : 3;break;
		case 2: p = 50; v = (level >= 2) ? 6 : 3;break;
		case 1: p = 40; v = 3;break;
		case 0: p = 90; v = 0;break;
		case -1: p = 50; v = 4;break;
		case -2: p = 75; v = (level >= 2) ? 7 : 4;break;
		default: p = 90; v = (level == 3) ?  10 : (level == 2) ? 7 : 4;break;
	}
	int tmp = rand() % 100;
	if(tmp < p){
		return v;
	}else{
		tmp = rand() % 100;
		if(tmp < 30){
			return (rand() % maxMonstre) + 12;
		}
	}
	return 0;
}

double octaveNoise(double x, double y, int *p, int octaves, double persistence, double frequency){
	double r = 0;
	double amplitude = 1;
	double maxValue = 0;
	for(int i = 0; i < octaves; ++i){
		double tmp = noise(x * frequency, y * frequency, p) * amplitude;
		r += tmp;
		frequency *= 2;
		amplitude *= persistence;
		maxValue += amplitude * .9;
	}
	return r /maxValue;
}

double noise (double x, double y, int *p){
	int X = ((int)x) & 255;
	int Y = ((int)y) & 255;
	x -= (int)x;
	y -= (int)y;
	double u = fade(x);
	double v = fade (y);
	int A = p [ X ]+ Y;
	int B = p [ X +1]+ Y;
	return lerp(
            v,
            lerp(u, grad(p[A & 255], x, y),grad(p[B & 255], x - 1, y)),
            lerp(u,grad(p[(A + 1)  & 255], x, y - 1),grad(p[(B + 1)  & 255], x - 1, y - 1))
            );
}
double fade ( double t ){
	return t * t * t *( t *( t *6 - 15) + 10);
}

double lerp ( double t , double a, double b){
	return a + t * ( b - a );
}

double produit_scalaire(double *vec, double x, double y){
	double res =  vec[0] * x + vec[1] * y;
	free(vec);
	return res;
}


double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double *generateVector(int seed){
	srand(seed);
	double angle = fRand(-1, 1) * M_PI;
	double *vec = malloc(sizeof(double) * 2);
	if(vec == NULL){
		return NULL;
	}
	vec[0] = cos(angle);
	vec[1] = sin(angle);
	return vec;
}

double grad(int hash, double x, double y){
	return produit_scalaire(generateVector(hash), x, y);
}

int *generatePerm(int seed){
	srand(seed);
	int *perm = malloc(sizeof(int) * 256);
	if(perm == NULL){
		return NULL;
	}
	int randI;
	for(int i = 0; i < 256; ++i){
		perm[i] = 0;
	}
	for(int i = 1; i < 256; ++i){
		do{
			randI = rand() % 256;
		}while(perm[randI] != 0);
		perm[randI] = i;
	}
	return perm;
}