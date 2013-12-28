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


#ifndef LIST_H
#define LIST_H

struct ListItem {
    void *item;
    struct ListItem *next;
    struct ListItem *prev;
};

struct List {
    struct ListItem *first;
    struct ListItem *last;
    int length;
};


struct List *listNew();
struct ListItem *listNext(struct ListItem *list);
struct List *listAppend(struct List *list, void *item);
void listFromArray(struct List *list, void *array, int itemSize, int len);
struct ListItem *listIndex(struct List *list, int index);
void freeList(struct List *list);

#endif // LIST_H
