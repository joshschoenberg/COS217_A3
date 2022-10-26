/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"
#include <string.h>

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
        free(psCurrentNode->pcKey); 
        free(psCurrentNode);
    }
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->symTableSize;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                                                  const void *pvValue) {
    
    struct SymTableNode *psNewNode;

    char *pcKeyCopy;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* Go through oSymTable to check if pcKey already exists */
    if (SymTable_contains(oSymTable, pcKey)) {
        /* If it does, return 0. */
        return 0;
    }
    
    /* If it does not, add it in and return 1. */
    psNewNode = (struct SymTableNode*)calloc(1, sizeof(struct SymTableNode)); 

    /* REMOVE? psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode)); */

    /* If insufficient mememory, return 0. */
    if (psNewNode == NULL)
        return 0;

    /* Make copy of pcKey NEW ADDITION! */
    pcKeyCopy = (const char *) malloc(strlen(pcKey) + 1);
    if (pcKeyCopy == NULL) {
        free(psNewNode);
        return 0;
    }
    strcpy(pcKeyCopy, pcKey);

    /* Use that copy to put the key and value into the symbol table */
    psNewNode->pcKey = pcKeyCopy;
    psNewNode->pvValue = (void *) pvValue;
    psNewNode->next = oSymTable->psFirstNode;
    oSymTable->psFirstNode = psNewNode;

    /* Update oSymTable size. */
    oSymTable->symTableSize += 1;

    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue) {

    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    /* If oSymTable does not contain pcKey, return NULL.
    Otherwise, find pcKey and replace its value with pvValue */
    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        if ((strcmp(psCurrentNode->pcKey, pcKey)) == 0) {
            void *oldValue = psCurrentNode->pvValue;
            psCurrentNode->pvValue = (void *) pvValue;
            return oldValue;
        }
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* If Symbol table contains pcKey, return 1 */

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        if ((strcmp(psCurrentNode->pcKey, pcKey)) == 0) 
            return 1;
}

    /* If Symbol table does not contain pcKey, return 0 */
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        if ((strcmp(psCurrentNode->pcKey, pcKey)) == 0) {
            return psCurrentNode->pvValue;
        }
    }
    /* If key is not in symbol table, return NULL. */
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
   
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    struct SymTableNode *psPreviousNode;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /* If symbol table is empty, return NULL */
    if (oSymTable->psFirstNode == NULL) 
        return NULL;
    /* If first node contains pcKey, remove first node and replace 
    with second */
    if (strcmp(oSymTable->psFirstNode->pcKey, pcKey) == 0) {
        void * oldValue = oSymTable->psFirstNode->pvValue;
        psNextNode = oSymTable->psFirstNode->next;
        free(oSymTable->psFirstNode->pcKey);
        free(oSymTable->psFirstNode);
        oSymTable->psFirstNode = psNextNode;
        oSymTable->symTableSize -= 1;
        return oldValue;
        
    }
    /* If other node contains pcKey, remove that node */
    psPreviousNode = NULL;
    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        if ((strcmp(psCurrentNode->pcKey, pcKey)) == 0) {      
            void * oldValue = psCurrentNode->pvValue;
            psPreviousNode->next = psNextNode;
            free(psCurrentNode->pcKey);
            free(psCurrentNode);
            oSymTable->symTableSize -= 1;
            return oldValue;
        } /* THIS CHUNK CONTAINS SEGFAULT! */
        psPreviousNode = psCurrentNode;
    }
    return NULL;
}
void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {

    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
                                           psCurrentNode = psNextNode) {
        psNextNode = psCurrentNode->next;
        (*pfApply) (psCurrentNode->pcKey, psCurrentNode->pvValue, (void *) pvExtra);
    }
}

/* int  main (void) {

} */