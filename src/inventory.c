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
	tabItem = malloc(sizeof (item*) * (count+1));
	count = 0;
	for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
		if(inventory->slots[i].item != NULL && (inventory->slots[i].item->type & category) == category) {
			tabItem[count] = inventory->slots[i].item;
			count++;
		}
	}
	tabItem[count] = NULL;
	return tabItem;
}

//---------------------- R??cup??ration et Modification ----------------------
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
	s->slots = realloc(s->slots, sizeof(slot) * s->size);
}
//---------------------- GET ET SET ----------------------


//----------------------| AFFICHAGE |----------------------
void printInventory(inventory* inventory) {
	printc("\n   Inventaire :\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	printInventoryLineSeparator(inventory->slots,MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printName(inventory->slots[i], i);
    }
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    printf(" <- Item\n");
	printInventoryLineSeparator(inventory->slots,MAX_SLOTS_INVENTORY);
	for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
		printId(inventory->slots[i], i);
	}
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf(" <- ID\n");
	printInventoryLineSeparator(inventory->slots,MAX_SLOTS_INVENTORY);
    for(int i=0 ; i<MAX_SLOTS_INVENTORY ; i++) {
        printQuantity(inventory->slots[i], i);
    }
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    printf(" <- Count/Durability\n");
	printInventoryLineSeparator(inventory->slots,MAX_SLOTS_INVENTORY);
}
void printName(slot slot, int id) {
    printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    if(slot.item != NULL) {
        item* item = slot.item;
        printf("%20s ",item->name);
    }
    else {
        printf("%3s", "");
    }
}
void printId(slot slot, int id) {
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	if(slot.item != NULL) {
		item* item = slot.item;
		printf("%20d ",item->id);
	}
	else {
		printf("%3s", "");
	}
}
void printQuantity(slot slot, int id) {
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
    if(slot.item != NULL) {
        item *item = slot.item;
        if ((item->type & RESSOURCES) != 0) {
			setTextDefault();
			printf("%20d ", slot.quantity);
		}
        else{
            setTextDefault();
            printf("%20d ", item->durability);
        }
    }
    else
        printf("%3s","");
}
void printInventoryLineSeparator(slot *slots, int count) {
    setText(2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf("    ");
    for(int i=0 ; i<count ; i++) {
		if(slots[i].item != NULL)
        	printf("+---------------------");
		else
			printf("+---");
    }
    printf("+\n    ");
    setTextDefault();
}

void printStorage(storage *storage) {
	int size = storage->size;
	printc("\n   Stockage :\n",2,FOREGROUND_GREEN,FOREGROUND_INTENSITY);
	if(size == 0) {
		printc(	"\n    +---+"
			   		"\n    |   |"
				   "\n    +---+"
				   "\n    |   |"
				   "\n    +---+"
				   "\n    |   |"
				   "\n    +---+\n",2,FOREGROUND_INTENSITY,FOREGROUND_BLUE);
		return;
	}
	printInventoryLineSeparator(storage->slots, size);
	for(int i=0 ; i<size ; i++) {
		printName(storage->slots[i], i);
	}
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf(" <- Item\n");
	printInventoryLineSeparator(storage->slots, size);
	for(int i=0 ; i<size ; i++) {
		printId(storage->slots[i], i);
	}
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf(" <- ID\n");
	printInventoryLineSeparator(storage->slots, size);
	for(int i=0 ; i<size ; i++) {
		printQuantity(storage->slots[i], i);
	}
	printc("|",2,FOREGROUND_BLUE,FOREGROUND_INTENSITY);
	printf(" <- Count/Durability\n");
	printInventoryLineSeparator(storage->slots, size);
}


void repairInventory(item **itemList, size_t nItem, inventory *inv){
	item *tmp;
	for(int i = 0; i < MAX_SLOTS_INVENTORY; ++i){
		tmp = inv->slots[i].item;
		while(tmp != NULL && repair(itemList, nItem, tmp)){
			tmp = tmp->next;
		}
	}
}