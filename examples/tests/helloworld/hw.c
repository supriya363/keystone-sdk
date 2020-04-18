//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"

int arr[4096];


void EAPP_ENTRY eapp_entry(){

  char* msg = "Hello World!\n";
  char* msg2 = "This is a test message!\n";

  edge_init();

  unsigned long ret = ocall_print_buffer(msg, 24);
  ocall_print_buffer(msg2, 24);

  ocall_print_value(ret);

  struct edge_data pkgstr;
  ocall_get_string(&pkgstr);

  void* host_str = malloc(pkgstr.size);
  copy_from_shared(host_str, pkgstr.offset, pkgstr.size);

  int i;
  int ct;
  for(i = 0; i < pkgstr.size; i++){
    if( ((char*)host_str)[i] == 'l' ){
      ct++;
    }
  }
  // char *str1 = "&arr[0] = ";
  // char *str2 = "&arr[4095] = ";
  // ocall_print_buffer(str1, strlen(str1));
  // ocall_print_value((unsigned long)&arr[0]);  //0x2060
  // ocall_print_value((unsigned long)&arr[4096]);  //0x6060
  // ocall_print_buffer(str2, strlen(str2));
  ocall_print_value(ct);
  char* msg3 = "Writing to first page\n";
  ocall_print_buffer(msg3, strlen(msg3));
  // arr[0] = 1;
  // arr[2048] = 1;
  // arr[3072] = 1;
  // arr[4096] = 1;


  EAPP_RETURN(ret);
}
