/* xz driver code for SPEC CPU
 *
 * $Id: spec.c 5572 2017-04-10 22:49:59Z JohnHenning $
 */

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG
#define ALREADY_SPEC
#include <stdint.h>
#if defined(SPEC_WINDOWS) && defined(SPEC_NEED_INTTYPES_H)
# include "win32/inttypes.h"
#else
# include <inttypes.h>
#endif
#include <stdlib.h>
#include <edge_call.h>
#include "syscall.h"
#include "spec_mem_io.h"
#include "sha512.h"
#include "lzma.h"

#define SHASIZE 512

void __attribute__((__section__(".text._start"))) execute_benchmark() {
    int argc = 7;
    int level = 0, i = 0;
    unsigned extreme = 0;
    int64_t input_size = 0;
    int64_t compressed_min = -1,
            compressed_max = -1;
    char *input_name = NULL;
    unsigned char *sha = NULL;
    unsigned char *input_sha = NULL, *check_sha = NULL;
# if defined(DEBUG_DUMP)
    int fd = -1;
# endif
    SPEC_MEMOP_PAUSE_SETUP

    SPEC_PREALLOC_PAUSE("main", "malloc", sizeof(unsigned char) * SHASIZE/8);
    check_sha = (unsigned char *)malloc(sizeof(unsigned char) * SHASIZE/8);
    if (check_sha == NULL) {
        ocall (1,"main: Error mallocing memory for SHA-512 sum!\n",47,NULL,0,0);
        //exit(1);
        EAPP_RETURN(1);
    }
    SPEC_POSTALLOC_PAUSE("main", "malloc", sizeof(unsigned char) * SHASIZE/8);

    /*if (argc > 1) input_name      = argv[1];
    if (argc > 2) input_size      = strtol(argv[2], NULL, 10);
    if (argc > 3) {
      if (strlen(argv[3]) * 4 != SHASIZE) {
        focall(stderr, "Error: Supplied original hash value is not the correct length to be SHA-%d\n", SHASIZE);
        exit(1);
      } else
                  sha             = sum_str_to_hex(argv[3], strlen(argv[3]) * 4);
    }
    if (argc > 4) compressed_min  = strtol(argv[4], NULL, 10);
    if (argc > 5) compressed_max  = strtol(argv[5], NULL, 10);
    if (compressed_max < 0)
      compressed_max = compressed_min;*/
    input_name = "SAT.tar.xz";
    input_size = 1;
    int hash_file = open2("SAT.tar.xz.hash","r");
    char hash_input[129];
    read2(hash_file,hash_input,128);
    hash_input[128]='\0';
    close2(hash_file);
    sha = sum_str_to_hex(hash_input,strlen(hash_input)*4);
    compressed_min = -1;
    compressed_max = 10;

    if (input_name == NULL || input_size <= 0 || sha == NULL || compressed_min == 0 || compressed_max == 0 || argc < 6) {
        /*ocall("Usage: %s <input filename> <input size> <input raw SHA-%d> <compressed minimum> <compressed maximum> <compression level, ...>\n", "xz_r", SHASIZE);
        ocall("input_name: %s\n",input_name);
        ocall("input_size: %ld\n",input_size);
        ocall("SHA: %s\n",sha);
        ocall("Compressed_min: %ld",compressed_min);
        ocall("Compressed_max: %ld",compressed_max);
        ocall("argc: %d",argc);*/
        ocall(1,"Input Wrong\n",13,NULL,0,0);
        //exit(1);
        EAPP_RETURN(1);
    }
    //ocall("SPEC CPU XZ driver: input=%s insize=%" PRId64 "\n", input_name, input_size);

    spec_fd = spec_mem_alloc_fds(MAX_SPEC_FD);
    if (spec_fd == NULL) {
        ocall(1, "Error allocating in-memory file descriptors\n",45,NULL,0,0);
        //exit(1);
        EAPP_RETURN(1);
    }

    // Initial buffer size is <input_size> MiB or FILE_CHUNK, whichever is
    // larger (will be grown if necessary, but has to be able to hold at
    // least one read's worth of data)
    if (input_size * MiB < FILE_CHUNK) {
        spec_fd[0].limit = spec_fd[1].limit = FILE_CHUNK;
    } else {
        spec_fd[0].limit = spec_fd[1].limit = input_size * MiB;
    }
    spec_init();

    debug_time();
    ocall(1,"Loading Input Data\n",20,NULL,0,0);
    input_sha = spec_load(0, input_name, input_size * MiB, 1, sha, SHASIZE);
    //ocall("Input data %" PRId64 " bytes in length\n", spec_fd[0].len);

#ifdef DEBUG_DUMP
  #undef DEBUG_DUMP
#endif

#if defined(DEBUG_DUMP)
    fd = open ("out.uncompressed", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    write(fd, spec_fd[0].buf, spec_fd[0].len);
    close(fd);
#endif

    /* Mark the input files as open, as open() is never actually called */
    spec_fd[0].open = spec_fd[1].open = spec_fd[2].open = 1;

    spec_initbufs();

    // Loop through requested compression levels
    char comp_level[] = "6";
    for(i = 6; i < argc; i++) {

        level = comp_level[0] - '0';
        if (comp_level[1] == 'e')
            extreme = LZMA_PRESET_EXTREME;
        else
            extreme = 0;

        if (level < 0 || level > 9) {
            ocall(1,"Error: compression level not valid\n", 36,NULL,0,0);
            //ocall("Usage: %s <input filename> <input size> <input raw SHA-%d> <compressed minimum> <compressed maximum> <compression level, ...>\n", "xz_r", SHASIZE);
            //exit(1);
            EAPP_RETURN(1);
        }

        debug_time();
        ocall(1,"Compressing Input Data\n",24,NULL,0,0);

        spec_compress(0,1, level | extreme);

        debug_time();
        //focall(stderr, "Compressed data %" PRId64 " bytes in length\n", spec_fd[1].len);
        ocall(1,"Compressed Data\n",17,NULL,0,0);
        /*if (compressed_min > 0) {
          if (spec_fd[1].len >= compressed_min && spec_fd[1].len <= compressed_max) {
            if (compressed_min == compressed_max)
              //ocall("Compressed data length is %" PRId64 " bytes in length\n",
                //    compressed_min);
            else
              //ocall("Compressed data is between %" PRId64 " and %" PRId64 " bytes in length\n",
                  //  compressed_min, compressed_max);
          } else {
            if (compressed_min == compressed_max)
              //ocall("Compressed data length of %" PRId64 " does not match expected length of %" PRId64 " bytes\n",
                //  spec_fd[1].len, compressed_min);
            else
              //ocall("Compressed data length of %" PRId64 " is outside the allowable range of %" PRId64 " to %" PRId64 " bytes\n",
                //  spec_fd[1].len, compressed_min, compressed_max);
          }
        }*/
        ocall(1,"Finished compressing data\n",27,NULL,0,0);

/*#if defined(DEBUG_DUMP)
        {
            char buf[256];
            socall(buf, "out.uncompress.%d%s", level, extreme ? "e" : "");
            fd = open (buf, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            write(fd, spec_fd[1].buf, spec_fd[1].len);
            close(fd);
        }
#endif*/

        spec_reset(0);
        spec_rewind(1);

        debug_time();
        ocall(1,"Uncompressing previously compressed data\n",42,NULL,0,0);
        spec_uncompress(1, 0);
        debug_time();
        //ocall("Uncompressed data %" PRId64 " bytes in length\n", spec_fd[0].len);

/*#if defined(DEBUG_DUMP)
        {
            char buf[256];
            socall(buf, "out.uncompress.%d%s", level, extreme ? "e" : "");
            fd = open (buf, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            write(fd, spec_fd[0].buf, spec_fd[0].len);
            close(fd);
        }
#endif*/

        spec_sum(0, check_sha, SHASIZE);
        if (compare_sum(input_sha, check_sha, SHASIZE)) {
            ocall(1,"Computed SHA-512 sum for decompression step did not match expected\n",68,NULL,0,0);
            /*ocall(1,"  Generated: ");
            print_sum(stdout, check_sha, SHASIZE);
            ocall("\n");
            ocall("   Expected: ");
            print_sum(stdout, input_sha, SHASIZE);
            ocall("\n");*/
            //exit (0);
            EAPP_RETURN(0);
        }
        debug_time();
        ocall(1,"Uncompressed data compared correctly\n",38,NULL,0,0);
        spec_reset(1);
        spec_rewind(0);
        //ocall("Tested %" PRId64 " MiB buffer: OK!\n", input_size);
        //fflush(stdout);
    }


    char *s= (char*)0;
    *s=*s;
    EAPP_RETURN(0);
}

#if defined(__cplusplus)
}
#endif
