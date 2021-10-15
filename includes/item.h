#ifndef MALLOCWORLD_ITEM_H
#define MALLOCWORLD_ITEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct s_item{
    __int8_t durability; //durabilité ou cout en durabilité
    __uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    __int32_t id; //id objet
    __int32_t *craft; // si item son craft en tableau d'id nécessaire
    __uint8_t flag; // zone
} item;


bool isTool(item* item);
bool isWeapon(item* item);
bool isArmor(item* item);
bool isRessource(item* item);
bool canStack(item* item);

void useDurability(item* item);
void repareItem(item* item);



#endif //MALLOCWORLD_ITEM_H
