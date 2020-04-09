// Writen by Attractive Chaos; distributed under the MIT license
// Naive matrix mulitplication benchmark

//#include <stdlib.h>
//#include <stdio.h>
#include "malloc.h"
#include "eapp_utils.h"
#include "syscall.h"
#define OCALL_PRINT_VALUE 2
#define OCALL_PRINT_BUFFER 1

int co=10;
void display_matrix(uintptr_t**a , int n)
{
  int i,j;
  for (i = 0; i < n; ++i)
  {
      for (j = 0; j < n; ++j)
      {
          ocall(OCALL_PRINT_VALUE, &a[i][j], sizeof(uintptr_t), 0, 0,0);
      }
  }
}


uintptr_t **mm_init(int n)
{
  uintptr_t **m;
  int i;
  m = (uintptr_t**)malloc(n * sizeof(void*));
  for (i = 0; i < n; ++i)
    m[i] = (uintptr_t*)malloc(n* sizeof(uintptr_t));
  return m;
}
void mm_destroy(int n, uintptr_t **m)
{
  int i;
  for (i = 0; i < n; ++i) free(m[i]);
  free(m);
}
uintptr_t **mm_gen(int n)
{
  co++;
  uintptr_t **m;
  int i, j;
  m = mm_init(n);
  for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
      //m[i][j] = tmp * (i - j) * (i + j);
      m[i][j] = (uintptr_t)((i+j)*co);
  return m;
}
/*
// better cache performance by transposing the second matrix
uintptr_t **mm_mul(int n, uintptr_t *const *a, uintptr_t *const *b)
{
  int i, j, k;
  uintptr_t **m, **c;
  m = mm_init(n); c = mm_init(n);
  for (i = 0; i < n; ++i) // transpose
    for (j = 0; j < n; ++j)
      c[i][j] = b[j][i];

  for (i = 0; i < n; ++i) {
    uintptr_t *p = a[i], *q = m[i];
    for (j = 0; j < n; ++j) {
      uintptr_t t = 0.0, *r = c[j];
      for (k = 0; k < n; ++k)
        t += p[k] * r[k];
      q[j] = t;
    }
  }
  mm_destroy(n, c);
  return m;
}
*/
// better cache performance by transposing the second matrix
uintptr_t **mm_mul(int n, uintptr_t *const *a, uintptr_t *const *b)
{
  int i, j, k;
  uintptr_t **m;
  m = mm_init(n);
  for (i = 0; i < n; ++i) // transpose
    for (j = i+1; j < n; ++j)
    {
      uintptr_t temp= b[i][j];
      b[i][j]=b[j][i];
      b[j][i]=temp;
    }

  for (i = 0; i < n; ++i) {
    uintptr_t *p = a[i], *q = m[i];
    for (j = 0; j < n; ++j) {
      uintptr_t t = 0.0, *r = b[j];
      for (k = 0; k < n; ++k)
        t += p[k] * r[k];
      q[j] = t;
    }
  }
  //mm_destroy(n, c);
  return m;
}




void EAPP_ENTRY entry()
{
  //int n = 280;

  int max_limit=500;
  int n = 0;
  s_rand((unsigned long)&n, (unsigned long)sizeof(int));
  if(n<0)n=n*-1;
  n=n%max_limit;

  n=500+n;
  n=1000;//change to 1000 and uncomment after roram trace
  ocall(2, &n, sizeof(int), 0, 0,0);

  unsigned long ret=4;

  uintptr_t **a;
  //uintptr_t a[4];
  uintptr_t **b;
  uintptr_t **m;
  //if (argc > 1) n = atoi(argv[1]);
  n = (n/2) * 2;
  a = mm_gen(n);

  /*for (int i = 0; i < n; i++) // transpose
    for (int j = 0; j < n; j++)
      a[i]=a[i];
  */

  b = mm_gen(n);
  //display_matrix(a ,  n);
  //display_matrix(b ,  n);
  m = mm_mul(n, a, b);
  //display_matrix(m ,  n);
  //fprintf(stderr, "%lf\n", m[n/2][n/2]);
  mm_destroy(n, a); mm_destroy(n, b); mm_destroy(n, m);
  ocall(OCALL_PRINT_BUFFER, "Mult done", 11, NULL ,0,0);

  float error=1.35;error=error;
  /*s_rand((unsigned long)&error, (unsigned long)sizeof(float));
  float error2=3.35;
  float error3=error*error2;error3=error3;
  */
  char* j= (char*)0;
  *j=*j;

  EAPP_RETURN(ret);
}
