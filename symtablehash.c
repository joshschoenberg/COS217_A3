/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"
#include <string.h>

/* auBucketCounts[] is an array of different bucket counts */
static const size_t auBucketCounts[] = {509, 1021, 2039, 4093, 8191, 
                                                   16381, 32749, 65521};

/* Each key and value is stored in a SymTableBinding. SymTableBindings
are linked to form a list. */
struct SymTableBinding {
    /* The key */
    const char *pcKey; 
    /* The value */
    void *pvValue; 
    /* A pointer to the next SymTableBinding */
    struct SymTableBinding *next;
    };

/* A SymTable contains a pointer to the first bucket (of the array of 
buckets) in the hash table, the total number of bindings, and the number 
of buckets */
struct SymTable {
    /* The number of bindings */
    size_t bindingsCount;
    /* The number of buckets in the hash table */
    size_t uBucketCount;
    /* A pointer to the first bucket of the hash table */
    struct SymTableBinding **buckets;
    };

/*--------------------------------------------------------------------*/

/* SymTable_hash returns a hash code for pcKey that is between 0 and 
uBucketCount-1, inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
} 

/*--------------------------------------------------------------------*/

/* SymTable_expand expands SymTable_T oSymTable to have a hash table 
with a bucket size of the next bucket size in auBucketCounts[], and it 
places each node in the correct bucket of the new hash table. If the 
current bucket size is the last one in auBucketCounts[] SymTable_expand 
does not change oSymTable */ 

static void SymTable_expand(SymTable_T oSymTable) {
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;
    struct SymTableBinding **newBuckets;
    size_t auBucketCountsIndex;
    size_t oldSymTableBucketIndex;
    size_t newBucketCount;
    size_t newSymTableBucketIndex;
    /* Defines the number of bucket counts */
    size_t numBucketCounts = 
        sizeof(auBucketCounts)/sizeof(auBucketCounts[0]);
    auBucketCountsIndex = 0;
    while (auBucketCountsIndex < numBucketCounts) {
        /* If it's the last one, do not change the sym table */
        if (auBucketCountsIndex == numBucketCounts - 1)
            return;
        /* Set the newBucketCount to be the next one */
        if (auBucketCounts[auBucketCountsIndex] == 
                oSymTable->uBucketCount) {
            break;
        }
        auBucketCountsIndex++;
    }
    newBucketCount = auBucketCounts[auBucketCountsIndex + 1];

    /* Create a new hash table, which is a copy of the old one, but with  
    a new size. The SymTable is still the same, though. But, the 
    bindings are in the correct bucket */
    newBuckets = (struct SymTableBinding **) calloc(newBucketCount, 
                        sizeof(struct SymTableBinding *));
    if(newBuckets == NULL) {
        return;
        }
        
    /* Add in all of the buckets to new the correct spot */
    /* Go through each bucket, and determine what the new bucket is */
    oldSymTableBucketIndex = 0;
    while(oldSymTableBucketIndex < oSymTable->uBucketCount) {
        /* If bucket is empty, move to the next bucket. Otherwise, go 
                                                       through bucket */
        for (psCurrentBinding = oSymTable->buckets[oldSymTableBucketIndex]; 
            psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {
            /* Keep track of next binding */
            psNextBinding = psCurrentBinding->next;
            /* Set current binding's next to be first binding in bucket */
            newSymTableBucketIndex = 
                SymTable_hash(psCurrentBinding->pcKey, newBucketCount);
            psCurrentBinding->next = newBuckets[newSymTableBucketIndex]; 
            /* Set first binding of bucket to be current binding */
            newBuckets[newSymTableBucketIndex] = psCurrentBinding;
        }
        oldSymTableBucketIndex++;

    }
    /* Free the old hash table's buckets */ 
    free(oSymTable->buckets);
    /* Set oSymTable's buckets be the new hashtable */
    oSymTable->buckets = newBuckets;
    oSymTable->uBucketCount = newBucketCount;
    return;
}

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T) calloc(1, sizeof(struct SymTable));
    if(oSymTable == NULL) {
        return NULL;
        }
    /* Allocate space for the array of buckets, with the number of 
    buckets being 509 (auBucketCounts[0]) */
    oSymTable->buckets = (struct SymTableBinding**) calloc(auBucketCounts[0], 
                                     sizeof(struct SymTableBinding *));
    if (oSymTable->buckets == NULL) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->bindingsCount = 0;
    oSymTable->uBucketCount = auBucketCounts[0];
    
    return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable) {
    size_t bucketCountIndex;
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;

    assert(oSymTable != NULL);
    bucketCountIndex = 0;
    /* Go through every bucket */
    while (bucketCountIndex < oSymTable->uBucketCount) {
        /* Go through every binding within each bucket, freeing each one */
        for (psCurrentBinding = oSymTable->buckets[bucketCountIndex]; 
                psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {
        psNextBinding = psCurrentBinding->next;
        free((char *) psCurrentBinding->pcKey); 
        free(psCurrentBinding);
    }
    bucketCountIndex++;
    }
    /* Free the array of buckets */
    free(oSymTable->buckets);
    /* Free the SymTable */
    free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    /* Return the number of bindings in oSymTable */
    return oSymTable->bindingsCount;
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                                                  const void *pvValue) {
    size_t uBucketIndex;
    struct SymTableBinding *psNewBinding;
    char *pcKeyCopy;
    
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Go through oSymTable to check if pcKey already exists */
    if (SymTable_contains(oSymTable, pcKey)) {
        /* If it does, return 0. */
        return 0;
    }

    /* Put key and value in correct bucket */                                                
    uBucketIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);

    psNewBinding = (struct SymTableBinding*)calloc(1, 
                                        sizeof(struct SymTableBinding)); 
    /* If insufficient mememory, return 0. */
    if (psNewBinding == NULL)
        return 0;

    /* Make copy of pcKey NEW ADDITION! */
    pcKeyCopy = (char *) malloc(strlen(pcKey) + 1);
    if (pcKeyCopy == NULL) {
        free(psNewBinding);
        return 0;
    }
    strcpy(pcKeyCopy, pcKey);

    /* Use that copy to put the key and value into the symbol table */
    psNewBinding->pcKey = pcKeyCopy;
    psNewBinding->pvValue = (void *) pvValue;
    /* IS THIS CORRECT?? */
    psNewBinding->next = oSymTable->buckets[uBucketIndex];
    oSymTable->buckets[uBucketIndex] = psNewBinding;

    /* Update oSymTable size. */
    oSymTable->bindingsCount += 1;

    /* Expand the number of buckets, if necessary. */
    if ((oSymTable->bindingsCount) > (oSymTable->uBucketCount)) {
        SymTable_expand(oSymTable);
    } 
    /* When SymTable_put is successful, return 1 */ 
    return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue) {
    
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;
    size_t uBucketIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Find pcKey and replace its value with pvValue */
    uBucketIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);
    for (psCurrentBinding = oSymTable->buckets[uBucketIndex]; 
            psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {

        psNextBinding = psCurrentBinding->next;
        if ((strcmp(psCurrentBinding->pcKey, pcKey)) == 0) {
            void *oldValue = psCurrentBinding->pvValue;
            psCurrentBinding->pvValue = (void *) pvValue;
            return oldValue;
        }
    }
    /* If oSymTable does not contain pcKey, return NULL. */
    return NULL;

    }

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    size_t uBucketsIndex;
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* If Symbol table contains pcKey, return 1. */
    uBucketsIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);
    for (psCurrentBinding = oSymTable->buckets[uBucketsIndex]; 
            psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {
        psNextBinding = psCurrentBinding->next;
        if ((strcmp(psCurrentBinding->pcKey, pcKey)) == 0) 
            return 1;
}
    /* If table does not contain pcKey, return 0 */
    return 0;
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;
    size_t uBucketIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /* Search appropriate bucket for the binding */
    uBucketIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);
    for (psCurrentBinding = oSymTable->buckets[uBucketIndex]; 
                psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {
        psNextBinding = psCurrentBinding->next;
        if ((strcmp(psCurrentBinding->pcKey, pcKey)) == 0) {
            return psCurrentBinding->pvValue;
        }
    }
    /* If key is not in symbol table, return NULL. */
    return NULL;
}
/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;
    struct SymTableBinding *psPreviousBinding;
    size_t uBucketIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* If symbol table is empty, return NULL */
    if (oSymTable->buckets == NULL) 
        return NULL;
    /* If bucket is empty, return NULL */
    uBucketIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);
    if (oSymTable->buckets[uBucketIndex] == NULL)
        return NULL;
    /* If first binding contains pcKey, remove first binding and replace 
    with second */
    if (strcmp(oSymTable->buckets[uBucketIndex]->pcKey, pcKey) == 0) {
        void * oldValue = oSymTable->buckets[uBucketIndex]->pvValue;
        psNextBinding = oSymTable->buckets[uBucketIndex]->next;
        free((char *) oSymTable->buckets[uBucketIndex]->pcKey);
        free(oSymTable->buckets[uBucketIndex]);
        oSymTable->buckets[uBucketIndex] = psNextBinding;
        oSymTable->bindingsCount -= 1;
        return oldValue;
    }
    /* If other binding contains pcKey, remove that binding */
    psPreviousBinding = oSymTable->buckets[uBucketIndex];
    for (psCurrentBinding = oSymTable->buckets[uBucketIndex]->next; 
                psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {
        psNextBinding = psCurrentBinding->next;
        if ((strcmp(psCurrentBinding->pcKey, pcKey)) == 0) {      
            void * oldValue = psCurrentBinding->pvValue;
            /* Set the previous binding's next to be the removed 
                                                       binding's next */
            psPreviousBinding->next = psNextBinding;
            free((char *) psCurrentBinding->pcKey);
            free(psCurrentBinding);
            oSymTable->bindingsCount -= 1;
            return oldValue;
        } 
        psPreviousBinding = psCurrentBinding;
    }
    /* If pcKey does not apear in Symbol Table, return NULL. */
    return NULL;
}
/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra){

    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;
    size_t bucketCountIndex;

    assert(oSymTable != NULL);
    assert(*pfApply != NULL);

    bucketCountIndex = 0;
    /* Go through every bucket */
    while (bucketCountIndex < oSymTable->uBucketCount) {
        /* Go through every binding within each bucket, applying pfApply 
            to each one */
        for (psCurrentBinding = oSymTable->buckets[bucketCountIndex]; 
                psCurrentBinding != NULL; psCurrentBinding = psNextBinding) {

        psNextBinding = psCurrentBinding->next;
        (*pfApply) (psCurrentBinding->pcKey, psCurrentBinding->pvValue, (void *) pvExtra);
        }
        bucketCountIndex++;
    }
}

