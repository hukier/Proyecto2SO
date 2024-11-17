#ifndef TABLA_HASH_H
#define TABLA_HASH_H

// Nodo para implementar chaining en la tabla hash
typedef struct Node {
    int page;
    struct Node *next;
} Node;

// Tabla de paginas (hash table)
typedef struct {
    Node **buckets;
    int size;
} HashTable;

// Declaraciones de las funciones
HashTable *createHashTable(int size);
int hashFunction(int page, int size);
void insertHash(HashTable *table, int page);
int searchHash(HashTable *table, int page);
void removeHash(HashTable *table, int page);

#endif
