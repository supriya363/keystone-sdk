//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <getopt.h>
#include "keystone.h"
#include "edge_wrapper.h"
#include "report.h"
#include "test_dev_key.h"
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std::chrono;
const char* longstr = "My name is Nirjhar. llllll\n";

//uintptr_t args_user[5];
unsigned long print_buffer(char* str){
  printf("[ENCLAVE]: %s",str);
  fflush(stdout);

  /*

  printf("Enclave said: ");
  unsigned long len=strlen(str);
  for(int i=0;i<len;i++)
      printf("%c",str[i]);
  printf(" Length = %lu: ",len);
  printf("\n");
  */


  return strlen(str);
}

void print_value(long val){
  printf("Enclave said value: %ld\n",val);
  return;
}

const char* get_host_string(){
  return longstr;
}

static struct report_t report;

void print_hex(void* buffer, size_t len)
{
  int i;
  for(i = 0; i < len; i+=sizeof(uintptr_t))
  {
    printf("%.16lx ", *((uintptr_t*) ((uintptr_t)buffer + i)));
  }
  printf("\n");
}

void copy_report(void* buffer)
{
  Report report;

  report.fromBytes((unsigned char*)buffer);

  if (report.checkSignaturesOnly(_sanctum_dev_public_key))
  {
    printf("Attestation report SIGNATURE is valid\n");
  }
  else
  {
    printf("Attestation report is invalid\n");
  }
}

static struct option long_opts[] = {
  {"pf-handler", required_argument, NULL, 'f'     },
  {"integrity", required_argument, NULL, 'i'      },
  {"confidentiality", required_argument, NULL, 'c'},
  {"free-pages", required_argument, NULL, 'p'     },
  {"victim-cache", required_argument, NULL, 'v'   },
  {"tracing", required_argument, NULL, 't'        },
  {"debug", required_argument, NULL, 'd'        },
  {"file", required_argument, NULL, 'n'        },
  {"tree-exc", required_argument, NULL, 'e'        },
  {"fault-limit", required_argument, NULL, 'l'        },
  {"help", no_argument, NULL, 'h'                 }
};

void print_usage(const char* argv0)
{
  std::cerr
    << argv0 << " [options] <enclave> <runtime>" << std::endl
    << std::endl
    << "  --pf-handler <PF-HANDLER>   0 - simple, 1 - ORAM, 2 - OPAM, 3 - EPF 4 - RORAM, 5 - WORAM (default=0)" << std::endl
    << "  --integrity <flag>          enable/disable integrity protection (default=0)" << std::endl
    << "  --confidentiality <flag>    enable/disable confidentiality (default=0)" << std::endl
    << "  --victim-cache <flag>    enable/disable victim cache (default=0)" << std::endl
    << "  --tracing <flag>              enable/disale tracing (default=0)" << std::endl
    << "  --debug <flag>              enable/disale debugging (default=0)" << std::endl
    << "  --file <file name>              file name to print details" << std::endl
    << "  --free-pages <num-pages>    number of free pages (default=10)" << std::endl
    << "  --tree-exc <num-pages>    tree exclusivity (default=0)" << std::endl
    << "  --fault-limit <num-pages>    page fault limit (default=0)" << std::endl
    << "  --h/--help                  print this message." << std::endl
    << std::endl
    << "Enclave application loader and executer." << std::endl << std::endl;

  exit(1);
}





int main(int argc, char** argv)
{
  // set default values of arguments.

  //args_user[0] = 0;   // default is no ORAM/OPAM/Encrypted Page FAult
  args_user.page_fault_handler=PAGE_FAULT_HANDLER_SIMPLE;

  //args_user[1] = 0;   // no integrity protection by default.
  args_user.integrity_protection=0;

  //args_user[2] = 0;   // no confidentiality by default.
  args_user.confidentiality=0;

  args_user.victim_cache=0;

  //args_user[3] = 10;  // number of free pages by default = 10
  args_user.num_free_pages=10;

  args_user.confidentiality=0;

  //args_user[4] = 0;   // default is not tracing.
  args_user.page_addr_tracing=0;

  args_user.debug_mode=0;// default is not debug.

  args_user.tree_exc=0;// default is not exclusive.

  args_user.fault_limit=0;

  int option_index, c;
  char fn[100];
  while((c = getopt_long(argc, argv, "f:i:c:p:v:t:d:n:e:l:h",
                         long_opts, &option_index)) != -1)
  {
    switch(c) {
      case 'f':
        args_user.page_fault_handler = (page_fault_type) atoi(optarg);
        break;
      case 'i':
        args_user.integrity_protection = (uint8_t) atoi(optarg);
        break;
      case 'c':
        args_user.confidentiality = (uint8_t) atoi(optarg);
        break;
      case 'p':
        args_user.num_free_pages = (int) atoi(optarg);
        break;
      case 'v':
        args_user.victim_cache = (uint8_t) atoi(optarg);
      case 't':
        args_user.page_addr_tracing = (uint8_t) atoi(optarg);
        break;
      case 'd':
        args_user.debug_mode = (uint8_t) atoi(optarg);
        break;
      case 'n':
        strcpy(fn,optarg);
        break;
      case 'e':
        args_user.tree_exc = (uint8_t) atoi(optarg);
        break;
      case 'l':
        args_user.fault_limit = (int) atoi(optarg);
        break;
      case 'h':
        print_usage(argv[0]);
        break;
      case '?':
        std::cerr << "Usage error." << std::endl;
        print_usage(argv[0]);
        break;
    }
  }
  char del_cmd[100];
  strcpy(del_cmd,"rm -rf ");


  strcpy(trace_file,"tracing/");
  mkdir(trace_file, 0777);







  strcat(trace_file,argv[optind]);

  //remove_directory(trace_file);


  //return 1;


  mkdir(trace_file, 0777);

  strcat(trace_file,"/tracing_");
  strcat(trace_file,fn);
  strcat(del_cmd,trace_file);
  system(del_cmd);



  //strcpy(pc_file,"perf_");
  //strcat(pc_file,argv[optind]);
  //mkdir(pc_file, 0777);
  strcat(pc_file,"performance_counters.csv");
  //strcat(pc_file,fn);

  FILE *fptr_pc=NULL;

  std::ifstream infile(pc_file);
  if(!infile.good())
  {
    //printf("file NOT present");
    fptr_pc=fopen(pc_file, "a");
    fprintf(fptr_pc,"enclave_name,page_fault_handler,integrity_protection,confidentiality,PAGES_READ,PAGES_WRITTEN,INIT_NUM_PAGES,PAGE_FAULT_COUNT,FREE_PAGES_FR,EXTENSIONS,TOTAL_PAGES,REAL_PAGES_READ,REAL_PAGES_WRITTEN,DUMMY_PAGES_READ,DUMMY_PAGES_WRITTEN,MAX_STASH_OCC,SUM_STASH_OCC,ORAM_ACC,HANDLER_INIT,ENCLAVE_INIT_CYC,ENCLAVE_RUN_CYC,ENCLAVE_WITHOUT_HANDLER_INIT_RUN_CYC,TOTAL_TIME_CYC,ENCLAVE_INIT_SEC,ENCLAVE_RUN_SEC,TOTAL_TIME_SEC\n");
    fclose(fptr_pc);
  }
  else
  {
    //printf("file present");
    fptr_pc=fopen(pc_file, "a");
    //fprintf(fptr_pc,"hello world\n");
    fclose(fptr_pc);
  }
  // exit(0);



  if (optind + 2 > argc) {
    std::cerr << "Usage error." << std::endl;
    print_usage(argv[0]);
    // will never get here because print_usage exits.
    exit(1);
  }
  strcpy(enc_name,argv[optind]);

  /*
  printf("enclave name          : %s\n", argv[optind]);
  printf("enclave library       : %s\n", argv[optind+1]);
  printf("page fault handling   : %d\n", args_user.page_fault_handler);
  printf("integrity-protection  : %d\n", args_user.integrity_protection);
  printf("confidentiality       : %d\n", args_user.confidentiality);
  printf("tracing               : %d\n", args_user.page_addr_tracing);
  printf("# of free pages       : %d\n", args_user.num_free_pages);
  printf("tracing file name     : %s\n", trace_file);
  printf("counter file name     : %s\n", pc_file);
  */

  // if(args_user.num_free_pages>MAX_PERMISSIBLE_FREE_PAGES)
  // {
  //   printf("[TEST-RUNNER] Number of free pages should be less than %lu\n", MAX_PERMISSIBLE_FREE_PAGES);
  //   exit(0);
  // }

  FILE *fptr2 = fopen(trace_file, "a");
  if (fptr2 == NULL)
  {
      printf("[UTM] Could not open file in print_counters\n");
      return 1;
  }
  fprintf(fptr2,"NAME:%s,",enc_name);
  fclose(fptr2);

  //printf("fault limit    : %d\n", args_user.fault_limit);
  //exit(0);

  /*
  char *fff= (char*)malloc(4096*280000);
  for(int lop=0;lop<4096*280000;lop++)
  {
    fff[lop]='a';
  }
  */



  int self_timing = 1;
  //int load_only = argv[4][0]-48;
  int load_only = 0;
  if(args_user.page_fault_handler==0)
  {
    p_arr=(pages *)malloc(190000*sizeof(pages));
  }

  if(args_user.page_fault_handler==2)
  {
    N=OPAM_TREE_SIZE;//till now 1<<14 is fine
    Z=1;
    L=17;
    tree_opam_md=(Bucket_opam_md*)malloc(OPAM_TREE_SIZE*sizeof(Bucket_opam_md));
    tree_opam=(Bucket_opam*)malloc(OPAM_TREE_SIZE*sizeof(Bucket_opam));

  }
  else if(args_user.page_fault_handler==1)
  {
    N=ORAM_TREE_SIZE;
    Z=4;
    L=4;//4
    treearray=(Bucket_oram*)malloc( (ORAM_TREE_SIZE)*sizeof(Bucket_oram)   );
    if(treearray==NULL)
    {
      printf("[test runner] PATH ORAM TREE ALLOCATION FAILED\n");
    }
  }
  else if(args_user.page_fault_handler==4)
  {
    N=RORAM_TREE_SIZE;
    Z=RORAM_BUCKET_SIZE;//34
    L=3;//3 4 7
    tree_roram_d=NULL;
    tree_roram_d= (Bucket_roram *)malloc((RORAM_TREE_SIZE)*sizeof(Bucket_roram));
    /*
    for(int i=0;i<(RORAM_TREE_SIZE)*sizeof(Bucket_roram);i++)
    {
      *((char*)(tree_roram_d)+i)=*((char*)(tree_roram_d)+i);
    }
    */
    if(tree_roram_d==NULL)
    {
      printf("[test runner] RING ORAM TREE ALLOCATION FAILED\n");
    }

  }
  else if(args_user.page_fault_handler==5)
  {
    printf("[testrunner] Write-only ORAM chosen as Page-fault handler\n");
    p_arr=(pages *)malloc(190000*sizeof(pages));
  }

  unsigned long long cycles1,cycles2,cycles3,cycles4;

  auto start_sec = high_resolution_clock::now();

  Keystone enclave;
  Params params;
  // params.setUntrustedSize(4096*(1024));//32k 8 pages
  params.setUntrustedSize(4096*(4096)*2); //32MB
  params.setFreeMemSize(129660*1024);//129660
  // params.setFreeMemSize(4096*(129660-500+50000));//129660
  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles1));
  }

  enclave.init(argv[optind], argv[optind+1], params);
  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles2));
  }

  edge_init(&enclave);
  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles3));
  }

  auto stop_enc_init_sec = high_resolution_clock::now();

  if( !load_only )
    enclave.run();

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles4));
    printf("[keystone-test] Init: %llu cycles\r\n", cycles2-cycles1);
    printf("[keystone-test] Runtime: %llu cycles\r\n", cycles4-cycles3);
    printf("[keystone-test] Total: %llu cycles\r\n", cycles4-cycles1);
  }
  auto stop_enc_fin_sec = high_resolution_clock::now();
FILE *fptr = fopen(trace_file, "a");
if (fptr == NULL)
{
    printf("[UTM] Could not open file in print_counters\n");
    return 1;
}

fprintf(fptr,"\n");
fclose(fptr);

fptr = fopen(pc_file, "a");
if (fptr == NULL)
{
    printf("[UTM] Could not open file in print_counters\n");
    return 1;
}

auto duration_enc_init = duration_cast<seconds>(stop_enc_init_sec-start_sec);
auto duration_enc_run = duration_cast<seconds>(stop_enc_fin_sec-stop_enc_init_sec);
auto duration_enc_total = duration_cast<seconds>(stop_enc_fin_sec-start_sec);
//printf("copy waste = 0x%lx\n",copy_waste );

fprintf(fptr,"%llu,%llu,%llu,%llu,%llu,%llu,%llu\n",(cycles2-cycles1),(cycles4-cycles3),( (cycles4-cycles3)-enc_run_wo_oram_init - copy_waste   ),(cycles4-cycles1),(duration_enc_init.count()),(duration_enc_run.count()),(duration_enc_total.count()));
fclose(fptr);



  return 0;
}
