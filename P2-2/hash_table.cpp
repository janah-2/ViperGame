// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

/*
Student Name: Janah Abu Hassan
Date: 11/01/2022
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
    e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
    e.g. "_HashTable", "_HashTableEntry"

    Recall that in C, we have to type "struct" together with the name of the struct
    in order to initialize a new variable. To avoid this, in hash_table.h
    we use typedef to provide new "nicknames" for "struct _HashTable" and
    "struct _HashTableEntry". As a result, we can create new struct variables
    by just using:
        - "HashTable myNewTable;"
    or
        - "HashTableEntry myNewHashTableEntry;"
    
    The preceding underscore "_" simply provides a distinction between the names
    of the actual struct defition and the "nicknames" that we use to initialize
    new structs.
    [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
    the first letter is lower-case.
    e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
        It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
    as a distinction between function local variables and struct members.
    e.g. "num_buckets" is a member of "HashTable".
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    HashTableEntry* entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));    // creates new entry
    (entry -> key) = key;                                                       // set key of new entry = key parameter
    (entry -> value) = value;                                                   // set value of new entry = value parameter
    (entry -> next) = NULL;                                                     // set next pointer to null
    return entry;                                                               // return entry created
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    int indexOfBucket = hashTable -> hash(key);                                   // get index of the bucket to look for key
    HashTableEntry *currentNode = hashTable -> buckets[indexOfBucket];            // set current node to head of that bucket 

    while (currentNode) {                                                         // loop through the linked list while current node exists
        if (currentNode -> key == key) {                                          // check if key of current node == key we are looking for 
            return currentNode;                                                   // if yes, return that node
        }
        currentNode = currentNode -> next;                                        // if not equal, go to next node to check it
    }
    return currentNode;                                                           // return the current node (the entry)
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    for (int i = 0; i < (hashTable -> num_buckets); i++) {                       // 1. Loop through each bucket of the hash table to remove all items.
        if (hashTable -> buckets[i]) {
            HashTableEntry* currentNode = hashTable -> buckets[i];              
            HashTableEntry *temp = hashTable -> buckets[i];                     // 1a. set temp to be the first entry of the ith bucket
            // 1b. delete all entries and free buckets
            while (currentNode) {
                temp = currentNode -> next;                                     // store next node
                free(currentNode -> value);                                     // free current node
                free(currentNode);                                              // free current node
                currentNode = temp;                                             // point to next node
            }
        }
    }
    free(hashTable -> buckets);                                                 // free buckets
    // 3. Free hash table
    free(hashTable);
    return;
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    int indexOfBucket = hashTable -> hash(key);                                 // get index of bucket

    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry *present = findItem(hashTable,key);
    //2. If the key is present in the hash table, store new value and return old value
    if(present) {
        void *oldValue = present -> value;                            // get current value of node
        present -> value = value;                                               // change value of current node
        return oldValue;                                                        // return old value of node
    }

    //3. If not, create entry for new value and return NULL
    HashTableEntry *currentNode = createHashTableEntry(key,value);              //create entry

    if(!currentNode) return NULL;                                               // if malloc fails in createHashTableEntry, return NULL

    (currentNode -> next) = hashTable -> buckets[indexOfBucket];                // let current node point to head of bucket
    (hashTable -> buckets[indexOfBucket]) = currentNode;                        // make the head of bucket to be currentNode (adding node)
    return NULL;                                                                // return NULL
}

void *getItem(HashTable *hashTable, unsigned int key)
{
    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry* present = findItem(hashTable,key);  

    //2. If the key exist, return the value
    if (present) {
        return present -> value;
    }
     //3. If not, just return NULL
    return NULL;
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
    //Get the bucket number and the head entry
    int indexOfBucket = hashTable -> hash(key);
    HashTableEntry *currentNode = hashTable -> buckets[indexOfBucket];

    //If the head holds the key, change the head to the next value, and return the old value
    if (currentNode && (currentNode -> key) == key) {
        void* removedItem = (currentNode -> value);                               // save value of current node to be removed
        (hashTable -> buckets[indexOfBucket]) = (currentNode -> next);            // set current head of bucket to next node
        free(currentNode);                                                        // free current node
        return removedItem;
    }

     //If not the head, search for the key to be removed 
    while(currentNode && (currentNode -> next)) {                                 // while current node and next node exist
        if((currentNode -> next -> key) == key) {                                 // check if next key is equal to key we are looking for
            //Unlink node from the list and return old value 
            HashTableEntry* nodeToBeRemoved = (currentNode -> next);              // if yes, save the node to be removed
            void* removedItem = (nodeToBeRemoved -> value);                       // store value of the node to be removed
            (currentNode -> next) = (currentNode -> next -> next);                // node.next points to node.next.next instead which skips the node to be removed
            free(nodeToBeRemoved);                                                // free node that was removed
            return removedItem;
        }
        currentNode = currentNode -> next;                                        
}
    //If the key is not present in the list, return NULL
    return NULL;
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    int indexOfBucket = (hashTable -> hash(key));                                   // get the bucket
    HashTableEntry *currentNode = (hashTable -> buckets[indexOfBucket]);            // get the head of the bucket
    if (findItem(hashTable, key) == NULL){                                          // if item not in list, return NULL
        return;
    }

    if (currentNode && (currentNode -> key) == key) {                               // if key is found
        (hashTable -> buckets[indexOfBucket]) = (currentNode -> next);              // update the head of the bucket to next node
        free(currentNode -> value);                                                 // free value of the node
        free(currentNode);                                                          // free the node
        return;
    }

    while(currentNode && (currentNode -> next)) {                                   // if node and next node exist and the node to be deleted is not head
        if((currentNode -> next -> key) == key) {                                   // check if next key is the key we are looking for
            HashTableEntry* nodeToBeDeleted = currentNode -> next;                  // save the node that is to get deleted
            (currentNode -> next) = (currentNode -> next -> next);                  // node.next points to node.next.next instead which skips the node to be removed
            free(nodeToBeDeleted -> value);                                         // free the value of the node
            free(nodeToBeDeleted);                                                  // free the node
            return;
        }
        currentNode = currentNode -> next;                                          // get next entry and check again if that is the one to be deleted
    }
}
