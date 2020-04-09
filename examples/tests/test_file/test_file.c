#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "file.h"

#define OCALL_PRINT_BUFFER 1

void EAPP_ENTRY eapp_entry()
{
  char filename[] = "test.txt";
  FILE* fhand = fopen(filename,"r");
  char buffer[8];
  fread(buffer,6,1,fhand);
  ocall(OCALL_PRINT_BUFFER, buffer,7, NULL ,0,0);
  EAPP_RETURN(0);
}
