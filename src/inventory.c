
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "inventory.h"

//---------------------- Creation et Destruction ----------------------
void initInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++) {
		inventory->slots[i]->item = NULL;
		inventory->slots[i]->quantity = 0;
	}
}

inventory *createInventory(){
	inventory *inventory = malloc(sizeof(inventory));
	initInventory(inventory);
}

void freeInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		free(inventory->slots[i]);
	}
	free(inventory);
}

//---------------------- Test du contenue ----------------------
bool isLikeItem(item *item1, item *item2){
	return item1->id == item2->id;
}

bool isFullStack(slot *slot){
	return slot->quantity >= 20;
}

bool isFullInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->quantity == 0)
			return false;
	}
	return true;
}

bool isFullStackInventory(inventory *inventory){
	if(!isFullInventory(inventory)){
		return false;
	}
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->item == 0)
			return false;
	}
	return true;
}

bool isInInventory(inventory *inventory, item *item){
	int i = 0;
	while(inventory->slots[i] != NULL && i < MAX_SLOTS_INVENTORY){
		if(isLikeItem(inventory->slots[i]->item, item)){
			return true;
		}
		i++;
	}
	return false;
}

//---------------------- Récupération et Modification ----------------------
int indexInInventory(inventory *inventory, item *item){
	int i = 0;
	while(inventory->slots[i] != NULL && i < MAX_SLOTS_INVENTORY){
		if(isLikeItem(inventory->slots[i]->item, item)){
			return i;
		}
		i++;
	}
	return -1;
}