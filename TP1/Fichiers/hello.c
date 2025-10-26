#include <stdio.h>
#include <omp.h> 

int main()
{
#pragma omp parallel
#pragma omp critical 

  {
   printf("Bonjour ! le numero d'equipier est %d \n",omp_get_thread_num());
  
 
  
   printf("Au revoir !le numero d'equipier est %d \n",omp_get_thread_num());
  }

  return 0;
}
