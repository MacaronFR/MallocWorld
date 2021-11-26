#include <craft.h>

item *craftItem(inventory *inv, storage *s, item *toCraft){
	int i = 0;
	item *list = NULL;
	item *tmp;
	while(toCraft->craft[i] != 0){
		tmp = retrieveItemInInventory(inv, toCraft->craft[i]);
		if(tmp == NULL){
			tmp = retrieveItemInStorage(s, toCraft->craft[i]);
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

item **getCraftableItem(item **listItem, size_t nItem, int zone){
	item **craftable = NULL;
	item **tmp;
	int count = 0;
	for(size_t i = 0; i < nItem; ++i){
		if(listItem[i]->craft[0] != 0 && (listItem[i]->flag & (1 << zone)) != 0){
			tmp = realloc(craftable, (count + 1) * sizeof(item *));
			if(tmp == NULL){
				if(craftable != NULL){
					free(craftable);
				}
				return NULL;
			}
			tmp[count] = listItem[i];
			count++;
			craftable = tmp;
		}
	}
	if(count > 0){
		tmp = realloc(craftable, (count +1) * sizeof(item *));
		if(tmp == NULL){
			free(craftable);
			return NULL;
		}
		craftable = tmp;
		craftable[count] = NULL;
	}
	return craftable;
}