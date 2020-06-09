#ifndef __COMMON_H__
#define __COMMON_H__

#define PAGE_BITS 12
#define PAGE_SIZE (1UL<<PAGE_BITS)
#define ROUND_UP(n, b) (((((n) - 1ul) >> (b)) + 1ul) << (b))
#define ROUND_DOWN(n, b) (n & ~((2 << (b-1)) - 1))
#define PAGE_DOWN(n) ROUND_DOWN(n, PAGE_BITS)
#define PAGE_UP(n) ROUND_UP(n, PAGE_BITS)

#define BOOST_STRINGIZE(X) BOOST_DO_STRINGIZE(X)
#define BOOST_DO_STRINGIZE(X) #X

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

#define MSG(str) "[Keystone SDK] " __FILE__ ":" BOOST_STRINGIZE(__LINE__) " : " str
#define ERROR(str, ...) fprintf(stderr, MSG(str) "\n", ##__VA_ARGS__)
#define PERROR(str) perror(MSG(str))
#define IS_ALIGNED(x, align) (!((x)&(align-1)))

/* Currently we have only one command avaiable from the enclave.
 * We need to move it out to other header file (e.g., keystone-sm.h) */
#define KEYSTONE_ENCLAVE_INTERRUPTED  2
#define KEYSTONE_ENCLAVE_EDGE_CALL_HOST  11

/* Types */
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;
#define AES_KEYLEN 16
typedef enum {
  KEYSTONE_ERROR=-1,
  KEYSTONE_SUCCESS,
  KEYSTONE_NOT_IMPLEMENTED,
} keystone_status_t;

#define MAX_PERMISSIBLE_FREE_PAGES 123000//125500
//-----------------------------------PATH ORAM DATA STRUCTURES-------------------------------------------


typedef struct Block_oram
{
  uintptr_t enclav_vaddr;
  uintptr_t version;
  char  page[PAGE_SIZE];
  uint8_t iv[AES_KEYLEN];
  char p_hash[32];
} Block_oram;

typedef struct Bucket_oram
{
  Block_oram blocks[4];

} Bucket_oram;

#define ORAM_TREE_SIZE 70000//70000 87390 1400
//extern Bucket_oram treearray[ORAM_TREE_SIZE];
extern Bucket_oram *treearray;
extern uintptr_t treearray_len;
//------------------------------------------------------------------------------

//-----------------------------------OPAM DATA STRUCTURES-------------------------------------------
#define OPAM_STASH_SIZE 48
#define OPAM_TREE_SIZE  262144 // 1<<18
typedef struct Block_opam_md
{
  uintptr_t enclav_vaddr;
  uintptr_t version;
  uintptr_t tree_index;
  uintptr_t stash_index;
  uint8_t iv[AES_KEYLEN];
  char p_hash[32];
  uint8_t ivd[AES_KEYLEN];// IV is length 16 bytes
  char p_hash_d[32];
} Block_opam_md;

typedef struct Bucket_opam_md
{
  Block_opam_md blocks[1];


} Bucket_opam_md;

//extern Bucket_opam_md tree_opam_md[OPAM_TREE_SIZE];
extern Bucket_opam_md *tree_opam_md;
extern uintptr_t tree_opam_md_len;

//extern Block_opam_md stash_opam_md[OPAM_STASH_SIZE];
//extern uintptr_t stash_opam_md_len;

typedef struct Block_opam
{
  char  page[PAGE_SIZE];
} Block_opam;

typedef struct Bucket_opam
{
  Block_opam blocks[1];

} Bucket_opam;

//extern Bucket_opam tree_opam[OPAM_TREE_SIZE];
extern Bucket_opam *tree_opam;
extern uintptr_t tree_opam_len;

//extern Block_opam stash_opam[OPAM_STASH_SIZE];
//extern uintptr_t stash_opam_len;

//-----------------------------RORAM DATA STRUCTURES-------------------------------------------------
#define STASH_SIZE_RORAM 1//45
#define RORAM_BUCKET_SIZE 64//64 17(for 4ary)
#define RORAM_TREE_SIZE 4400//4400 4690 21900
typedef struct Block_roram
{
    uintptr_t address;
    uintptr_t version;
    char data[PAGE_SIZE];
    uint8_t iv[16];// IV is length 16 bytes
    char p_hash[32];// 32 bytes of hmac is used
} Block_roram;


typedef struct Bucket_roram
{
    Block_roram blocks[RORAM_BUCKET_SIZE];
} Bucket_roram;


typedef struct Bucket_roram_md
{
    uintptr_t count;
    char valids[RORAM_BUCKET_SIZE];
    uintptr_t addr[RORAM_BUCKET_SIZE];
//    uintptr_t leaves[RORAM_BUCKET_SIZE];
    uintptr_t ptrs[RORAM_BUCKET_SIZE];
    //uint8_t iv[16];// IV is length 16 bytes
    //char p_hash[32];// 32 bytes of hmac is used
} Bucket_roram_md;

//extern Bucket_roram_md tree_roram_md[RORAM_TREE_SIZE];
//extern Bucket_roram tree_roram_d[RORAM_TREE_SIZE];
extern Bucket_roram *tree_roram_d;

//--------------------------------------------------------------------------



extern uintptr_t N;
extern uintptr_t Z;
extern uintptr_t L;
//extern uintptr_t args_user[5];

typedef enum  { PAGE_FAULT_HANDLER_SIMPLE=0, PAGE_FAULT_HANDLER_ORAM=1,
       PAGE_FAULT_HANDLER_OPAM=2, PAGE_FAULT_HANDLER_ENC_ADDR=3 }page_fault_type;
typedef struct enclave_options_t {
  page_fault_type page_fault_handler;
  uint8_t integrity_protection;
  uint8_t confidentiality;
  int num_free_pages;
  uint8_t victim_cache;
  uint8_t page_addr_tracing;
  uint8_t debug_mode;
  uint8_t tree_exc;
  int fault_limit;
} enclave_options;

extern enclave_options args_user;
extern char trace_file[100];
extern char pc_file[100];
extern char enc_name[100];
#define NO_OF_COUNTERS 16


typedef struct counter_t
{
  uintptr_t count;
  char name[50];
} counter;





typedef struct pages_at
{
  uintptr_t address;
  uintptr_t dummy;
  char data[PAGE_SIZE];
  char hmac[32];
}pages_at;

typedef struct pages
{
  uintptr_t address;
  char data[PAGE_SIZE];
  char hmac[32];
  uintptr_t ver_num;
  uintptr_t dummy;
}pages;


extern pages *p_arr;
extern uintptr_t enc_run_wo_oram_init;
extern uintptr_t copy_waste;



#define NO_ORAM 0
#define PATH_ORAM 1
#define OPAM 2
#define ENC_PFH 3
#define RORAM 4
#define WORAM 5





#endif
