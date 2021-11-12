#include <item.h>

int main(){
	//item *res = load_item("../test.mw");
	//if(res == NULL){
		//return 1;
	//}
	//free(res);
	size_t nItem;
	item **itemList = load_items("..", &nItem);
	if(itemList != NULL){
		if(checkCraftValidity(itemList, nItem)){
			printf("OK");
		}else{
			printf("NIKK");
		}
		freeItemList(itemList, nItem);
	}
	return 0;
}