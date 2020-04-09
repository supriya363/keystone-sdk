extern "C"
{
  #include <edge_call.h>
  #include <stdio.h>
  #include "eapp_utils.h"
  #include "string.h"
  #include "syscall.h"
  #include "malloc.h"
}

char* image_name;
long offset_in_file=0;

extern "C"
{
  FILE* fopen(const char *path, const char *mode);
  int fclose(FILE *stream);
  char *fgets(char *s, int size, FILE *stream);
  void rewind(FILE *stream);
}

void rewind(FILE *stream)
{
  offset_in_file = 0;
}

FILE* fopen(const char *path, const char *mode)
{
  image_name = (char*)malloc(strlen(path)+1);
  strncpy(image_name,path,strlen(path)+1);
  return 20;
}

int fclose(FILE *stream)
{
  free(image_name);
  return 0;
}

typedef struct file_struct
{
  long block_size;
  long num_blocks;
  long offset;
}file_struct;

void read_from_file(char* read_buffer,long block_size, long num_blocks,long offset,int *data_read)
{
  edge_data_t pkgstr;
  file_struct* file_data = (file_struct*)malloc(sizeof(file_struct)+strlen(image_name)+1);
  file_data->block_size = block_size;
  file_data->num_blocks = num_blocks;
  file_data->offset = offset;
  memcpy((void*)(file_data+1),image_name,strlen(image_name));
  void* file_input = malloc(sizeof(int)+num_blocks*block_size);

  ocall(7,file_data,sizeof(file_struct)+strlen(image_name)+1,&pkgstr,sizeof(edge_data_t),file_input);

  *data_read = *((int*)file_input);
  memcpy(read_buffer,(char*)((int*)file_input + 1),(*data_read) * block_size);
  free(file_data);
  free(file_input);
  //ocall(2, &block_size, sizeof(unsigned long), 0, 0,0);
  //ocall(2, &num_blocks, sizeof(unsigned long), 0, 0,0);
  return;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  int value_to_return;
  read_from_file((char*)ptr,size,nmemb,offset_in_file,&value_to_return);
  //ocall(2, &value_to_return, sizeof(unsigned long), 0, 0,0);
  offset_in_file = offset_in_file + size*value_to_return;
  return value_to_return;
}

char *fgets(char *s, int size, FILE *stream)
{
  int i = 0;
  int result = 1;
  char last_read = '\0';
  while(result!=0 && last_read != '\n')
  {
    result = fread(s+i,1,1,stream);
    last_read = s[i];
    i = i + result;
  }
  s[i] = '\0';
  if(i == 0)
  {
    return NULL;
  }
  else
  {
    return s;
  }
}
