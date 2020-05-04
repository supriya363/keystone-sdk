//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"

static int addr_data= 100;

struct sample
{
  int values[1024];
};

void EAPP_ENTRY eapp_entry(){

  char* msg = "Hi! This is Supriya Suresh!";
  unsigned long ret = 1;
  // char *msg2 = "Address of test_value = ";
  // char* msg2 = "Hi! This is message 2! Hello World";
  // edge_data_t pkgstr;

  edge_init();

  ocall_print_value((  unsigned long )addr_data);
  // int test_value = 555;
  ocall_print_buffer (msg, 27);
  // ocall_print_value_test( (unsigned long) test_value);
  // ret = ocall_print_buffer(msg, 22);
  // ocall_print_value((  unsigned long )&addr_data);
  // unsigned long ret = ocall_print_buffer(msg, 23);
  
  // ocall_print_buffer(msg2, 42);
  // ocall_print_value(ret);
  // void* host_str = malloc(56);
  // ocall_get_string(&pkgstr,host_str);
  // ocall_print_buffer(host_str, pkgstr.size+1);

  // int *arr = malloc(2048 * 2048 * sizeof(int));
  // for(int i=0; i<2048 * 2048; i++)
  // {
  //   // ocall_print_value((  unsigned long )i);
  //   arr[i] = i;
  // }

  ocall_print_value(sizeof(struct sample));
  struct sample *arr = malloc(10 * sizeof(struct sample));

  //need only 7 write access to replace 1st page 
  for(int i=0; i<10; i++)
  {
    ocall_print_value((  unsigned long )i);
    arr[i].values[0] = i*10;
  }

  //fill holding area
  arr[1].values[0] = 11; //arr[1] = 11 0x4000
  arr[4].values[0] = 44; //arr[4] = 44 0x7000
  arr[2].values[0] = 22; //0x5000
  arr[3].values[0] = 33; //0x6000
  arr[0].values[0] = 213; //0x6000
  arr[7].values[0] = 48; //0x6000
  arr[6].values[0] = 12; //0x6000
  arr[1].values[0] = 89; //0x6000

  ocall_print_value((  unsigned long )1);
  int value = arr[1].values[0];
  ocall_print_value((  unsigned long )value);

  // for(int i=0; i<100; i++)
  // {
  //   // ocall_print_value((  unsigned long )i);
  //   int value = arr[i].values[0];
  //   if(value != i*10)
  //     EAPP_RETURN(ret);
  //   // ocall_print_value((  unsigned long )value);
  // }
  // for(int i=999; i>=800; i--)
  // {
  //   // ocall_print_value((  unsigned long )i);
  //   int value = arr[i].values[0];
  //   if(value != i*10)
  //     EAPP_RETURN(ret);
  //   // ocall_print_value((  unsigned long )value);
  // }
  // for(int i=5; i<300; i++)
  // {
  //   // ocall_print_value((  unsigned long )i);
  //   int value = arr[i].values[0];
  //   if(value != i*10)
  //     EAPP_RETURN(ret);
  //   // ocall_print_value((  unsigned long )value);
  // }
  
  char *buf = "Success";
  ocall_print_buffer (buf, 7);
  EAPP_RETURN(ret);
}
