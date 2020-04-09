
#include<stdlib.h>
extern "C"
{
  #include <edge_call.h>
  #include "eapp_utils.h"
  #include "syscall.h"
}
//using namespace std;

//extern "C" {int ocall(unsigned long call_id,void* data, size_t data_len,void* return_buffer, size_t return_len,void* data_ptr);}

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}

void operator delete(void * ptr, unsigned long size)
{
	::operator delete(ptr);
}


void operator delete[](void * ptr, unsigned long size)
{
	::operator delete(ptr);
}

class sample
{
    public:
	sample()
	{
		ocall(1,(void*)"Hi\n",4,NULL,0,0);
	}
	~sample()
	{
		ocall(1,(void*)"Bye\n",5,NULL,0,0);
	}
};

void __attribute__((__section__(".text._start"))) enter()
{
	sample *obj = new sample();
	delete(obj);
	EAPP_RETURN(0);
}
