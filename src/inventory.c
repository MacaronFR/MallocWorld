#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <inventory.h>



#define MAX_SLOTS_INVENTORY 10
#define MAX_STACK 20

//---------------------- Creation et Destruction ----------------------
inventory* createInventory(){
    inventory* inv = malloc(sizeof(inventory));
    if(inv == NULL){
#ifdef DEBUG
        fprintf(stderr, "Error : Out of memory");
#endif
        return NULL;
    }
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		inv->slots[i].item = NULL;
		inv->slots[i].quantity = 0;
	}
    return inv;
}

void freeInventory(inventory* inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i].item != NULL){
			freeInventorySlot(&(inventory->slots[i]));
		}
	}
	free(inventory);
}

void freeInventorySlot(slot *s){
	item *tmp;
	for(int j = 0; j < s->quantity; ++j){
		tmp = s->item->next;
		freeItem(s->item);
		s->item = tmp;
	}
}

storage *createStorage(){
	storage *s = malloc(sizeof(storage));
	if(s == NULL){
		return NULL;
	}
	s->size = 0;
	s->slots = NULL;
	return s;
}

void freeStorage(storage *s){
	item *tmp;
	for(size_t i = 0; i < s->size; ++i){
		while(s->slots[i].item != NULL){
			tmp = s->slots[i].item->next;
			freeItem(s->slots[i].item);
			s->slots[i].item = tmp;
		}
	}
	free(s->slots);
	free(s);
}

//---------------------- Test du contenue ----------------------
bool isStackFull(slot *slot){
    return slot->item != NULL && slot->item->maxStack <= slot->quantity;
}

int indexEmptySlot(inventory *inventory){
	for(int i = 0; i < MAX_SLOTS_INVENTORY; i++){
		if(inventory->slots[i].quantity == 0)
			return i;
	}
	return -1;
}

int indexSlotInInventory(inventory *inventory, int32_t id, int start){
	for(int i = start; i < MAX_SLOTS_INVENTORY; ++i){
		if(inventory->slots[i].item != NULL && inventory->slots[i].item->id == id){
			return i;
		}
	}
	return -1;
}

item **getItemCategory(inventory *inventory, category category) {
	item **tabItem;
	int count = 0;
	for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
		if(inventory->slots[i].item != NULL && (inventory->slots[i].item->type & category) == category) {
			count++;
		}
	}
	if(count == 0)
		return NULL;
	tabItem = malloc(sizeof (item*) * count);
	count = 0;
	for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
		if(inventory->slots[i].item != NULL && (inventory->slots[i].item->type & category) == category) {
			tabItem[count] = inventory->slots[i].item;
			count++;
		}
	}
	return tabItem;
}

//---------------------- Récupération et Modification ----------------------
bool addItemInInventory(inventory *inventory, item *add) {
    int slot = indexSlotInInventory(inventory, add->id, 0);
    int emptySlot;
    while(slot != -1) {
        if(!isStackFull(&(inventory->slots[slot]))) {
            add->next = inventory->slots[slot].item;
            inventory->slots[slot].item = add;
            inventory->slots[slot].quantity++;
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
        inventory->slots[emptySlot].item = add;
        inventory->slots[emptySlot].quantity++;
        return true;
    }
    return false;
}

item *retrieveItemInInventory(inventory *inv, int32_t id){
	int index = indexSlotInInventory(inv, id, 0);
	item *tmp;
	if(index == -1){
		return NULL;
	}
	tmp = inv->slots[index].item;
	inv->slots[index].item = tmp->next;
	inv->slots[index].quantity -= 1;
	return tmp;
}

int indexSlotInStorage(storage *s, int32_t id){
	for(size_t i = 0; i < s->size; ++i){
		if(s->slots[i].item->id == id){
			return i;
		}
	}
	return -1;
}

bool addItemInStorage(storage *s, item *add){
	int index = indexSlotInStorage(s, add->id);
	slot *tmp;
	if(index == -1){
		index = s->size;
		tmp = realloc(s->slots, sizeof(slot) * (s->size + 1));
		if(tmp == NULL){
#ifdef DEBUG
			fprintf(stderr, "Out of memory in allocation of new storage slot");
#endif
			freeStorage(s);
			return false;
		}
		s->slots = tmp;
		s->slots[s->size].item = NULL;
		s->slots[s->size].quantity = 0;
		s->size += 1;
	}
	add->next = s->slots[index].item;
	s->slots[index].item = add;
	s->slots[index].quantity += 1;
	return true;
}

item *retrieveItemInStorage(storage *s, int32_t id){
	int index = indexSlotInStorage(s, id);
	item *tmp;
	if(index == -1){
		return NULL;
	}
	tmp = s->slots[index].item;
	s->slots[index].item = tmp->next;
	s->slots[index].quantity -= 1;
	if(s->slots[index].quantity <= 0){
		removeSlot(s, index);
	}
	return tmp;
}

void removeSlot(storage *s, int index){
	for(int i = index; i < s->size - 1; ++i){
		s->slots[i] = s->slots[i + 1];
	}
	s->size -= 1;
	s->slots = realloc(s->slots, sizeof(slot*) * s->size);
}
//---------------------- GET ET SET ----------------------


//----------------------| AFFICHAGE |----------------------
void printInventory(inventory* inventory) {
	printInventoryLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printSlot(inventory, i);
    }
    printf(" <- Item\n");
	printInventoryLineSeparator(MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printQuantity(inventory, i);
    }
    printf(" <- Count/Durability\n");
	printInventoryLineSeparator(MAX_SLOTS_INVENTORY);
}
void printSlot(inventory* inventory, int id) {
    printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    if(inventory->slots[id].item != NULL) {
        item* item = inventory->slots[id].item;
        printf("%3d", item->id);
    }
    else {
        printf("   ");
    }
    if(id == MAX_SLOTS_INVENTORY-1) {
        printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    }
}
void printQuantity(inventory* inventory, int id) {
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    if(inventory->slots[id].item != NULL) {
        item *item = inventory->slots[id].item;
        if (isRessource(item)) {
			setTextDefault();
			printf("%3d", inventory->slots[id].quantity);
		}
        else{
            setTextDefault();
            printf("%3d", item->durability);
        }
    }
    else
        printf("   ");
    if(id == MAX_SLOTS_INVENTORY-1) {
		printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    }
}
void printInventoryLineSeparator(int count) {
    setText(2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    for(int i=0 ; i<count ; i++) {
        printf("+---");
    }
    printf("+\n");
    setTextDefault();
}