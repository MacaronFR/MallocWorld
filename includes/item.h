#ifndef MALLOCWORLD_ITEM_H
#define MALLOCWORLD_ITEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



typedef struct s_item{
    int8_t durability; //durabilité ou cout en durabilité
    uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    int32_t id; //id objet
    int32_t *craft; // si item son craft en tableau d'id nécessaire
    uint8_t flag; // zone
} item;


#endif //MALLOCWORLD_ITEM_H
