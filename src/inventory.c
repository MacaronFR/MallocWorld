//
// Created by basil on 12/10/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int bool ;  //définition du type booléen
#define false  0;
#define true  1;


typedef struct Item {
    int object;
    int durability;
}Item;

typedef struct Slot {
    Item* item;
    int quantity;
}Slot;

typedef struct Inventory {
    Slot* slots[10];
}Inventory;

//CONSTANTE
const int MAX_SLOTS_INVENTORY = 10;

//---------------------- Creation et Destruction ----------------------
void initInventory(Inventory* inventory) {
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        inventory->slots[i]->item = NULL;
        inventory->slots[i]->quantity = 0;
    }
}

Inventory* createInventory() {
    Inventory* inventory = malloc(sizeof(Inventory));
    initInventory(inventory);
}

void freeInventory(Inventory* inventory) {
    for (int i=0; i<MAX_SLOTS_INVENTORY ; i++) {
        free(inventory->slots[i]);
    }
    free(inventory);
}

//---------------------- Test du contenue ----------------------
bool isLikeItem(Item* item1, Item* item2) {
    return item1->object == item2->object;
}

bool isFullStack(Slot* slot) {
    return slot->quantity >= 20;
}

bool isFullInventory(Inventory* inventory) {
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        if(inventory->slots[i]->quantity == 0)
            return false;
    }
    return true;
}

bool isFullStackInventory(Inventory* inventory) {
    if(!isFullInventory(inventory)) {
        return false;
    }
    else{
        for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
            if(inventory->slots[i]->item == 0)
                return false;
        }
    }

}

bool isInInventory(Inventory* inventory, Item* item) {
    int i = 0;
    while(inventory->slots[i] != NULL && i<MAX_SLOTS_INVENTORY) {
        if(isLikeItem(inventory->slots[i]->item, item)) {
            return true;
        }
        i++;
    }
    return false;
}

//---------------------- Récupération et Modification ----------------------
int indexInInventory(Inventory* inventory, Item* item) {
    int i = 0;
    while(inventory->slots[i] != NULL && i<MAX_SLOTS_INVENTORY) {
        if(isLikeItem(inventory->slots[i]->item, item)) {
            return i;
        }
        i++;
    }
    return -1;
}