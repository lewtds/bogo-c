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

struct ListItem *listNext(struct ListItem *list);
struct List *listAppend(struct List *list, void *item);
void listFromArray(struct List *list, void *array, int itemSize, int len);


#endif // LIST_H
