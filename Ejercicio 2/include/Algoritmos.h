#ifndef ALGORITMOS_H
#define ALGORITMOS_H

// Declaraciones de las funciones
int fifoReplacement(int *references, int n, int frames);
int optimalReplacement(int *references, int n, int frames);
int lruReplacement(int *references, int n, int frames);
int lruClockReplacement(int *references, int n, int frames);

#endif