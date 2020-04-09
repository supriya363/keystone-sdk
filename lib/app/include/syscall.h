//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stdint.h>
#include <stddef.h>
/* TODO We should be syncing these more explictly with the runtime
   defs */
#define SYSCALL_OCALL 1001
#define SYSCALL_SHAREDCOPY  1002
#define SYSCALL_ATTEST_ENCLAVE  1003
#define SYSCALL_EXIT  1101
#define SYSCALL_SBRK  2000
#define SYSCALL_RAND  3000


#define SYSCALL(which, arg0, arg1, arg2, arg3, arg4 , arg5) ( {	\
	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);	\
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);	\
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);	\
	register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);	\
	register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);	\
	register uintptr_t a6 asm ("a6") = (uintptr_t)(arg5);	\
	register uintptr_t a7 asm ("a7") = (uintptr_t)(which);	\
	asm volatile ("ecall"					\
		      : "+r" (a0)				\
		      : "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a6), "r" (a7)\
		      : "memory");				\
	a0;							\
})

#define SYSCALL_0(which) SYSCALL(which, 0, 0, 0, 0, 0,0)
#define SYSCALL_1(which, arg0) SYSCALL(which, arg0, 0, 0, 0, 0,0)
#define SYSCALL_2(which, arg0, arg1) SYSCALL(which, arg0, arg1, 0, 0, 0,0)
#define SYSCALL_3(which, arg0, arg1, arg2) SYSCALL(which, arg0, arg1, arg2, 0, 0,0)
#define SYSCALL_5(which, arg0, arg1, arg2, arg3, arg4) SYSCALL(which, arg0, arg1, arg2, arg3, arg4,0)

#define SYSCALL_6(which, arg0, arg1, arg2, arg3, arg4,arg5) SYSCALL(which, arg0, arg1, arg2, arg3, arg4,arg5)


int copy_from_shared(void* dst,
		     uintptr_t offset, size_t data_len);

int ocall(unsigned long call_id,void* data, size_t data_len,void* return_buffer, size_t return_len,void* data_ptr);
uintptr_t untrusted_mmap();
int attest_enclave(void* report, void* data, size_t size);
unsigned long s_brk(size_t bytes);
unsigned long s_rand(unsigned long loc, unsigned long sz);

#endif /* syscall.h */
