#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct s_item{
	int8_t durability; //durabilité ou cout en durabilité
	uint16_t type; // type de l'objet (ressources/item arme/armure/outil/potion/arbre/roche/plante épée/lance/…
	int32_t id; //id objet
	int32_t *craft; // si item son craft en tableau d'id nécessaire
	uint8_t flag; // zone
} item;

typedef struct s_slot{
	item **item;
	int quantity;
} slot;

typedef struct s_inventory{
	slot *slots[10];
} inventory;

#define MAX_SLOTS_INVENTORY 10
#define MAX_STACK 20

//---------------------- Creation et Destruction ----------------------
int initInventory(inventory *inventory){
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

inventory *createInventory(){
	inventory *inventory = malloc(sizeof(inventory));
	if(inventory == NULL){
		fprintf(stderr, "Error : Out of memory");
		return NULL;
	}
	if(initInventory(inventory) == -1){
		return NULL;
	}
	return inventory;
}

void freeInventory(inventory *inventory){
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

bool isFullInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i]->quantity == 0)
			return false;
	}
	return true;
}

bool isStackFull(slot *slot){
	return slot->quantity >= MAX_STACK;
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
int indexInInventory(inventory *inventory, item *item){
	int i = 0;
	while(inventory->slots[i] != NULL && i < MAX_SLOTS_INVENTORY){
		if(itemSameId(inventory->slots[i]->item, item)){
			return i;
		}
		i++;
	}
	return -1;
}