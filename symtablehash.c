/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

SymTable_T SymTable_new(void) {
    struct Node {
        const char *key;
        int value;
        struct Node *next;
    };
    struct List {
        struct Node *first;
    };
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