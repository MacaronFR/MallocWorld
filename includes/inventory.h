#ifndef MALLOCWORLD_INVENTORY_H
#define MALLOCWORLD_INVENTORY_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <terminalManager.h>
#include <item.h>

#define MAX_SLOTS_INVENTORY 10

typedef struct s_slot{
    item *item; //TODO
    int quantity;
} slot;

typedef struct s_inventory{
    slot slots[MAX_SLOTS_INVENTORY];
} inventory;

typedef struct s_storage{
	slot* slots;
	size_t size;
} storage;

//---------------------- Creation et Destruction ----------------------
int initInventory(inventory* inventory);
inventory* createInventory();
void freeInventory(inventory* inventory);

storage *createStorage();
void freeStorage(storage *);

//---------------------- Test du contenue ----------------------
bool isStackFull(slot *slot);
int indexEmptySlot(inventory* inventory);
int indexSlotInInventory(inventory *inventory, int32_t id, int start);
void inventoryContainCategory(inventory *inventory, category category, item** tabItem);

//---------------------- Récupération et Modification ----------------------
bool addItemInInventory(inventory *inventory, item* add);
void removeItemInInventory(inventory *inventory, item* item);
item *retrieveItemInInventory(inventory *inventory, int32_t id);
void decrementStackInInventory(inventory *inventory, item* item, int count);

int indexSlotInStorage(storage *, int32_t);
bool addItemInStorage(storage *, item *);
item *retrieveItemInStorage(storage *, int32_t);
void removeSlot(storage *s, int index);

//---------------------- Affichage ----------------------
void printInventory(inventory* inventory);
void printLineSeparator(int count);
void printSlot(inventory* inventory, int id);
void printQuantity(inventory* inventory, int id);
#endif