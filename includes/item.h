#ifndef MALLOCWORLD_ITEM_H
#define MALLOCWORLD_ITEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h>

#include <terminalManager.h>
#include <utils.h>

typedef enum e_category {
    ITEMS = 32768,
    RESSOURCES = 16384,

	WOODS = RESSOURCES + 8192,
	ORES = RESSOURCES + 4096,
	PLANTS = RESSOURCES + 2048,

	ARMORS = ITEMS + 8192,
	WEAPONS = ITEMS + 4096,
	TOOLS = ITEMS + 2048,
	POTIONS = ITEMS + 1024
}category;
/*
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
}items;*/

typedef struct s_item{
    uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
    int32_t id; //id objet
    uint8_t flag; // zone / nb degat / % de reduction / nb de soin
    int8_t durability; //durabilité ou cout en durabilité
	uint8_t maxStack;
    int32_t *craft; // si item son craft en tableau d'id nécessaire
	char *name;
	struct s_item *next;
} item;

item *load_item(const char *filename);
item **loadItems(const char *dir, size_t *nItem);
void freeItemList(item **items, size_t length);
bool checkCraftValidity(item **items, int length);
void freeItem(item *item);
item *copyItem(item *i);

//-------------- CHECK TYPE ITEM --------------
bool isItem(item *item);
bool isRessource(item *item);
bool isWood(item* item);
bool isOre(item* item);
bool isPlant(item* item);
bool isArmor(item* item);
bool isWeapon(item* item);
bool isTool(item* item);
bool isPotion(item* item);
/*
bool isSword(item* item);
bool isPickaxe(item* item);
bool isHoe(item* item);
bool isAxe(item* item);
bool isSpear(item* item);
bool isHammer(item* item);*/



void useDurability(item* item);
void repareItem(item* item);



#endif //MALLOCWORLD_ITEM_H
