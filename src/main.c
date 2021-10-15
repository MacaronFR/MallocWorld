#include <item.h>

int main(){
	item *res = loadItem("../test.mw");
	if(res == NULL){
		return 1;
	}
	free(res);
	return 0;
}