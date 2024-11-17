#include <stdio.h>
#include <stdlib.h>
#include "../include/TablaHash.h"

// Crear una tabla hash
HashTable *createHashTable(int size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->buckets = (Node **)calloc(size, sizeof(Node *));
    table->size = size;
    return table;
}

// Hash function
int hashFunction(int page, int size) {
    return page % size;
}

// Insertar en tabla hash con chaining
void insertHash(HashTable *table, int page) {
    int index = hashFunction(page, table->size);
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->page = page;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;

    printf("Pagina %d insertada en el indice %d\n", page, index); // Debug print
}

// Buscar en tabla hash
int searchHash(HashTable *table, int page) {
    int index = hashFunction(page, table->size);
    Node *current = table->buckets[index];
    while (current) {
        if (current->page == page) {
            printf("Pagina %d encontrada en la tabla hash en el indice %d\n", page, index); // Debug print
            return 1; // Pagina encontrada
        }
        current = current->next;
    }
    printf("Pagina %d no encontrada en la tabla hash\n", page); // Debug print
    return 0; // Pagina no encontrada
}

// Eliminar de tabla hash
void removeHash(HashTable *table, int page) {
    int index = hashFunction(page, table->size);
    Node *current = table->buckets[index];
    Node *prev = NULL;
    while (current) {
        if (current->page == page) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            free(current);
            printf("Pagina %d eliminada del indice %d\n", page, index); // Debug print
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Pagina %d no encontrada para eliminar\n", page); // Debug print
}
