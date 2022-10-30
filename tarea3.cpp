#include <stdio.h>
#include <time.h>
#include "sthread.h"

#define tamano 5

int matriz[tamano][tamano];
int matriz2[tamano][tamano];
int matriz3[tamano][tamano];

void multiplicacion(int i){

  int fila = (int) i / tamano;
  int columna = i % tamano;
  for(int n = 0; n<tamano; n++){
    matriz3[fila][columna] += matriz[fila][n] * matriz2[n][columna];
  }
}

int main(int argc, char **argv)
{

  //abrir el archivo con las matrices
  FILE *fp;
  fp = fopen("matrices-n.txt", "r");

  //CHEQUEO ERROR AL ABRIR ARCHIVO
  if(fp == NULL){
    printf("No se puede abrir el archivo de la carta\n");
  }

  //obtencion del tamaño de las matrices
  char tamano_char[10], *linea;
  fgets(tamano_char, sizeof(tamano_char), fp);
  //tamano = (int) atoi(tamano_char), 
  int fila = 0;

  //creacion de las matrices con el tamaño adecuado
  //int matriz[tamano][tamano];
  //int matriz2[tamano][tamano];
  //int matriz3[tamano][tamano];
  
  //transformar las matrices del .txt a un arreglo 2d
  size_t len=0;
  while(getline(&linea, &len, fp)!=-1){
    for(int i = 0; i < tamano; i++){
      if(fila < tamano){
        matriz[fila][i] = (int)(linea[i] - '0');
      }
      else if(fila >= tamano){
        matriz2[fila-tamano][i] = (int)(linea[i] - '0');
      }
    }
    fila += 1;
  }
  fclose(fp);

  //mostrar por pantalla las 2 matrices a multiplicar
  printf("primera matriz: \n");
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", matriz[a][b]);
    }
    printf("\n");
  }

  printf("\n");

  printf("segunda matriz: \n");
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", matriz2[a][b]);
    }
    printf("\n");
  }
  printf("\n");

  //iniciar cronometro
  unsigned t0 = clock();


  //creación de los threads
  sthread_t threads[tamano*tamano];

  //creando los threads para cada casilla de la matriz
  for(int i = 0; i < tamano*tamano; i++){
    sthread_create(&threads[i], &multiplicacion, i);
  }

  //uniendo los threads
  for(int i = 0; i < tamano*tamano; i++){
    sthread_join(threads[i]);
  }

  //finalizar cronómetro y calcular tiempo de ejecución 
  unsigned t1 = clock();

  double time = (double(t1-t0) / CLOCKS_PER_SEC);

  //mostrar la matriz final
  printf("resultado de la multiplicacion de la primera por la segunda: \n");
  for(int a = 0; a < tamano; a++){
    for(int b = 0; b < tamano; b++){
      printf("%d ", matriz3[a][b]);
    }
    printf("\n");
  }

  printf("\nTiempo que demoró en realizar el calculo: %f\n", time);

  return 0;
}