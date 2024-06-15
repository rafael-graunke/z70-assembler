#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEY_SIZE 255

typedef struct
{
    int value;
    char key[MAX_KEY_SIZE];
} HashmapNode;

typedef struct
{
    HashmapNode **entries;
    int curr;
    int size;
} HashMap;

HashMap *create_hashmap(int size);
void destroy_hashmap(HashMap *hashmap);
bool hashmap_insert(HashMap *hashmap, char *key, int value);
int hashmap_fetch(HashMap *hashmap, char *key);

#endif