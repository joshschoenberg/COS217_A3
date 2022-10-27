/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

static const size_t auBucketCounts[] = {509, 1021, 2039, 4093, 8191, 
                                                   16381, 32749, 65521};

struct SymTableBinding {
    const char *pcKey; 
    void *pvValue; 
    struct SymTableBinding *next;
    };

struct SymTable {
    size_t bindingsCount;
    size_t uBucketCount;
    struct SymTableBinding **buckets;
    };

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

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

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T) calloc(1, sizeof(struct SymTable));
    if(oSymTable == NULL) {
        return NULL;
        }
    /* HOW CAN WE GENERALIZE auBucketCounts to create a new hash table of any size?? */
    oSymTable->buckets = (struct SymTableBinding**) calloc(auBucketCounts[0] , sizeof(struct SymTableBinding *)) ;
    if (oSymTable->buckets == NULL) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->bindingsCount = 0;
    oSymTable->uBucketCount = auBucketCounts[0];
    
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    size_t bucketCountIndex;
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;

    assert(oSymTable != NULL);
    bucketCountIndex = 0;
    /* Go through every BUCKET */
    while (bucketCountIndex < oSymTable->uBucketCount) {
        /* Go through every node within each bucket, freeing each one */
        for (psCurrentBinding = oSymTable->buckets[bucketCountIndex]; psCurrentBinding != NULL; 
                                           psCurrentBinding = psNextBinding) {
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

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->bindingsCount;
}

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
    uBucketIndex = SymTable_hash(pcKey, oSymTable->uBucketCount)
    
    return 0;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue) {
return (void *) pcKey;
    }

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    size_t uBucketsIndex;
    struct SymTableBinding *psCurrentBinding;
    struct SymTableBinding *psNextBinding;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* If Symbol table contains pcKey, return 1. */
    uBucketsIndex = SymTable_hash(pcKey, oSymTable->uBucketCount);
    for (psCurrentBinding = oSymTable->buckets[uBucketsIndex]; psCurrentBinding != NULL; 
                                           psCurrentBinding = psNextBinding) {
        psNextBinding = psCurrentBinding->next;
        if ((strcmp(psCurrentBinding->pcKey, pcKey)) == 0) 
            return 1;
}
    /* If table does not contain pcKey, return 0 */
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    return (void *) pcKey;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    return (void *) pcKey;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra){
        
    }

/* RESIZE HASHTABLE! */
