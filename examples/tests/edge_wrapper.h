//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include "edge_call.h"
#include "keystone.h"

typedef struct packaged_str{
  unsigned long str_offset;
  size_t len;
} packaged_str_t;

typedef unsigned char byte;

int edge_init(Keystone* enclave);
void call_exit(void* buffer);
void calc_log(void* buffer);
void calc_exp(void* buffer);
void calc_tanh(void* buffer);
void calc_pow(void* buffer);
void calc_exp(void* buffer);
void calc_srand(void* buffer);
void calc_rand(void* buffer);
void calc_sqrt(void* buffer);
void print_double_wrapper(void* buffer);

void print_buffer_wrapper(void* buffer);
unsigned long print_buffer(char* str);

void print_value_wrapper(void* buffer);
void print_value(long val);

void copy_report_wrapper(void* buffer);
void copy_report(void* shared_buffer);

void get_host_string_wrapper(void* buffer);
void get_host_content_from_utm_wrapper(void* buffer);
void get_enclave_content_from_epm_wrapper(void* buffer);
void read_bucket(void* buffer);
void write_buckets(void* buffer);
void get_testing_params(void* buffer);
void read_from_file_wrapper(void* buffer);

//OPAM FUNCTIONS
void opam_write_buckets(void* buffer);
void opam_read_bucket_md(void* buffer);
void opam_read_bucket_d(void* buffer);
void opam_write_buckets_d(void* buffer);
void opam_write_buckets_md(void* buffer);
void read_num(void* buffer);
void full_trace(void* buffer);

void store_content_to_utm_enc_pfh(void* buffer);
void get_host_content_from_utm_wrapper_enc_pfh(void* buffer);
void print_counters(void* buffer);

typedef struct file_struct
{
  long block_size;
  long num_blocks;
  long offset;
}file_struct;

typedef struct sat_struct
{
  int is_open;
  FILE* fd;
}sat_struct;

const char* get_host_string();
void roram_write_bucket_init(void* buffer);
void roram_read_block(void* buffer);
#endif /* _EDGE_WRAPPER_H_ */
