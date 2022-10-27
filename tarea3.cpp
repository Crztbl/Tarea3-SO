#include <stdio.h>
#include "sthread.h"

static void go(int n);

#define NTHREADS 10
static sthread_t threads[NTHREADS];

int main(int argc, char **argv)
{
  int i;

  for(i = 0; i < NTHREADS; i++){
    sthread_create(&(threads[i]), &go, i);/* i es argumento*/
  }
  for(i = 0; i < NTHREADS; i++){
    long ret = sthread_join(threads[i]);
    printf("El Thread %d retorna un  %ld\n", i, ret);
  }
  printf("Fin del programa.\n");
  return 0;
}

void go(int n)
{
  printf("Hola desde el thread %d\n", n);
  sthread_exit(100 + n);
  //Aqui no se llega 
}