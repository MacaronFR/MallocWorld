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
    inventory* inv = malloc(sizeof(inv));
    if(inv == NULL){
        fprintf(stderr, "Error : Out of memory");
        return NULL;
    }
    if(initInventory(inv) == -1){
        return NULL;
    }
    return inv;
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

storage *createStorage(){
	storage *s = malloc(sizeof(storage));
	s->size = 0;
	s->slots = NULL;
	return s;
}

void freeStorage(storage *s){
	item *tmp;
	for(size_t i = 0; i < s->size; ++i){
		while(s->slots[i]->item != NULL){
			tmp = s->slots[i]->item->next;
			free(s->slots[i]->item);
			s->slots[i]->item = tmp;
		}
		free(s->slots[i]);
	}
	free(s);
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

int indexSlotInStorage(storage *s, int32_t id){
	for(size_t i = 0; i < s->size; ++i){
		if(s->slots[i]->item->id == id){
			return i;
		}
	}
	return -1;
}

bool addItemInStorage(storage *s, item *add){
	int index = indexSlotInStorage(s, add->id);
	slot **tmp;
	if(index == -1){
		index = s->size;
		tmp = realloc(s->slots, sizeof(slot*) * s->size + 1);
		if(tmp == NULL){
#ifdef DEBUG
			fprintf(stderr, "Out of memory in allocation of new storage slot");
#endif
			freeStorage(s);
			return false;
		}
		s->slots[s->size]->item = NULL;
		s->slots[s->size]->quantity = 0;
		s->size += 1;
	}
	add->next = s->slots[index]->item;
	s->slots[index]->item = add;
	s->slots[index]->quantity += 1;
	return true;
}

item *retrieveItemInStorage(storage *s, int32_t id){
	int index = indexSlotInStorage(s, id);
	item *tmp;
	if(index == -1){
		return NULL;
	}
	tmp = s->slots[index]->item;
	s->slots[index]->item = tmp->next;
	s->slots[index]->quantity -= 1;
	if(s->slots[index]->quantity <= 0){
		removeSlot(s, index);
	}
	return tmp;
}

void removeSlot(storage *s, int index){
	free(s->slots[index]);
	for(int i = index; i < s->size - 1; ++i){
		s->slots[i] = s->slots[i + 1];
	}
	s->size -= 1;
	s->slots = realloc(s->slots, sizeof(slot*) * s->size);
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