/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

enum { BUCKET_COUNT = 509 };

struct Binding {
    const char *pcKey; 
    void *pvValue; 
    struct Binding *next;
    };

struct HashTable {
    struct Binding *buckets[BUCKET_COUNT];
};

struct SymTable {
    struct HashTable *psFirst;
    size_t bindingsCount;
    size_t uBucketCount;
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
    oSymTable->psFirst = (struct HashTable *) calloc(1, sizeof(struct HashTable));
    if (oSymTable->psFirst == NULL) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->bindingsCount = 0;
    oSymTable->uBucketCount = BUCKET_COUNT;
    
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {

}

size_t SymTable_getLength(SymTable_T oSymTable) {

}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                                                  const void *pvValue) {

}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue) {

    }

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){

}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra){
        
    }