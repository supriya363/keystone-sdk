//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"
#include "printf.h"

#define MAX 500
#define LOOP 50000
typedef struct Page
{
  int values[1024];
} Page;

	
void EAPP_ENTRY eapp_entry(){	
	int i,k,data,num;num=9;data=9;num=num,data=data;
        char* msg = "APP IS RUNNING"; //14
	ocall_print_buffer(msg,14);
	char* check = "cHECKING!\0";check=check;//10
	char* success = "SUCCESS!\0";success=success;//9
        char* failed = "FAILED!\0";failed=failed;//8
	Page *dynamicpage = (Page*)malloc(MAX*sizeof(Page));
	Page *staticpage = (Page*)malloc(MAX*sizeof(Page));
	k=0;k=k;
	

	for (i=0;i<MAX;i++){
		for(k=0;k<1024;k++){
			staticpage[i].values[k]=-1;
			dynamicpage[i].values[k]=-1;
		}
	}
	
	for(i=0;i<LOOP;i++){
		s_rand((unsigned long)&num, (unsigned long)sizeof(int));
		num = num<0?(-1)*num:num;
		num = num%MAX;
		//num=i%MAX;
		s_rand((unsigned long)&data, (unsigned long)sizeof(int));
		data = data<0?(-1)*data:data;
		data = data%1007;
		if(i%500==0){
			ocall_print_value(i);
			ocall_print_buffer(check,10);
		}
		for(k=0;k<1024;k++){
			staticpage[num].values[k]=(int)data;
			dynamicpage[num].values[k]=(int)data;
		}
	}
	ocall_print_buffer(check,10);
//	staticpage[0].values[0]=0;  //THESE LINES ARE ADDEDD TO DELIBERATELY CAUSE A MATCHING FAILURE`
//	dynamicpage[0].values[0]=1;
	
	for(i=0;i<MAX;i++){
		for(k=0;k<1024;k++){
			if(staticpage[i].values[k] != dynamicpage[i].values[k]){
				ocall_print_buffer(failed,8);
				ocall_print_value(i);
				ocall_print_value(staticpage[i].values[k]);
				ocall_print_value(dynamicpage[i].values[k]);
				EAPP_RETURN(-1);
			}
		}
	}

	ocall_print_buffer(success,9);
	EAPP_RETURN(0);
}
