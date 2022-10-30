/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Josh Schoenberg                                            */
/*--------------------------------------------------------------------*/
#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED
#include <stddef.h>

/* A SymTable_T object is an unordered collection of bindings,
containing a key and value pair. */
typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Returns a new SymTable object that contains no bindings, or NULL if 
insufficient memory is available. */ 
SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Frees all memory occupied by oSymTable */
void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Returns the number of bindings in the SymTable object oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* If SymTable object oSymTable does not contain a binding with key 
pcKey, add a new binding to oSymTable consisting of key pcKey and value 
pvValue, and return 1. 
Otherwise (or if insufficient memory is available), leave oSymTable 
unchanged and return 0. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
    const void *pvValue);

/*--------------------------------------------------------------------*/

/* If SymTable oSymTable contains binding with key pcKey, replace  
binding's value with pvValue and return the old value. 
Otherwise, leave oSymTable unchanged and return NULL. */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
    const void *pvValue);

/*--------------------------------------------------------------------*/

/* Returns 1 if SymTable oSymTable contains a binding whose key is 
pcKey, and 0 otherwise.*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Returns the value of the binding within SymTable oSymTable whose key 
is pcKey, or NULL if no such binding exists. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* If SymTable oSymTable contains a binding with key pcKey, remove that 
binding from oSymTable and return the binding's value. 
Otherwise, leave oSymTable unchanged and return NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each binding in Symtable oSymTable, 
passing pvExtrav as an extra parameter. */
void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

#endif