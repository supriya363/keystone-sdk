#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "hunspell.h"
#include<sys/stat.h>

int main()
{
  char affpath[] = "en_US.aff";
  char dpath[] = "en_US.dic";
  Hunhandle* library_handle;
  library_handle = Hunspell_create(affpath,dpath);

  char filename[] = "test.txt";
  int filesize;
  struct stat st;
  if(stat(filename,&st)==0)
  {
    filesize = st.st_size;
  }
  else
  {
    filesize = -1;
  }

  char* test_file = (char*)malloc(filesize);
  FILE* test_file_handle = fopen(filename,"r");
  fread(test_file, 1, filesize, test_file_handle);
  fclose(test_file_handle);

  int curr_pointer=0;
  int prev_pointer=0;
  int result;
  while(curr_pointer < filesize)
  {
    if(test_file[curr_pointer] == ' ' || curr_pointer == (filesize-1))
    {
      char* check_string = (char*)malloc(curr_pointer-prev_pointer+1);
      strncpy(check_string,test_file+prev_pointer,curr_pointer-prev_pointer);
      check_string[curr_pointer-prev_pointer] = '\0';
      curr_pointer++;
      prev_pointer = curr_pointer;

      result = Hunspell_spell(library_handle, check_string);
      if(result == 0)
      {
        printf("Bad Word %s\n",check_string);
      }
      else
      {
        printf("Good Word %s\n",check_string);
      }
    }
    else
    {
      curr_pointer++;
    }
  }

  return 0;
}
