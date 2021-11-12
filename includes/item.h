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

typedef enum e_itemType {
    ITEMS = 32768,
    RESSOURCES = 16384
}itemType;
typedef enum e_ressources {
    WOODS = RESSOURCES + 8192,
    ORES = RESSOURCES + 4096,
    PLANTS = RESSOURCES + 2048
}ressources;
typedef enum e_items {
    ARMORS = ITEMS + 8192,
    WEAPONS = ITEMS + 4096,
    TOOLS = ITEMS + 2048,
    POTIONS = ITEMS + 1024
}items;

/*typedef enum e_weapons {
    SWORD = WEAPONS + 512,
    SPEAR = WEAPONS + 256,
    HAMMER = WEAPONS + 128
}weapons;
typedef enum e_tools {
    AXE = TOOLS + 512,
    PICKAXE = TOOLS + 256,
    HOE = TOOLS + 128
}tools;*/

typedef struct s_item{
    uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    int32_t id; //id objet
    uint8_t flag; // zone
    int8_t durability; //durabilité ou cout en durabilité
	uint8_t maxStack;
    int32_t *craft; // si item son craft en tableau d'id nécessaire
	char *name;
	struct s_item *next;
} item;

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
