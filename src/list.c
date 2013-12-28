/*

  This file is a part of bogoengine project.

  Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>

  bogoengine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  bogoengine is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <malloc.h>
#include <string.h>
#include "list.h"
#include "common.h"

struct List *listNew() {
    struct List *newList = malloc(sizeof(struct List));
    memset(newList, 0x00, sizeof(struct List));
}

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


void listFree(struct List *list)
{
    struct ListItem *iter = list->first;
    while (iter != NULL) {
        struct ListItem *next = iter->next;
        free(iter);
        iter = next;
    }
    free(list);
}
