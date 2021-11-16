#include <craft.h>

item *craftItem(inventory *inv, storage *s, item *toCraft){
	int i = 0;
	item *list = NULL;
	item *tmp;
	while(toCraft->craft[i] != 0){
		tmp = retrieveItemInInventory(inv, toCraft->id);
		if(tmp == NULL){
			retrieveItemInStorage(s, toCraft->id);
		}
		if(tmp == NULL){
			break;
		}
		tmp->next = list;
		list = tmp;
		++i;
	}
	if(toCraft->craft[i] != 0){
		while(list != NULL){
			tmp = list;
			list = list->next;
			addItemInStorage(s, tmp);
		}
		return NULL;
	}
	while(list != NULL){
		tmp  = list;
		list = list->next;
		freeItem(tmp);
	}
	return copyItem(toCraft);
}