#ifndef MALLOCWORLD_ITEM_H
#define MALLOCWORLD_ITEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h>
#include <utils.h>

typedef enum e_typeItem {
    RESSOURCES = 1,
    ITEM = 2
}typeItem;

typedef enum e_categorieItem {
    sword = 4,
    spear = 8,
    hammer = 16,
    armor = 32,
    pickaxe = 64,
    hoe = 128,
    axe = 256,
    wood = 512,
    ore = 1024,
    plant = 2048
}categorieItem;

typedef enum e_levelItem {
    level1 = 4096,
    level2 = 8192,
    level3 = 16384,
    level4 = 32768
}levelItem;

typedef struct s_item{
    uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    int8_t durability; //durabilité ou cout en durabilité
    int32_t id; //id objet
    int32_t *craft; // si item son craft en tableau d'id nécessaire
    uint8_t flag; // zone
	char *name;
}item;

void init_item(item *item);
item *load_item(const char *filename);
item **load_items(const char *dir, size_t *nItem);
void freeItemList(item **items, int length);
bool checkCraftValidity(item **items, int length);

//-------------- CHECK TYPE ITEM --------------
bool isItem(item *item);
bool isRessource(item *item);
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
