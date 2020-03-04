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

  for(int i=0;i<4096;i++)
  {
    arr[i]++;
  }

  ocall_print_value(ct);

  // char * bad_addr=(char*)0x12345;
  // *bad_addr=*bad_addr+1;


  EAPP_RETURN(ret);
}
