
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include "item.h"
#include "id.h"


bool isSword(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
}
bool isPickaxe(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
};
bool isPickaxe(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
};
bool isPickaxe(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
};
bool isPickaxe(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
};
bool isPickaxe(item* item) {
    for(int i=0 ; i<sizeof(I_PICKAXES) ; i++) {
        if(item->id == I_PICKAXES[i])
            return true;
    }
    return false;
};
bool isWeapon(item* item);
bool isArmor(item* item);
bool isRessource(item* item);
bool canStack(item* item);

void useDurability(item* item);
void repareItem(item* item);