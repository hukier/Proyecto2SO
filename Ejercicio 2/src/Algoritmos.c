#include <stdio.h>
#include <stdlib.h>
#include "../include/Algoritmos.h"

// FIFO reemplazo de paginas
int fifoReplacement(int *references, int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int)); // Array de marcos
    int pageFaults = 0; // Contador de fallos de pagina
    int index = 0; // Indice FIFO
    int count = 0; // Contador de marcos ocupados

    for (int i = 0; i < n; i++) {
        int page = references[i];
        int found = 0;

        // Verificar si la pagina ya esta en los marcos
        for (int j = 0; j < count; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) { // Si no esta en los marcos (fallo de pagina)
            if (count < frames) { // Si aun hay espacio libre
                frame[count++] = page; // Agregar pagina a los marcos
                printf("Pagina %d cargada en el marco %d\n", page, count); // Debug print
            } else { // Si no hay espacio, reemplazar usando FIFO
                frame[index] = page;
                printf("Pagina %d reemplazada en el indice %d\n", page, index); // Debug print
                index = (index + 1) % frames; // Actualizar indice FIFO
            }
            pageFaults++; // Incrementar fallos de pagina
        } else {
            printf("Pagina %d ya esta en memoria\n", page); // Debug print
        }

        // Debug: Mostrar estado de los marcos despues de cada reemplazo
        printf("marco despues del reemplazo (iteracion %d): ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (j < count) {
                printf("%d ", frame[j]);
            } else {
                printf("- "); // Indicar marcos vacios
            }
        }
        printf("\n");

        // Debug: Mostrar el contador de fallos de pagina en cada paso
        printf("fallos: %d\n", pageFaults); // Debug print
    }

    free(frame); // Liberar memoria
    printf("Reemplazo de paginas FIFO finalizado\n"); // Debug print
    return pageFaults;
}

int optimalReplacement(int *references, int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int)); // Array de marcos
    int pageFaults = 0; // Contador de fallos de página
    int count = 0; // Contador de marcos ocupados

    for (int i = 0; i < n; i++) {
        int page = references[i];
        int found = 0;

        // Verificar si la página ya está en los marcos
        for (int j = 0; j < count; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
        }

        // Si no está en los marcos (fallo de página)
        if (!found) {
            if (count < frames) {
                // Si aún hay espacio libre en los marcos
                frame[count++] = page;
            } else {
                // Si no hay espacio, encontrar la página óptima para reemplazar
                int farthestIndex = -1;
                int farthestDistance = -1;

                for (int j = 0; j < frames; j++) {
                    int k;
                    // Buscar en el futuro cuál es la próxima aparición de frame[j]
                    for (k = i + 1; k < n; k++) {
                        if (frame[j] == references[k]) {
                            break; // Encontrada en el futuro
                        }
                    }
                    // Si la página no se usará más o se usará muy tarde
                    if (k > farthestDistance) {
                        farthestDistance = k;
                        farthestIndex = j;
                    }
                }

                // Reemplazar la página en el índice encontrado
                frame[farthestIndex] = page;
            }
            pageFaults++; // Incrementar fallos de página
        }

        // Debug: Mostrar el estado de los marcos después de cada reemplazo
        printf("marco despues del reemplazo (iteracion %d): ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (j < count) {
                printf("%d ", frame[j]);
            } else {
                printf("- "); // Indicar marcos vacíos
            }
        }
        printf("\n");
    }

    free(frame); // Liberar memoria
    printf("Reemplazo de páginas Óptimo finalizado\n"); // Debug print
    return pageFaults;
}

int lruReplacement(int *references, int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int)); // Array de marcos
    int *lastUsed = (int *)malloc(frames * sizeof(int)); // Arreglo para almacenar el tiempo de uso de cada página
    int pageFaults = 0; // Contador de fallos de página
    int count = 0; // Contador de marcos ocupados

    for (int i = 0; i < frames; i++) {
        frame[i] = -1; // Inicializar los marcos como vacíos
        lastUsed[i] = -1; // Inicializar el uso de cada página
    }

    for (int i = 0; i < n; i++) {
        int page = references[i];
        int found = 0;

        // Verificar si la página ya está en los marcos
        for (int j = 0; j < count; j++) {
            if (frame[j] == page) {
                found = 1;
                lastUsed[j] = i; // Actualizar el "tiempo" de uso de la página
                break;
            }
        }

        // Si no está en los marcos (fallo de página)
        if (!found) {
            if (count < frames) {
                // Si aún hay espacio libre en los marcos
                frame[count] = page;
                lastUsed[count] = i;
                count++;
            } else {
                // Si no hay espacio, encontrar la página menos recientemente usada (LRU)
                int lruIndex = 0;
                for (int j = 1; j < frames; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex]) {
                        lruIndex = j;
                    }
                }

                // Reemplazar la página en el índice encontrado
                frame[lruIndex] = page;
                lastUsed[lruIndex] = i;
            }
            pageFaults++; // Incrementar fallos de página
        }

        // Debug: Mostrar el estado de los marcos después de cada reemplazo
        printf("marco despues del reemplazo (iteracion %d): ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (frame[j] != -1) {
                printf("%d ", frame[j]);
            } else {
                printf("- "); // Indicar marcos vacíos
            }
        }
        printf("\n");
    }

    free(frame); // Liberar memoria
    free(lastUsed); // Liberar memoria
    printf("Reemplazo de páginas LRU finalizado\n"); // Debug print
    return pageFaults;
}

int lruClockReplacement(int *references, int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int)); // Array de marcos
    int *useBit = (int *)calloc(frames, sizeof(int)); // Array de bits de uso inicializado a 0
    int pageFaults = 0; // Contador de fallos de página
    int count = 0; // Contador de marcos ocupados
    int pointer = 0; // Puntero para el algoritmo del reloj

    for (int i = 0; i < frames; i++) {
        frame[i] = -1; // Inicializar los marcos como vacíos
    }

    for (int i = 0; i < n; i++) {
        int page = references[i];
        int found = 0;

        // Verificar si la página ya está en los marcos
        for (int j = 0; j < count; j++) {
            if (frame[j] == page) {
                found = 1;
                useBit[j] = 1; // Página encontrada, actualizar el bit de uso
                break;
            }
        }

        // Si no está en los marcos (fallo de página)
        if (!found) {
            pageFaults++; // Incrementar fallos de página

            if (count < frames) {
                // Si aún hay espacio libre en los marcos
                frame[count] = page;
                useBit[count] = 1; // Marcar la página con bit de uso 1
                count++;
            } else {
                // Buscar un marco con bit de uso 0
                while (useBit[pointer] == 1) {
                    useBit[pointer] = 0; // Resetear el bit de uso
                    pointer = (pointer + 1) % frames; // Avanzar el puntero
                }

                // Reemplazar la página en el índice señalado por el puntero
                frame[pointer] = page;
                useBit[pointer] = 1; // Marcar la página con bit de uso 1
                pointer = (pointer + 1) % frames; // Avanzar el puntero
            }
        }

        // Debug: Mostrar el estado de los marcos después de cada reemplazo
        printf("marco despues del reemplazo (iteracion %d): ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (frame[j] != -1) {
                printf("%d ", frame[j]);
            } else {
                printf("- "); // Indicar marcos vacíos
            }
        }
        printf("\n");
    }

    free(frame); // Liberar memoria
    free(useBit); // Liberar memoria
    printf("Reemplazo de páginas LRU con Reloj finalizado\n"); // Debug print
    return pageFaults;
}
