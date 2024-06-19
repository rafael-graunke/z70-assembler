#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEY_SIZE 255

typedef struct
{
    void *value;
    char key[MAX_KEY_SIZE];
} HashmapNode;

typedef struct
{
    HashmapNode **entries;
    int curr;
    int size;
} HashMap;

HashMap *hm_create(int size);
void hm_destroy(HashMap *hashmap);
bool hm_insert(HashMap *hashmap, char *key, void *value);
void *hm_fetch(HashMap *hashmap, char *key);

#endif