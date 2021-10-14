#ifndef MALLOCWORLD_INVENTORY_H
#define MALLOCWORLD_INVENTORY_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <item.h>

typedef struct s_slot{
    item** item;
    int quantity;
} slot;

typedef struct s_inventory{
    slot* slots[10];
} inventory;

#define MAX_SLOTS_INVENTORY 10


//---------------------- Creation et Destruction ----------------------
int initInventory(inventory* inventory);
inventory* createInventory();
void freeInventory(inventory* inventory);

//---------------------- Test du contenue ----------------------
bool isLikeItem(item* item1, item* item2);
bool isFullStack(slot* slot);
bool isFullInventory(inventory* inventory);
bool isInInventory(inventory* inventory, int32_t id);

//---------------------- Récupération et Modification ----------------------
int indexInInventory(inventory* inventory, item* item);
void addItemInInventory(inventory* inventory, item* item);
void removeItemInInventory(inventory* inventory, item* item);
void decrementStackInInventory(inventory* inventory, item* item, int count);

//---------------------- Affichage ----------------------
void printInventory(inventory* inventory);


#endif