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

  int *arr = malloc(2048 * sizeof(int));
  for(int i=0; i<2048; i++)
  {
    ocall_print_value((  unsigned long )i);
    arr[i] = i;
  }


  EAPP_RETURN(ret);
}
