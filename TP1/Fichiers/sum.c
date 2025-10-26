#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

/* macro de mesure de temps, retourne une valeur en �secondes */
#define TIME_DIFF(t1, t2) \
        ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec))

#define N (10 * 1024 * 1024)

int tab[N];

int main (int argc, char *argv[]) 
{
  struct timeval t1, t2;
  int sum;
  unsigned long temps;

  srand(1);

  for (int i=0; i<N;i++)
    tab[i] = rand();

  sum = 0;
  gettimeofday(&t1,NULL);

  // version s�quentielle
  for (int i=0; i<N;i++)
    sum += tab[i];

  gettimeofday(&t2,NULL);
  
  temps = TIME_DIFF(t1,t2);
  printf("seq\t\t: %ld.%03ldms   sum = %u\n", temps/1000, temps%1000, sum);

  ///////////// premi�re technique : critical
  sum = 0;
  gettimeofday(&t1,NULL);

#pragma omp parallel for
  for (int i=0; i<N;i++)
  #pragma omp critical
    sum += tab[i];

  gettimeofday(&t2,NULL);
  
  temps = TIME_DIFF(t1,t2);
  printf("critical\t: %ld.%03ldms   sum = %u\n", temps/1000, temps%1000, sum);


  ///////////// deuxi�me technique : atomic
  sum = 0;
  gettimeofday(&t1,NULL);

#pragma omp parallel for
  for (int i=0; i<N;i++)
  #pragma omp atomic
    sum += tab[i];

  gettimeofday(&t2,NULL);
  
  temps = TIME_DIFF(t1,t2);
  printf("atomic\t\t: %ld.%03ldms   sum = %u\n", temps/1000, temps%1000, sum);


  ///////////// troisi�me technique : sommes partielles
  sum = 0;
  gettimeofday(&t1,NULL);

  #pragma omp parallel
  {
    int ma_somme;
    ma_somme=0;
    #pragma omp for
  for (int i=0; i<N;i++)

    ma_somme+= tab[i];
    #pragma omp atomic 
    sum+=ma_somme;
  }
  gettimeofday(&t2,NULL);

  temps = TIME_DIFF(t1,t2);
printf("local\t\t: %ld.%03ldms   sum = %u\n", temps/1000, temps%1000, sum);

  ///////////// quatri�me technique : reduction OpenMP
  sum = 0;
  gettimeofday(&t1,NULL);

#pragma omp parallel for reduction(+:sum)
  for (int i=0; i<N;i++)
    sum += tab[i];

  gettimeofday(&t2,NULL);

  temps = TIME_DIFF(t1,t2);
printf("reduction\t: %ld.%03ldms   sum = %u\n", temps/1000, temps%1000, sum);

  return 0;
}
