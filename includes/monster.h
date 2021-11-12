//
// Created by basil on 26/10/2021.
//

#ifndef MALLOCWORLD_MONSTER_H
#define MALLOCWORLD_MONSTER_H

typedef struct s_monster {
    int id;
    int life;
    int damage;
    int deadTime;
} monster;

monster* generateMonster();
void monsterTakeDamage(monster *monster, int amount);
void monsterAttack(monster *monster, player *player);
void monsterIncrementTime(monster *monster);

#endif //MALLOCWORLD_MONSTER_H
