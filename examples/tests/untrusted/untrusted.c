//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"
static int addr_data=5;

/*
void sort(int *arr, int n)
{
 for(int i=0;i<100-1;i++)
 {
   for(int j=0;j<100-1-i;j++)
   {
     if(arr[j]>arr[j+1])
     {
       int temp=arr[j];
       arr[j]=arr[j+1];
       arr[j+1]=temp;
     }
   }
 }
}
*/
void EAPP_ENTRY eapp_entry(){

  char* msg = "hello world!. Only getstring() withoout copy to user"; msg=msg;
  char* msg2 = "2nd hello world!";msg2=msg2;
  edge_data_t pkgstr;

  edge_init();

  /*
  int *badaddress2=(int*)0x12349;
  int c2=*badaddress2;
  c2=c2+1;
  */

  addr_data=addr_data;
  /*
  int *badaddress2=(int*)0x12349;
  int cc=*badaddress2;
  cc=cc+1;
  */
  ocall_print_value((  unsigned long )&addr_data);
  unsigned long ret = ocall_print_buffer(msg, 53);//o/p-1
  ocall_print_buffer(msg2, 17);//o/p-2
  //unsigned long ret = 38;

  //ocall_print_buffer(msg2, 17);

  ocall_print_value(ret);//o/p-3





  //ecall_sbrk(7*4096);


  void* host_str = malloc(19000);
  //(unsigned long)&
  ocall_get_string(&pkgstr,host_str);
  //malloc_stats();



  //ocall_get_string(&pkgstr,0);
  //malloc(10);
  //void* host_str = malloc(pkgstr.size);
  //char host_str[100];

  //char host_str2[100];
  //host_str=host_str2;



  //ocall_print_value(pkgstr.size);//o/p-4
  //copy_from_shared((void*)host_str, pkgstr.offset, pkgstr.size);
  /*
  for(i=12216;i<18100;i++)
      ((char*)host_str)[i]='R';
  ((char*)host_str)[i]='\0';

  */


  ocall_print_buffer(host_str, pkgstr.size+1);//o/p-5

  int i;
  int ct=0;
  for(i = 0; ((char*)host_str)[i] !='\0'; i++){
    if( ((char*)host_str)[i] == 'l' || 1==1){
      ((char*)host_str)[i]++;
      ct++;
    }


  }

  ct++;
  ocall_print_value((unsigned long)host_str);//(unsigned long)&
  ocall_print_value((unsigned long)&pkgstr);
  ct++;
  ocall_print_value(ct);
  char *success_msg="Mini demand paging completess";success_msg=success_msg;
  ocall_print_buffer(success_msg, 29+1);
  int v=0;
  s_rand((unsigned long)&v, (unsigned long)sizeof(int));
  ocall_print_value(v);




  //ocall_print_value(ct);
  //free(host_str);
  /*
  host_str = malloc(19000);
  host_str = malloc(19000);
  ocall_print_value((unsigned long)host_str);

  for(i = 0; i<19000; i++){
    ((char*)(host_str))[i]='l';
  }

  for(i = 0; i<19000; i++){
    char c = ((char*)(host_str))[i];c=c;
  }
  */




//-----------------------------------------------------------------
 int arr[100];
  for(int i=99;i>=0;i--)
      arr[i]=i;


for(int k=1;k<=5;k++){
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  //ocall_print_buffer(success_msg, 28);
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
  for(int i=0;i<100-1;i++)
  {
    for(int j=0;j<100-1-i;j++)
    {
      if(arr[j]>arr[j+1])
      {
        int temp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=temp;
      }
    }
  }
}//k ends


//-----------------------------------------------------------------






  //sort(arr,100);
  int *badaddress=(int*)0x12349;
  int c=*badaddress;
  c=c+1;




  EAPP_RETURN(ret);
}
