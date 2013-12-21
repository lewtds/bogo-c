#include <malloc.h>
#include "list.h"


struct List *listAppend(struct List *list, void *item)
{
    struct ListItem *newItem = malloc(sizeof(struct ListItem));
    newItem->item = item;
    newItem->next = NULL;
    newItem->prev = list->last;
    list->last->next = newItem;
    list->length++;

    return list;
}


struct ListItem *listNext(struct ListItem *listItem)
{
    return listItem->next;
}
