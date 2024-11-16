#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define NUM_ITEMS_A_PRODUCIR 100  // Número de elementos que cada productor debe generar

// Definir el archivo log global
FILE *logFile;

// Función para escribir en el archivo log
void registrarEvento(const char *mensaje) {
    if (logFile) {
        fprintf(logFile, "%s\n", mensaje);
        fflush(logFile);
    }
}

// Estructura para el Monitor que trabaje con cola circular
typedef struct {
    int *buffer;           // Arreglo que representa la cola circular
    int capacidad;         // Tamaño de la cola
    int frente;            // Índice del frente
    int final;             // Índice del final
    int contador;          // Número de elementos en la cola
    pthread_mutex_t mutex; // Mutex para proteger el acceso a la cola
    pthread_cond_t lleno;  // Variable de condición para saber si la cola está llena
    pthread_cond_t vacio;  // Variable de condición para saber si la cola está vacía
    bool terminado;        // Flag que indica si los productores han terminado
} ColaCircular;

// Función para redimensionar la cola
void redimensionarCola(ColaCircular *cola, int nuevaCapacidad) {
    int *nuevoBuffer = malloc(nuevaCapacidad * sizeof(int));
    if (!nuevoBuffer) {
        fprintf(stderr, "Error al redimensionar la cola: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < cola->contador; i++) {
        nuevoBuffer[i] = cola->buffer[(cola->frente + i) % cola->capacidad];
    }

    free(cola->buffer);
    cola->buffer = nuevoBuffer;
    cola->capacidad = nuevaCapacidad;
    cola->frente = 0;
    cola->final = cola->contador - 1;

    // Registrar el cambio de tamaño en el archivo log
    char mensaje[100];
    snprintf(mensaje, sizeof(mensaje), "Cola redimensionada a %d elementos", nuevaCapacidad);
    registrarEvento(mensaje);

    printf("Cola redimensionada a %d\n", nuevaCapacidad);
}

// Función para el productor
void *productor(void *arg) {
    ColaCircular *cola = (ColaCircular *)arg;
    int item;

    // Cada productor producirá un número finito de elementos
    for (int i = 0; i < NUM_ITEMS_A_PRODUCIR; i++) {
        item = rand() % 100;

        pthread_mutex_lock(&cola->mutex);

        // Si la cola está llena, redimensionar
        if (cola->contador == cola->capacidad) {
            redimensionarCola(cola, cola->capacidad * 2);
        }

        // Añadir el elemento a la cola
        cola->final = (cola->final + 1) % cola->capacidad;
        cola->buffer[cola->final] = item;
        cola->contador++;

        pthread_cond_signal(&cola->vacio); // Notifica a los consumidores que hay un nuevo elemento
        pthread_mutex_unlock(&cola->mutex);

        printf("Productor produjo %d\n", item);
        sleep(1);  // Simula el tiempo que toma producir un elemento
    }

    return NULL;  // El productor termina
}

// Función para el consumidor
void *consumidor(void *arg) {
    ColaCircular *cola = (ColaCircular *)arg;
    int item;

    while (true) {
        pthread_mutex_lock(&cola->mutex);

        // Espera a que haya elementos o a que se indique finalización
        while (cola->contador == 0 && !cola->terminado) {
            pthread_cond_wait(&cola->vacio, &cola->mutex);
        }

        // Salir si no quedan elementos y se ha indicado que los productores terminaron
        if (cola->contador == 0 && cola->terminado) {
            pthread_mutex_unlock(&cola->mutex);
            break;
        }

        // Consumir un elemento de la cola
        item = cola->buffer[cola->frente];
        cola->frente = (cola->frente + 1) % cola->capacidad;
        cola->contador--;

        // Si la ocupación baja al 25% y la capacidad es mayor a 1, reducir a la mitad
        if (cola->contador <= cola->capacidad / 4 && cola->capacidad > 1) {
            redimensionarCola(cola, cola->capacidad / 2);
        }

        pthread_cond_signal(&cola->lleno);
        pthread_mutex_unlock(&cola->mutex);

        printf("Consumidor consumió %d\n", item);
        sleep(1);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int numProductores = 0, numConsumidores = 0, capacidad = 0, tiempoEspera = 0;

    // Abrir archivo de log
    logFile = fopen("cola_log.txt", "w");
    if (!logFile) {
        fprintf(stderr, "Error al abrir el archivo de log.\n");
        exit(EXIT_FAILURE);
    }

    // Manejo de la entrada por teclado
    int opt;
    while ((opt = getopt(argc, argv, "p:c:s:t:")) != -1) {
        switch (opt) {
            case 'p': numProductores = atoi(optarg); break;
            case 'c': numConsumidores = atoi(optarg); break;
            case 's': capacidad = atoi(optarg); break;
            case 't': tiempoEspera = atoi(optarg); break;
            default:
                fprintf(stderr, "Uso: %s -p <numProductores> -c <numConsumidores> -s <capacidad> -t <tiempoEspera>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificar que la entrada es válida
    if (numProductores <= 0 || numConsumidores <= 0 || capacidad <= 0 || tiempoEspera <= 0) {
        fprintf(stderr, "Parámetros inválidos. (todo debe ser > 0).\n");
        exit(EXIT_FAILURE);
    }

    // Inicialización de la cola circular
    ColaCircular cola;
    cola.buffer = malloc(capacidad * sizeof(int));
    if (!cola.buffer) {
        fprintf(stderr, "Error al asignar memoria para la cola.\n");
        exit(EXIT_FAILURE);
    }
    cola.capacidad = capacidad;
    cola.frente = 0;
    cola.final = -1;
    cola.contador = 0;
    cola.terminado = false;
    pthread_mutex_init(&cola.mutex, NULL);
    pthread_cond_init(&cola.lleno, NULL);
    pthread_cond_init(&cola.vacio, NULL);

    // Creación de hilos de productores y consumidores
    pthread_t productores[numProductores], consumidores[numConsumidores];

    for (int i = 0; i < numProductores; i++) {
        pthread_create(&productores[i], NULL, productor, &cola);
    }

    for (int i = 0; i < numConsumidores; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, &cola);
    }

    // Espera a que todos los productores terminen
    for (int i = 0; i < numProductores; i++) {
        pthread_join(productores[i], NULL);
    }

    // Señala a los consumidores que deben terminar y espera
    pthread_mutex_lock(&cola.mutex);
    cola.terminado = true;
    pthread_cond_broadcast(&cola.vacio);  // Despierta a todos los consumidores
    pthread_mutex_unlock(&cola.mutex);

    // Espera a que los consumidores terminen
    for (int i = 0; i < numConsumidores; i++) {
        pthread_join(consumidores[i], NULL);
    }

    // Limpieza de recursos
    free(cola.buffer);
    pthread_mutex_destroy(&cola.mutex);
    pthread_cond_destroy(&cola.lleno);
    pthread_cond_destroy(&cola.vacio);

    fclose(logFile);

    printf("Simulación terminada\n");
    return 0;
}