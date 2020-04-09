#include <edge_call.h>
#include "syscall.h"
#include "malloc.h"
#include <stdio.h>
#include <memory.h>
#include <math.h>

double log(double x)
{
	edge_data_t pkgstr;
  double number;
  ocall(26,&x,sizeof(double),&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void __attribute__((__section__(".text._start"))) recommend_rating(){

	double answer1 = log(10.5);
	double answer2 = log(0.5);
	double answer3 = log(3.0);
	ocall(27,&answer1,sizeof(double),NULL,0,0);
	ocall(27,&answer2,sizeof(double),NULL,0,0);
	ocall(27,&answer3,sizeof(double),NULL,0,0);
	EAPP_RETURN(0);
}
