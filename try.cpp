#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

LL * initLinkedList(const size_t typeSize)
{
    LL * ptr = malloc(sizeof(LL));
    
    if (!ptr) {
        perror("failed\n");
        return NULL;
    }
    
    ptr->root = NULL;
    ptr->tail = NULL;
    ptr->typesize = typeSize;
    ptr->len = 0;
    
    return ptr;
}

static Node * makeNode(void * data)
{
    Node * ptr = malloc(sizeof(Node));
    
    if (!ptr) {
        perror("failed\n");
        return NULL;
    }
    
    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->data = data;
    
    return ptr;
}

_Bool addNode(LL * ptr, void * data)
{
    if (ptr->root == NULL) {
        ptr->root = makeNode(data);
        
        if (!ptr->root) {
            perror("failed\n");
            return 0;
        }
        
        ptr->tail = ptr->root;
        ptr->len++;
        return 1;
    }
    
    Node * iterator = ptr->root;
    
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    
    iterator->next = makeNode(data);
    
    if (!iterator->next) {
        perror("failed\n");
        return 0;
    }
    
    ptr->tail = iterator->next;
    iterator->next->prev = iterator;
    ptr->len++;
    return 1;
}

_Bool deleteAt(LL * ptr, size_t idx)
{
    if (!(idx <= ptr->len - 1)) {
        perror("out of bounds!\n");
        return 0;
    }
    if (idx == ptr->len - 1) {
        Node * temp = ptr->tail->prev;
        free(ptr->tail);
        ptr->tail = temp;
        ptr->tail->next = NULL;
        
        return 1;
    }
    
    Node * iterator = ptr->root;
    
    while (idx--) {
        iterator = iterator->next;
    }
    
    Node * temp = iterator;
    iterator->prev->next = temp->next;
    temp->next->prev = iterator->prev;
    free(temp);
    
    return 1;
}

_Bool addAt(LL * ptr, void * data, size_t idx)
{
    if (!(idx <= ptr->len)) {
        perror("out of bounds!\n");
        return 0;
    }
    
    if (ptr->root == NULL || idx == ptr->len - 1) {
        addNode(ptr, data);
        
        return 1;
    }
    
    Node * new = makeNode(data);
    
    if (!new) {
        perror("failed\n");
        return 0;
    }
    
    Node * iterator;
    if (ptr->len - idx > idx) {
        iterator = ptr->root;
        while (idx--) {
            iterator = iterator->next;
        }
    }
    else {
        iterator = ptr->tail;
        size_t idx_t = ptr->len - idx - 1;
        while (idx_t--) {
            iterator = iterator->prev;
        }
    }
    
    iterator->prev->next = new;
    new->next = iterator;
    iterator->prev = new;
    ptr->len++;
    
    return 1;
}
// 1 2 3 4 6
static _Bool memEqual(const void * p1, const void * p2, size_t typeSize)
{
    for (size_t i = 0; i < typeSize; ++i) {
        if (*(char *)p1++ != *(char *)p2++) {
            return 0;
        }
    }
    
    return 1;
}

Node * linearSearch(const LL * ptr, const void * data)
{
    Node * iterator = ptr->root;
    
    while (iterator && !memEqual(iterator->data, data, ptr->typesize)) {
        iterator = iterator->next;
    }
    
    return iterator;
}

static void swap(Node * p1, Node * p2)
{
    void * temp = p1->data;
    p1->data = p2->data;
    p2->data = temp;
}

static Node * indexOp(Node * ptr, size_t idx)
{
    while (idx--) {
        ptr = ptr->next;
    }
    return ptr;
}

void sortList(LL * ptr, fptr_t fp)
{
    Node * iterator = ptr->root;
    for (size_t i = 0; i < ptr->len - 1; ++i) {
        for (size_t j = 0; j < ptr->len - 1 - i; ++j) {
            if (fp(indexOp(iterator, j)->data, indexOp(iterator, j + 1)->data)) {
                swap(indexOp(iterator, j), indexOp(iterator, j + 1));
            }
        }
    }
}

void deleteList(LL * ptr)
{
    Node * iterator = ptr->tail;
    
    while (ptr->len--) {
        free(iterator->data);
        free(iterator);
        iterator = iterator->prev;
    }
    
    ptr->root = NULL;
    ptr->tail = NULL;
    ptr->len = 0;
    ptr->typesize = 0;
}
