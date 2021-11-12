#include <map.h>
#include <player.h>
#include <inventory.h>
#include <item.h>
#include <monster.h>
#include <terminalManager.h>

void printFlag(int pos, char* class) {
    setText(1,FOREGROUND_YELLOW);
    if(pos == 0)
        printf("\n|-------------------| start TEST %s |-------------------\n", class);
    else if(pos == 1)
        printf("|-------------------| end TEST %s |-------------------\n", class);
    else {
        setText(1,FOREGROUND_RED);
        printf("ERROR 'printFlag' : pos can have value [0,1]");
        setTextDefault();
    }
    setTextDefault();
}

void testPlayer(player* player) {
    printFlag(0, "PLAYER");
    printPlayer(player);


    printFlag(1, "PLAYER");
}

void testInventory(inventory* inventory) {
    printFlag(0, "INVENTORY");
    printInventory(inventory);

    printFlag(1, "INVENTORY");
}

void testItem() {
    printFlag(0, "ITEM");
    //item *res = load_item("../test.mw");
    //if(res == NULL){
    //return 1;
    //}
    //free(res);
    size_t nItem;
    item **itemList = load_items("..", &nItem);
    if(itemList != NULL){
        if(checkCraftValidity(itemList, nItem)){
            printf("testLoadItem : OK\n");
        }else{
            printf("testLoadItem : NIKK\n");
        }
        freeItemList(itemList, nItem);
    }


    printFlag(1, "ITEM");
}



int main(){
    player* player = createPlayer();
    //testPlayer(player);
    //testInventory(player->inventory);

	testItem();

	return 0;
}