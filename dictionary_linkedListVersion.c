// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

FILE *dic = NULL;

// Number of buckets in hash table
const unsigned int N = 26;
void insert(int key, const char *buffer);

// Hash table
node *table[26] = {NULL};

// Returns true if word is in dictionary else false
bool check(const char *wordToCheck)
{
    //check which bucket we should look in, based on first char
    int bucketnum = wordToCheck[0] - 97;
    if (isupper(wordToCheck[0]))
    {
        bucketnum = wordToCheck[0] - 65;
    }

    node *tmp = table[bucketnum];
	
    while (tmp != NULL)
    {
        int compareString = strcasecmp(tmp->word, wordToCheck);
        if (compareString == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //return MurmurHash2(word, strlen(word) , 0);
    // assign a number to the first char of word from 0-25
    // return tolower(word[0]) - 'a';
    return true;
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
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */

        // hash the name into a spot
        //  int hashedValue = hash(line);
        int hashedValue = 5;
        
        // insert the name in table with hashed value
        line[0] = tolower(line[0]);
        insert(hashedValue, line);
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
    node *tmp; //the HEAD of search
    node *next; // temp - as we traverse the list
    
    for (int i = 0; i < N; i++) //do this for all 26 buckets
    {
        tmp = table[i];
        if (tmp != NULL)
        {
            next = tmp->next;
            free(tmp);
            
            while (next != NULL)
            {
                tmp = next;
                next = tmp->next;
                free(tmp);
            }
        }
    }

    fclose(dic);
    return true;
}


/*
 * takes a string and inserts it into a linked list at a part of the hash table
 */
void insert(int key, const char *item)
{
    //assign bucket based on first character
    node *new_node = malloc(sizeof(node));

    memset(new_node->word, 0, LENGTH + 1);
    new_node->next = NULL;
    
    strncat(new_node->word, item, LENGTH);
    
    int w;
    for (w = 0; item[w] != '\0'; w++)
    {
        new_node->word[w] = item[w];
    }
    new_node->word[w - 1] = 0; //remove that whitespace char from the end of the word

    //find the bucket num based on lower case ascii character
    int bucketnum = item[0] - 97;
    if (isupper(item[0]))
    {
        bucketnum = item[0] - 65;
    }
    
    if (table[bucketnum] == NULL)
    {
        table[bucketnum] = new_node;
    }
    else
    {
        //create a temp variable for surfing the linked list
        node *tmp = table[bucketnum];
        
        //keep going til we find a node with next=NULL
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        
        //shove the current nodes address into the 'next' of the last node in the linked list
        tmp->next = new_node;
    }
}