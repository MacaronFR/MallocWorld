
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <inventory.h>


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
		inventory->slots[i]->item[0] = NULL;
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
bool itemSameId(item *item1, item *item2){
	return item1->id == item2->id;
}

bool isFullStack(slot *slot){
	return slot->quantity >= MAX_STACK;
}

bool isStackFull(slot *slot){
    return slot->quantity >= MAX_STACK;
}

bool isFullInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->quantity == 0)
			return false;
	}
	return true;
}




bool isInInventory(inventory *inventory, int32_t id){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; ++i){
		if(inventory->slots[i]->item[0] != NULL && inventory->slots[i]->item[0]->id == id){

			return true;
		}
	}
	return false;
}

//---------------------- Récupération et Modification ----------------------
int indexSlotInInventory(inventory *inventory, item *item){
	int i = 0;
	while(inventory->slots[i] != NULL && i < MAX_SLOTS_INVENTORY){
		if(itemSameId(inventory->slots[i]->item[0], item)){ //TODO correction comparaison item
			return i;
		}
		i++;
	}
	return -1;
}

//---------------------- Affichage ----------------------
void printInventory(inventory* inventory) {
    printLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printSlot(inventory, i);
    }
    printf("| <- Item\n");
    printLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printQuantity(inventory, i);
    }
    printf("| <- Count/Durability\n");
    printLineSeparator(MAX_SLOTS_INVENTORY);
}
void printSlot(inventory* inventory, int id) {
    if(inventory->slots[id]->item[0] != NULL) {
        item* item = inventory->slots[id]->item[0];
        printf("|%3d", item->id);
    }
    else {
        printf("|   ");
    }

}
void printQuantity(inventory* inventory, int id) {
    if(inventory->slots[id]->item[0] != NULL) {
        item* item = inventory->slots[id]->item[0];
        //if(isRessource(item))
            //printf("|%3d", inventory->slots[id]->quantity);
        //else
            //printf("|%3d", item->durability);
    }
    else
        printf("|   ");
}
void printLineSeparator(int count) {
    for(int i=0 ; i<count ; i++) {
        printf("+---");
    }
    printf("+\n");
}