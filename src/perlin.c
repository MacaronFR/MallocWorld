#include <perlin.h>

void freeMap(int ***map, int level, int h){
	for(int i = 0; i < level; ++i){
		for(int j = 0; j < h; ++j){
			free(map[i][j]);
		}
		free(map[i]);
	}
	free(map);
}

int ***generateMap(int seed){
	int ***map = malloc(sizeof(int **) * 3);
	int *p;
	for(int i = 0; i < 3; i++){
		p = generatePerm(seed * ((i + 1)* seed));
		map[i] = generateLevel(100, 100, i + 1, p);
		free(p);
	}
	return map;
}

int **generateLevel(int h, int w, int level, int *p){
	int **map = malloc(sizeof(int*) * h);
	int type;
	for(int i = 0; i < h; ++i){
		map[i] = malloc(sizeof(int) * w);
		for(int j = 0; j < w; ++j){
			map[i][j] = (int)(octaveNoise(i / 100., j / 100., p, 4, .8, 3) * 10);
			if(map[i][j] < 0){
				type = 16;
			}else{
				type = 0;
			}
			map[i][j] = applyRandom(map[i][j], level, type);
		}
	}
	return map;
}

int applyRandom(int n, int level, int type){
	int p;
	if(n < 0) n *= -1;
	switch(n){
		case 5:
		case 4:
		case 3: p = 90;break;
		case 2:  level = (level > 1 ? 2: 1); p = 75;break;
		case 1: level = 1; p = 50;break;
		default: p = 0;break;
	}
	int tmp = rand() % 100;
	if(tmp < p){
		return type + level;
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

double *generateVector(int seed){
    srand48(seed);
    double angle = (drand48() - .5) * 2 * M_PI;
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