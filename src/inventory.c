
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

//---------------------- Creation et Destruction ----------------------
void initInventory(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
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