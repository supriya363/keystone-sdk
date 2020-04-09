#include <edge_call.h>
#include <stdio.h>
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"

char* image_name;
char* image_name2;
long offset_in_file=0;
long offset_in_file2=0;

//extern int errno;

/*int* __errno_location()
{
  return &errno;
}*/

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define	LONG_MAX	((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#endif

int ISSPACE(char c)
{
  if(c == ' ')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISDIGIT(char c)
{
  if((int)c <= 57 && (int)c >= 48)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISALPHA(char c)
{
  if((int)c <= 122 && (int)c >= 97)
  {
    return 1;
  }
  else if((int)c <= 90 && (int)c >= 65)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISUPPER(char c)
{
  if((int)c <= 90 && (int)c >= 65)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/* https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c */
long strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (ISSPACE(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (ISDIGIT(c))
			c -= '0';
		else if (ISALPHA(c))
			c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		//errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}


int open(const char *path, const char *mode)
{
  image_name = (char*)malloc(strlen(path)+1);
  strncpy(image_name,path,strlen(path)+1);
  return 20;
}

int open2(const char *path, const char *mode)
{
  image_name2 = (char*)malloc(strlen(path)+1);
  strncpy(image_name2,path,strlen(path)+1);
  return 20;
}

int close(int stream)
{
  free(image_name);
  return 0;
}

int close2(int stream)
{
  free(image_name2);
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

void read_from_file2(char* read_buffer,long block_size, long num_blocks,long offset,int *data_read)
{
  edge_data_t pkgstr;
  file_struct* file_data = (file_struct*)malloc(sizeof(file_struct)+strlen(image_name2)+1);
  file_data->block_size = block_size;
  file_data->num_blocks = num_blocks;
  file_data->offset = offset;
  memcpy((void*)(file_data+1),image_name2,strlen(image_name2));
  void* file_input = malloc(sizeof(int)+num_blocks*block_size);

  ocall(7,file_data,sizeof(file_struct)+strlen(image_name2)+1,&pkgstr,sizeof(edge_data_t),file_input);

  *data_read = *((int*)file_input);
  memcpy(read_buffer,(char*)((int*)file_input + 1),(*data_read) * block_size);
  free(file_data);
  free(file_input);
  //ocall(2, &block_size, sizeof(unsigned long), 0, 0,0);
  //ocall(2, &num_blocks, sizeof(unsigned long), 0, 0,0);
  return;
}

size_t read2(int fd, void *buf, size_t count)
{
  int value_to_return;
  read_from_file2((char*)buf,1,count,offset_in_file2,&value_to_return);
  //ocall(2, &value_to_return, sizeof(unsigned long), 0, 0,0);
  offset_in_file2 = offset_in_file2 + 1*value_to_return;
  return value_to_return;
}

size_t read(int fd, void *buf, size_t count)
{
  int value_to_return;
  read_from_file((char*)buf,1,count,offset_in_file,&value_to_return);
  //ocall(2, &value_to_return, sizeof(unsigned long), 0, 0,0);
  offset_in_file = offset_in_file + 1*value_to_return;
  return value_to_return;
}

void exit(int status)
{
  ocall (11,&status,sizeof(int),NULL,0,0);
}
