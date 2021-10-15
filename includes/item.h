#ifndef MALLOCWORLD_ITEM_H
#define MALLOCWORLD_ITEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <utils.h>


typedef struct s_item{
    int8_t durability; //durabilité ou cout en durabilité
    uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    int32_t id; //id objet
    int32_t *craft; // si item son craft en tableau d'id nécessaire
    uint8_t flag; // zone
} item;

item *loadItem(const char *filename);
bool isSword(item* item);
bool isPickaxe(item* item);
bool isHoe(item* item);
bool isAxe(item* item);
bool isWood(item* item);
bool isOre(item* item);
bool isPlante(item* item);
bool isSpear(item* item);
bool isHammer(item* item);
bool isChestplate(item* item);
bool isPotion(item* item);


void useDurability(item* item);
void repareItem(item* item);



#endif //MALLOCWORLD_ITEM_H
