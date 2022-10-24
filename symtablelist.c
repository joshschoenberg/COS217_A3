/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

struct SymTableNode {
    const void *key; 
    void *value; 
    struct SymTableNode *next;
    };
struct SymTable {
    struct SymTableNode *first;
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
};

void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->first; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        free(psCurrentNode);
    }
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->symTableSize;
}

//  int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
//                                                   const void *pvValue) {
//     assert(oSymTable != NULL);
//     if (oSymTable->first == NULL) {
//         oSymTable->first = calloc(1, sizeof(*pcKey));

//     }
//     /* Go through oSymTable to check if pcKey already exists */

//     /* If it does, return 0. */

//     /* If it does not, add it in and return 1. */
    
//     /* If insufficient mememory, return 0. */

// }

// void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
//     const void *pvValue) {
//     assert(oSymTable != NULL);

//     }

// int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
//     assert(oSymTable != NULL);
//     assert(pcKey != NULL);

// }

// void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
//     assert(oSymTable != NULL);
//     assert(pcKey != NULL);

// }

// void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
//     assert(oSymTable != NULL);
//     assert(pcKey != NULL);

// }

// void SymTable_map(SymTable_T oSymTable,
//     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
//     const void *pvExtra) {
    
//     assert(oSymTable != NULL);
//     assert(pvExtra != NULL);
    

//     }
