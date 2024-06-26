#include <stdio.h>
#include <string.h>
#include "hashmap.h"

// This is djb2 hashing algorithm
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

HashMap *create_hashmap(int size)
{
    HashMap *hashmap = (HashMap *)malloc(sizeof(HashMap));
    if (hashmap == NULL)
    {
        printf("Error allocating hashmap.\n");
        exit(EXIT_FAILURE);
    }
    hashmap->curr = 0;
    hashmap->size = size;
    hashmap->entries = (HashmapNode **)malloc(sizeof(HashmapNode *) * size);

    if (hashmap->entries == NULL)
    {
        printf("Error allocating hashmap nodes.\n");
        exit(EXIT_FAILURE);
    }

    return hashmap;
}

void destroy_hashmap(HashMap *hashmap)
{
    for (int i = 0; i < hashmap->size; i++)
        if (hashmap->entries[i] != NULL)
            free(hashmap->entries[i]);

    free(hashmap->entries);
    free(hashmap);
}

bool hashmap_insert(HashMap *hashmap, char *key, int value)
{
    int pos = hash(key) % hashmap->size;
    int first_pos = pos;
    while (hashmap->entries[pos] != NULL)
    {
        pos = (pos + 1) % hashmap->size;
        if (pos == first_pos)
        {
            printf("Hashmap reached max size of %d.\n", hashmap->size);
            return false;
        }
    }

    hashmap->entries[pos] = (HashmapNode *)malloc(sizeof(HashmapNode));
    strncpy(hashmap->entries[pos]->key, key, MAX_KEY_SIZE);
    hashmap->entries[pos]->value = value;

    return true;
}

int hashmap_fetch(HashMap *hashmap, char *key)
{
    int pos = hash(key) % hashmap->size;
    int first_pos = pos;
    while (hashmap->entries[pos] == NULL || strcmp(hashmap->entries[pos]->key, key) != 0)
    {
        pos = (pos + 1) % hashmap->size;
        if (pos == first_pos)
            return -1;
    }

    return hashmap->entries[pos]->value;
}
