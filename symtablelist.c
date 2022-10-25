/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

struct SymTableNode {
    const char *pcKey; 
    void *pvValue; 
    struct SymTableNode *next;
    };
struct SymTable {
    struct SymTableNode *psFirstNode;
    size_t symTableSize;
    };

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T) calloc(1, sizeof(struct SymTable));
    if(oSymTable == NULL) {
        return NULL;
        }
    oSymTable->symTableSize = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        free(psCurrentNode);
    }
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->symTableSize;
}

 int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                                                  const void *pvValue) {
    assert(oSymTable != NULL);
    if (oSymTable->psFirstNode == NULL) {
        oSymTable->psFirstNode = calloc(1, sizeof(*pcKey));

    }
    /* Go through oSymTable to check if pcKey already exists */
    if (SymTable_contains(oSymTable, pcKey)) {
        /* If it does, return 0. */
        return 0;
    }
    
    /* If it does not, add it in and return 1. */
        struct SymTableNode *psNewNode;
        psNewNode = (struct SymTable*)calloc(1, sizeof(struct SymTable));
        
        /* If insufficient mememory, return 0. */
        if (psNewNode == NULL)
            return 0;
        
        psNewNode->pcKey = pcKey;
        psNewNode->next = oSymTable->psFirstNode;
        oSymTable->psFirstNode = psNewNode;

    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue) {
    assert(oSymTable != NULL);
    return (void*) pcKey;
    }

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    /* If Symbol table contains pcKey, return 1 */

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        if (psCurrentNode->pcKey == *pcKey) /* Is that the right syntax?? */
            return 1;
}

    /* If Symbol table does not contain pcKey, return 0 */
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    return (void*) pcKey;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    return (void*) pcKey;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    
    assert(oSymTable != NULL);
    assert(pvExtra != NULL);
    

    }
