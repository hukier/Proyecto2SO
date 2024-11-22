# Proyecto2SO
Proyecto N°2 de Sistemas Operativos

## PARTE 1
Este proyecto implementa un simulador de productores y consumidores utilizando hilos. Permite configurar el número de productores, consumidores, el tamaño de la cola y el tiempo de espera máximo entre las producciones y los consumos.
### Compilación
```sh
gcc -o simulapc main.c -lpthread
```
### Ejecución
```sh
./simulapc -p <productores> -c <consumidores> -s <capacidad> -t <tiempo>
```
---
## PARTE 2
Este proyecto implementa algoritmos de reemplazo de páginas para la gestión de memoria en los sistemas operativos. Los algoritmos implementados son FIFO, Óptimo, LRU y LRU con reloj.
### Compilación 
```sh
gcc -o reemplazopaginas Ejercicio\ 2/src/main.c Ejercicio\ 2/src/Algoritmos.c Ejercicio\ 2/src/TablaHash.c
```
### Ejecución
```sh
./reemplazopaginas -m <marcos> -a <algoritmo> -f <archivo>
```
algunos ejemplos de ejecucion pueden ser
```sh
./pagereplacement -m 3 -a FIFO -f Ejercicio\ 2/referencias/referencia.txt
./pagereplacement -m 4 -a OPTIMO -f Ejercicio\ 2/referencias/referencia.txt
./pagereplacement -m 5 -a LRU -f Ejercicio\ 2/referencias/referencia.txt
./pagereplacement -m 3 -a LRU_Reloj -f Ejercicio\ 2/referencias/referencia.txt
```