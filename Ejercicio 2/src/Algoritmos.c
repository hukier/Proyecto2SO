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
