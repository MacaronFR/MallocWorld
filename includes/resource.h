#ifndef MALLOCWORLD_RESOURCE_H
#define MALLOCWORLD_RESOURCE_H

#include <utils.h>
#include <item.h>

typedef struct s_resource{
	int32_t id;
	uint8_t flag;
	uint8_t respawn;
	item *item;
}resource;

resource *loadResource(const char *filename, item **itemList, size_t nItem);
resource **loadResources(const char *dir, size_t *nResource, item **itemList, size_t nItem);
void freeResourceList(resource **r, size_t nResource);

#endif //MALLOCWORLD_RESOURCE_H
