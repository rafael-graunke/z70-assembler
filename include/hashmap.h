#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEY_SIZE 255

struct hm_value;

typedef struct
{
    struct hm_value *value;
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
bool hm_insert(HashMap *hashmap, char *key, struct hm_value *value);
struct hm_value *hm_fetch(HashMap *hashmap, char *key);

#endif