#include <time.h>
#include <dirent.h>
#include "sthread.h"
#include <vector>
#include <fstream>

using namespace std;
int tamano;

int matriz1[10000][10000], matriz2[10000][10000], matriz3[10000][10000], matriz4[10000][10000];

void multiplicacion(int nro_thread){

  //CALCULAR LA COLUMNA Y LA FILA CON LA QUE SE ESTÁ TRABAJANDO
  int fila = nro_thread / tamano;
  int columna = nro_thread % tamano;

  //OBTENER EL VALOR DE LA CASILLA CORRESPONDIENTE DE LA MATRIZ RESULTANTE
  for(int i = 0; i < tamano; i++){
    matriz3[fila][columna] += matriz1[fila][i] * matriz2[i][columna];
  }
}

void txt_a_matriz(string nombre_archivo){
  //ABRIR EL ARCHIVO DE LAS MATRICES
  ifstream fp;
  fp.open(nombre_archivo);

  string linea;
  if(fp.is_open()){
    //LEER LA PRIMERA LINEA DEL ARCHIVO ".txt" PARA OBTENER EL TAMAÑO DE LAS MATRICES
    getline(fp, linea);
    tamano = stoi(linea);
    printf("Matriz %dx%d:\n", tamano, tamano);

    int fila = 0;

    //TRASPASAR LOS VALORES DE LAS MATRICES DESDE EL ".txt" A LAS MATRICES DEL CÓDIGO 
    while(getline(fp, linea)){
      for(int i = 0; i < tamano; i++){
        int hola = linea[i] - '0';
        if(fila < tamano){
          matriz1[fila][i] = hola;
        }
        else if(fila >= tamano && fila < 2*tamano){
          matriz2[fila-tamano][i] = hola;
        }
      }
      fila += 1;
    }
  }
}

int main(){

  //ABRIR EL DIRECTORIO
  DIR* dir = opendir(".");

  vector <string> archivos_a_abrir; 

  //CHEQUEAR ERROR AL ABRIR DIRECTORIO
  if(dir == NULL){
    return 1;
  }

  //GUARDAR EN UN VECTOR DE STRINGS LOS NOMBRES DE LOS ARCHIVOS PRESENTES EN EL DIRECTORIO QUE 
  //CORRESPONDEN A MATRICES BAJO EL CRITERO DE SI EL ARCHIVO TIENE EL FORMATO "matrices-N.txt"
  struct dirent* entity;
  entity = readdir(dir);
  while(entity != NULL){
    string nombre = entity->d_name;
    long unsigned int error = -1;
    if(nombre.find(".txt") != error && nombre.find("matrices-") != error){
      archivos_a_abrir.push_back(nombre);
    }
    entity = readdir(dir);
  }
  closedir(dir);

  for(unsigned long int i = 0; i < archivos_a_abrir.size(); i++){
    txt_a_matriz(archivos_a_abrir[i]);

    //"RE INICIAR" LA MATRIZ4 (DONDE VAN RESULTADOS DE LA MULTIPLICACIÓN CON THREADS), VOLVERLA A SU ESTADO ORIGINAL (PUROS CEROS)
    for(int a = 0; a < tamano; a++){
      for(int b = 0; b < tamano; b++){
          matriz3[a][b] = 0;
      }
    }
    //"RE INICIAR" LA MATRIZ4 (DONDE VAN RESULTADOS DE LA MULTIPLICACIÓN SIN THREADS), VOLVERLA A SU ESTADO ORIGINAL (PUROS CEROS)
    for(int a = 0; a < tamano; a++){
      for(int b = 0; b < tamano; b++){
          matriz3[a][b] = 0;
      }
    }

    //CREACION DE LOS THREADS
    sthread_t threads[tamano*tamano];

    //INICIAR CRONÓMETRO PARA MULTIPLICACIÓN SIN THREADS
    unsigned t0 = clock();

    //CAMBIAR MODALIDAD DEL USO DE THREADS EN BASE AL TAMAÑO DE LAS MATRICES(EN EL README SE DETALLA EL PORQUÉ DE ESTO)
    if(tamano > 150){
      for(int a = 0; a < tamano*tamano; a++){
        sthread_create(&threads[a], &multiplicacion, a);
        sthread_join(threads[a]);
      }
    }
    else{
      for(int a = 0; a < tamano*tamano; a++){
        sthread_create(&threads[a], &multiplicacion, a);
      }
      for(int a = 0; a < tamano*tamano; a++){
        sthread_join(threads[a]);
      }
    }

    //FINALIZAR CRONÓMETRO Y CALCULAR TIEMPO DE EJECUCIÓN
    unsigned t1 = clock();

    double time = (double(t1-t0) / CLOCKS_PER_SEC);

    printf("\nTiempo que demoró en realizar el calculo con threads: %f\n", time);

    //INICIAR CRONÓMETRO PARA MULTIPLICACIÓN SIN THREADS
    unsigned t2 = clock();
    
    //MULTIPLICACIÓN DE THREADS
    for(int i = 0; i < tamano*tamano; i++){
      for(int n = 0; n < tamano; n++){
        int fila = i /tamano;
        int columna = i % tamano;
        matriz4[fila][columna] += matriz1[fila][n] * matriz2[n][columna];
      }
    }

    //FINALIZAR CRONÓMETRO Y CALCULAR TIEMPO DE EJECUCIÓN
    unsigned t3 = clock();

    double time2 = (double(t3-t2) / CLOCKS_PER_SEC);

    printf("\nTiempo que demoró en realizar el calculo sin threads: %f\n\n", time2); 

    //MOSTRAR POR PANTALLA RESULTADOS DE MULTIPLICACIONES CON Y SIN THREADS
    /*
    printf("Resultado de la multiplicación con threads\n");
    for(int i = 0; i < tamano; i++){
      for(int j = 0; j < tamano; j++){
        printf("%d ", matriz3[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    printf("Resultado de la multiplicación sin threads\n");
    for(int i = 0; i < tamano; i++){
      for(int j = 0; j < tamano; j++){
        printf("%d ", matriz4[i][j]);
      }
      printf("\n");
    }
    */    
  }
}