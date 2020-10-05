// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>

#include "dictionary.h"

uint32_t qhashmurmur3_32(const char *data, size_t nbytes);

FILE *dic = NULL;

// Number of buckets in hash table
const unsigned int N = 143093;  //Chose the next prime number up from the max words in large dictionary (which was 143091)
void insert(int key, char *buffer);

// Hash table
char *table[N] = {NULL};

// Returns true if word is in dictionary else false
bool check(const char *wordToCheck)
{
    char *lowerwordToCheck = malloc(strlen(wordToCheck) + 1);
    memset(lowerwordToCheck, '0', strlen(wordToCheck) + 1);

    if (lowerwordToCheck != NULL)
    {
        strcpy(lowerwordToCheck, wordToCheck);

        for (int i = 0; i < strlen(lowerwordToCheck); i++)
        {
            lowerwordToCheck[i] = tolower(lowerwordToCheck[i]);
        }
        lowerwordToCheck[strlen(lowerwordToCheck)] = '\0';

        int tableaddr = hash(lowerwordToCheck);

        const char *wordinarray = table[tableaddr];

        if (wordinarray == NULL)
        {
            free(lowerwordToCheck);
            lowerwordToCheck = NULL;
            return false;
        }

        int compareString = strcasecmp(wordinarray, lowerwordToCheck);

        if (compareString == 0)
        {
            free(lowerwordToCheck);
            lowerwordToCheck = NULL;
            return true;
        }
    }
    free(lowerwordToCheck);
    lowerwordToCheck = NULL;
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    uint32_t hashedValue = qhashmurmur3_32(word, strlen(word));

    //shorten the hash to fit in our defined buckets.
    int shorthash = hashedValue % 143093;

    return shorthash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    dic = fopen(dictionary, "r");

    //read strings from file one at a time
    char line[256];

    while (fgets(line, sizeof(line), dic))
    {
        //ensure first char is lowercase
        line[0] = tolower(line[0]);

        //count word length
        int len = strcspn(line, "\n");

        //remove new line char
        line[len] = 0;

        int debuglen = len + 1;

        char *newline = malloc(len + 1);
        memset(newline, '0', len + 1);

        //strcpy(newline, line); not sure if we can use strcpy cos of the length of line. 46 chars vs newline which is the correct length of the word
        if (newline)
        {
            for (int i = 0; i < len; i++)
            {
                newline[i] = line[i];
            }
            newline[len] = '\0';

            //perform the magic hash
            int addr = hash(newline);
    
            // insert the name in table with hashed value
            insert(addr, newline);
        }
        else
        {
            return false;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int sz = 0;
    if (dic != NULL)
    {
        rewind(dic);

        char line[256];
        while (fgets(line, sizeof(line), dic))
        {
            if (sizeof(line) > 0)
            {
                sz++;
            }
        }
        return sz;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    char *tmp;  //the HEAD of search
    
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            tmp = table[i];
            free(tmp);
            tmp = NULL;
        }
    }
    rewind(dic);
    fclose(dic);
    return true;
}

void insert(int key, char *item)
{
    table[key] = item;
}

//murmurhash credit to Austin Appleby
uint32_t qhashmurmur3_32(const char *data, size_t nbytes)
{
    if (data == NULL || nbytes == 0)
    {
        return 0;
    }

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = nbytes / 4;
    const uint32_t *blocks = (const uint32_t *)(data);
    const uint8_t *tail = (const uint8_t *)(data + (nblocks * 4));

    uint32_t h = 0;

    int i;
    uint32_t k;
    for (i = 0; i < nblocks; i++) 
    {
        k = blocks[i];

        k *= c1;
        k = (k << 15) | (k >> (32 - 15));
        k *= c2;

        h ^= k;
        h = (h << 13) | (h >> (32 - 13));
        h = (h * 5) + 0xe6546b64;
    }

    k = 0;
    switch (nbytes & 3) 
    {
        case 3:
            k ^= tail[2] << 16;
        case 2:
            k ^= tail[1] << 8;
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << 15) | (k >> (32 - 15));
            k *= c2;
            h ^= k;
    };

    h ^= nbytes;

    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}