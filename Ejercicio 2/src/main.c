#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/TablaHash.h"
#include "../include/Algoritmos.h"

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Uso: %s -m <marcos> -a <algoritmo> -f <archivo>\n", argv[0]);
        return 1;
    }

    int frames = 0;
    char algorithm[10];
    char filename[100];

    // Leer argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            frames = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            strcpy(algorithm, argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            strcpy(filename, argv[++i]);
        }
    }

    printf("Marcos: %d, Algoritmo: %s, Archivo: %s\n", frames, algorithm, filename); // Debug print

    // Leer archivo de referencias
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir el archivo: %s\n", filename);
        return 1;
    }

    int references[1000], n = 0;
    while (fscanf(file, "%d", &references[n]) != EOF) {
        n++;
    }
    fclose(file);

    printf("Leidas %d referencias del archivo\n", n); // Debug print

    // Ejecutar algoritmo
    int pageFaults = 0;
    if (strcmp(algorithm, "FIFO") == 0) {
        pageFaults = fifoReplacement(references, n, frames);
    } else if (strcmp(algorithm, "OPTIMO") == 0) {
    pageFaults = optimalReplacement(references, n, frames);
    } else if (strcmp(algorithm, "LRU") == 0) {
    pageFaults = lruReplacement(references, n, frames);
    } else if (strcmp(algorithm, "LRU_Reloj") == 0) {
    pageFaults = lruClockReplacement(references, n, frames);
    } else {
            printf("Algoritmo no implementado: %s\n", algorithm);
            return 1;
            }

        printf("Numero de fallos de pagina: %d\n", pageFaults);
        return 0;
}
