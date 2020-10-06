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
//static int addr_data= 100;
#define MAX 100
#define LOOP 100000
typedef struct Page
{
  int values[1024];
}Page;

	
void EAPP_ENTRY eapp_entry(){	
	int i,k,data,num;
        char* msg = "APP IS RUNNING"; //14
	ocall_print_buffer(msg,14);
	char* check = "CHECKING!\0";//10
	char* success = "SUCCESS!\0";//9
        char* failed = "FAILED!\0";//8
	Page *dynamicpage = (Page*)malloc(MAX*sizeof(Page));
	Page *staticpage = (Page*)malloc(MAX*sizeof(Page));
	//Initialize the staticpage(actually dynamic, named so as work had progressed)
	//staticpage and static page are copies of each other. Aim of this App is to check if after encountering page faults andmoving arounf victims and bringing them back, still renders the pages unchanged, as must be the case.
	

	//Initializing the static page, to detect if page is fresh.
	for(i=0;i<MAX;i++){
		staticpage[i].values[0]=-1;
	}

	//Aim of this loop is to generate a random permutation from 0 to MAX-1 and fill these indices with random data.
	for(i=0;i<MAX;i++){
		//generate random indices 
		s_rand((unsigned long)&num, (unsigned long)sizeof(int));
		num = num<0?(-1)*num:num;
		num = num%MAX;
		//checks if this index was written earlier, if not write random data
		if(staticpage[num].values[0]!=-1){
			--i;
		}
		else{
			s_rand((unsigned long)&data, (unsigned long)sizeof(int));
			data = data<0?(-1)*data:data;
			data = data%1007;
			for(k=0;k<1024;k++){
				staticpage[num].values[k]=(int)data;
				dynamicpage[num].values[k]=(int)data;
			}
		}
	}
	ocall_print_buffer(check,10);
// Uncommenting the below lines should cause a failure
//	staticpage[0].values[0]=0;  //THESE LINES ARE ADDEDD TO DELIBERATELY CAUSE A MATCHING FAILURE`
//	dynamicpage[0].values[0]=1;
	


	// Sequntial access test
	for(i=0;i<MAX;i++){
		for(k=0;k<1024;k++){
			//This if should never execute if everything is Hunky Dory.
			if(staticpage[i].values[k] != dynamicpage[i].values[k]){
				ocall_print_buffer(failed,8);
				ocall_print_buffer(failed,8);
				ocall_print_value(i);
				ocall_print_value(staticpage[i].values[k]);
				ocall_print_value(dynamicpage[i].values[k]);
				EAPP_RETURN(-1);
			}
		}
	}
	ocall_print_buffer(success,9);

	// Random access test
	char* progressmessage="WORK IN PROGRESS!\0";//18
	char* finalmessage="COMPLETED!\0";//11
	for (i=0;i<LOOP;i++){
		s_rand((unsigned long)&num, (unsigned long)sizeof(int));
		num= num<0? (-1)*num : num;
		num=num%MAX;
		for(k=0;k<1024;k++){
			if(staticpage[num%MAX].values[k] != dynamicpage[num%MAX].values[k]){
				ocall_print_buffer(failed,8);
				EAPP_RETURN(-1);
			}

		}
		if (i%1000==0){
			//Heartbeat message
			ocall_print_buffer(progressmessage,18);
			ocall_print_value(i);
		}
	}
	ocall_print_buffer(finalmessage,18);
	EAPP_RETURN(0);
}
