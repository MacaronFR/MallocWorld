
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <inventory.h>
#include <terminalManager.h>


#define MAX_SLOTS_INVENTORY 10
#define MAX_STACK 20

//---------------------- Creation et Destruction ----------------------
inventory* createInventory(){
    inventory* inventory = malloc(sizeof(inventory));
    if(inventory == NULL){
        fprintf(stderr, "Error : Out of memory");
        return NULL;
    }
    if(initInventory(inventory) == -1){
        return NULL;
    }
    return inventory;
}

int initInventory(inventory* inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		inventory->slots[i] = malloc(sizeof(slot));
		if(inventory->slots[i] == NULL){
			fprintf(stderr, "Error : Out of memory");
			freeInventory(inventory);
			return -1;
		}
		inventory->slots[i]->item = malloc(sizeof(item *) * 20);
		if(inventory->slots[i]->item == NULL){
			fprintf(stderr, "Error : Out of memory");
			freeInventory(inventory);
			return -1;
		}
		inventory->slots[i]->item = NULL;
		inventory->slots[i]->quantity = 0;
	}
	return 1;
}

void freeInventory(inventory* inventory){
	for(int i = 0; inventory->slots[i] != NULL && i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->item == NULL){
			free(inventory->slots[i]->item);
		}
		free(inventory->slots[i]);
	}
	free(inventory);
}

//---------------------- Test du contenue ----------------------
bool isStackFull(slot *slot){
    return slot->item != NULL && slot->item->maxStack <= slot->quantity;
}

int indexEmptySlot(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->quantity == 0)
			return i;
	}
	return -1;
}

int indexSlotInInventory(inventory *inventory, int32_t id, int start){
	for(int i = start; i < MAX_SLOTS_INVENTORY; ++i){
		if(inventory->slots[i]->item != NULL && inventory->slots[i]->item->id == id){
			return i;
		}
	}
	return -1;
}

bool addItemInInventory(inventory *inventory, item *add) {
    int slot = indexSlotInInventory(inventory, add->id, 0);
    int emptySlot;

    while(slot != -1) {
        if(!isStackFull(inventory->slots[slot])) {
            add->next = inventory->slots[slot]->item;
            inventory->slots[slot]->item = add;
            inventory->slots[slot]->quantity++;
            return true;
        }
        else{
            slot++;
        }
        slot = indexSlotInInventory(inventory, add->id, slot);
    }
    emptySlot = indexEmptySlot(inventory);
    if(emptySlot != -1) {
        add->next = NULL;
        inventory->slots[emptySlot]->item = add;
        inventory->slots[emptySlot]->quantity++;
        return true;
    }
    return false;
}
//---------------------- GET ET SET ----------------------


//----------------------| AFFICHAGE |----------------------
void printInventory(inventory* inventory) {
    printLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printSlot(inventory, i);
    }
    printf(" <- Item\n");
    printLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printQuantity(inventory, i);
    }
    printf(" <- Count/Durability\n");
    printLineSeparator(MAX_SLOTS_INVENTORY);
}
void printSlot(inventory* inventory, int id) {
    setText(1,FOREGROUND_BLUE);
    printf("|");
    setTextDefault();
    if(inventory->slots[id]->item != NULL) {
        item* item = inventory->slots[id]->item;
        printf("%3d", item->id);
    }
    else {
        printf("   ");
    }
    if(id == MAX_SLOTS_INVENTORY-1) {
        setText(1,FOREGROUND_BLUE);
        printf("|");
        setTextDefault();
    }
}
void printQuantity(inventory* inventory, int id) {
    setText(1, FOREGROUND_BLUE);
    printf("|");
    setTextDefault();
    if(inventory->slots[id]->item != NULL) {
        item *item = inventory->slots[id]->item;
        if (isRessource(item))
            printf("%3d", inventory->slots[id]->quantity);
        else{
            setText(1, FOREGROUND_GREEN);
            printf("%3d", item->durability);
            setTextDefault();
        }
    }
    else
        printf("   ");
    if(id == MAX_SLOTS_INVENTORY-1) {
        setText(1,FOREGROUND_BLUE);
        printf("|");
        setTextDefault();
    }
}
void printLineSeparator(int count) {
    setText(2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    for(int i=0 ; i<count ; i++) {
        printf("+---");
    }
    printf("+\n");
    setTextDefault();
}