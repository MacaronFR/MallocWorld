#ifndef MALLOCWORLD_CRAFT_H
#define MALLOCWORLD_CRAFT_H

#include <inventory.h>

item *craftItem(inventory *inv, storage *s, item *toCraft);
item **getCraftableItem(item **listItem, size_t nItem, int zone);


#endif //MALLOCWORLD_CRAFT_H
