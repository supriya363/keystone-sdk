#include <string.h>
#include <stdint.h>
#include <ctype.h>

char* strcpy(char dest[], const char source[])
{
    int i = 0;
    while (1)
    {
        dest[i] = source[i];

        if (dest[i] == '\0')
        {
            break;
        }

        i++;
    }
    return dest;
}

int tolower(int c)
{
  if(c<= 90 && c>=65)
  {
    return c+32;
  }
  else
  {
    return c;
  }
}

void* memcpy(void* dest, const void* src, size_t len)
{
  const char* s = src;
  char *d = dest;

  if ((((uintptr_t)dest | (uintptr_t)src) & (sizeof(uintptr_t)-1)) == 0) {
    while ((void*)d < (dest + len - (sizeof(uintptr_t)-1))) {
      *(uintptr_t*)d = *(const uintptr_t*)s;
      d += sizeof(uintptr_t);
      s += sizeof(uintptr_t);
    }
  }

  while (d < (char*)(dest + len))
    *d++ = *s++;

  return dest;
}

size_t strlen(const char* str){
  size_t len = 0;
  while(*str != '\0'){
    str++;
    len++;
  }

  return len;
}

void* memset(void* dest, int byte, size_t len)
{
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = byte;
  }
  return dest;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
  unsigned char u1, u2;

  for ( ; n-- ; s1++, s2++) {
    u1 = * (unsigned char *) s1;
    u2 = * (unsigned char *) s2;
    if ( u1 != u2) {
      return (u1-u2);
    }
  }
  return 0;
}
//TODO this is from Linux src, needs licence

/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, size_t count)
{
  char *tmp;
  const char *s;

  if (dest <= src) {
    tmp = dest;
    s = src;
    while (count--)
      *tmp++ = *s++;
  } else {
    tmp = dest;
    tmp += count;
    s = src;
    s += count;
    while (count--)
      *--tmp = *--s;
  }
  return dest;
}
int strcmp(const char *s1, const char *s2)
{
  int len= strlen(s1);
  if(len!=strlen(s2))
    return 1;

  for(int i=0;i<len;i++)
  {
    if(s1[i]!=s2[i])
      return 1;
  }
  return 0;
}

char* strncpy(char *dest, const char *src, size_t n)
{
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
  {
    dest[i] = src[i];
  }
  for ( ; i < n; i++)
  {
    dest[i] = '\0';
  }
  return dest;
}
