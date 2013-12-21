#include <malloc.h>
#include "list.h"
#include "common.h"


struct List *listAppend(struct List *list, void *item)
{
    struct ListItem *newItem = new(struct ListItem);

    newItem->item = item;
    newItem->next = NULL;
    newItem->prev = list->last;
    list->length++;

    if (list->first == NULL) {
        list->first = newItem;
    } else {
        list->last->next = newItem;
    }

    list->last = newItem;

    return list;
}


struct ListItem *listNext(struct ListItem *listItem)
{
    return listItem->next;
}


void listFromArray(struct List *list, void *array, int itemSize, int len)
{
    for (int i = 0; i < len; i++) {
        listAppend(list, array + itemSize);
    }
}

struct ListItem *listIndex(struct List *list, int index) {
    struct ListItem *iter = list->first;
    while (iter != NULL && index > 0) {
        index--;
        iter = iter->next;
    }
    return iter;
}
