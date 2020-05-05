//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"

#define PAGE_SIZE 4096

void EAPP_ENTRY eapp_entry(){

  char* msg = "Hi! This is Matrix!";
  unsigned long ret = 1;

  edge_init();

  ocall_print_buffer (msg, 19);
  unsigned long value_range = 100000;
  unsigned long range = 100;
  unsigned long size = PAGE_SIZE * range;

  int *arr1 = (int*)malloc(size * sizeof(int));
  int *arr2 = (int*)malloc(size * sizeof(int));
  unsigned long rand;
  //initialize both arrays
  for(int i=0; i<size; i++)
  {
    arr1[i] = i;
    arr2[i] = i;
  }

  //random 10,000 write accesses
  for(int i=0; i<10000; i++)
  {
    s_rand((unsigned long)&rand, sizeof(unsigned long));
    int value = rand%value_range;
    int index = rand%range;
    arr1[index] = value;
    arr2[index] = value;
  }

  //test 
  for(int i=0; i<size; i++)
  {
    int value1 = arr1[i];
    int value2 = arr2[i];
    if(value1 != value2)
    {
      char *buf = "Fail";
      ocall_print_buffer (buf, 4);
      EAPP_RETURN(ret);
    }
      
  }

  char *buf = "Success";
  ocall_print_buffer (buf, 7);
  EAPP_RETURN(ret);
}
