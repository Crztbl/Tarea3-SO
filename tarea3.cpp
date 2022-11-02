#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sthread.h"

int o = 0;

int **matriz1, **matriz2, **matriz3;

void multiplicacion(int tamano){

  int fila = o / tamano;
  int columna = o % tamano;
  for(int n = 0; n<tamano; n++){
    *(*(matriz3+fila)+columna) += *(*(matriz1+fila)+n) * *(*(matriz2+n)+columna);
  }
  sthread_exit(0);

}


int main(int argc, char **argv)
{
    
  //abrir el archivo con las matrices
  FILE *fp;
  fp = fopen("matrices-n.txt", "r");

  //CHEQUEO ERROR AL ABRIR ARCHIVO
  if(fp == NULL){
    printf("No se puede abrir el archivo\n");
  }


  //obtencion del tamaño de las matrices
  char tamano_char[10], *linea;
  fgets(tamano_char, sizeof(tamano_char), fp);
  int tamano = atoi(tamano_char);

  //creación de las matrices con el tamaño adecuado
  matriz1 = new int*[tamano]; //reservando memoria para las filas
  for(int i = 0; i<tamano;i++){
    matriz1[i] = new int[tamano]; //reservando memoria para las columnas
  }

  matriz2 = new int*[tamano]; //reservando memoria para las filas
  for(int i = 0; i<tamano;i++){
    matriz2[i] = new int[tamano]; //reservando memoria para las columnas
  }

  matriz3 = new int*[tamano]; //reservando memoria para las filas
  for(int i = 0; i<tamano;i++){
    matriz3[i] = new int[tamano]; //reservando memoria para las columnas
  }

  printf("El tamaño de las matrices es: %d\n", tamano);

  int fila = 0;
  
  //transformar las matrices del .txt a un arreglo bidimensional
  size_t len=0;
  while(getline(&linea, &len, fp)!=-1){
    for(int i = 0; i < tamano; i++){
      if(fila < tamano){
        *(*(matriz1+fila)+i) = (int)(linea[i] - '0');
      }
      else if(fila >= tamano){
        *(*(matriz2+fila-tamano)+i) = (int)(linea[i] - '0');
      }
    }
    fila += 1;
  }
  free(linea);
  fclose(fp);

  /*
  //mostrar por pantalla las 2 matrices a multiplicar
  printf("primera matriz: \n");
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", *(*(matriz1+a)+b));
    }
    printf("\n");
  }

  printf("\n");

  printf("segunda matriz: \n");
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", *(*(matriz2+a)+b));
    }
    printf("\n");
  }

  printf("\n");
  */


  //multiplicacion con threads

  unsigned t0 = clock();

  //creación de los threads
  sthread_t threads[tamano*tamano];

  //creando los threads para cada casilla de la matriz
  for(int i = 0; i < tamano*tamano; i++){
    sthread_create(&threads[i], &multiplicacion, tamano);
    sthread_join(threads[i]);
    o += 1;
  }

  //finalizar cronómetro y calcular tiempo de ejecución 
  unsigned t1 = clock();

  double time = (double(t1-t0) / CLOCKS_PER_SEC);

  printf("\nTiempo que demoró en realizar el calculo con threads: %f\n", time);

  /*
  //mostrar por pantalla el resultado de la multiplicación
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", *(*(matriz3+a)+b));
    }
    printf("\n");
  }
  */

  unsigned t2 = clock();

  for(int i = 0; i < tamano*tamano; i++){
    for(int n = 0; n < tamano; n++){
      int fila = i /tamano;
      int columna = i % tamano;
      *(*(matriz3+fila)+columna) += *(*(matriz1+fila)+n) * *(*(matriz2+n)+columna);
    }
  }

  unsigned t3 = clock();

  double time2 = (double(t3-t2) / CLOCKS_PER_SEC);

  printf("\nTiempo que demoró en realizar el calculo sin threads: %f\n", time2);

  
  for(int i = 0; i < tamano; i++){
    delete[] matriz1[i];
    delete[] matriz2[i];
    delete[] matriz3[i];
  }

  delete[] matriz1;
  delete[] matriz2;
  delete[] matriz3;
  

}
