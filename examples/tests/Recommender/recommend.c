#include "recommender.h"

#include <stdio.h>
#include <memory.h>
#include <math.h>

#include "matrix_factorization.h"
#include "matrix_factorization_bias.h"
#include "sparse_matrix.h"
#include "rating_estimator.h"
#include <edge_call.h>
#include "syscall.h"
#include "malloc.h"
// void
// print_recommended_items(rb_node_t* node, int depth)
// {
// 	int i = 0;
//
// 	if (node == NULL)
// 		return;
//
// 	for (i = 0; i < depth; i++)
// 		printf("->");
//
// 	printf("%f \n", ((recommended_item_t*) node->value)->rating);
//
// 	depth++;
//
// 	print_recommended_items(node->left_node, depth);
// 	print_recommended_items(node->right_node, depth);
// }
void exit(int status)
{
  ocall(1,"Exitting\n",10,NULL,0,0);
  ocall (11,&status,sizeof(int),NULL,0,0);
}

void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function)
{
	ocall(1,"Assertion failed in File(mentioned below):\n",44,NULL,0,0);
	ocall(1,file,strlen(file),NULL,0,0);
	ocall(2,&line,sizeof(int),NULL,0,0);
	exit(1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/*https://code.woboq.org/userspace/glibc/stdlib/qsort.c.html*/
#define SWAP(a, b, size)                                                      \
  do                                                                              \
    {                                                                              \
      size_t __size = (size);                                                      \
      char *__a = (a), *__b = (b);                                              \
      do                                                                      \
        {                                                                      \
          char __tmp = *__a;                                                      \
          *__a++ = *__b;                                                      \
          *__b++ = __tmp;                                                      \
        } while (--__size > 0);                                                      \
    } while (0)

typedef struct
{
  char *lo;
  char *hi;
} stack_node;

typedef int (*__compar_d_fn_t) (void *, void *);

#define STACK_SIZE        (CHAR_BIT * sizeof (size_t))
#define PUSH(low, high)        ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define        POP(low, high)        ((void) (--top, (low = top->lo), (high = top->hi)))
#define        STACK_NOT_EMPTY        (stack < top)
#define MAX_THRESH 4
#define CHAR_BIT 8

void qsort(void * pbase, size_t total_elems, size_t size,__compar_d_fn_t cmp)
{
  char *base_ptr = (char *) pbase;
  const size_t max_thresh = MAX_THRESH * size;
  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;
  if (total_elems > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;
      PUSH (NULL, NULL);
      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;
          /* Select median value from among LO, MID, and HI. Rearrange
             LO and HI so the three values are sorted. This lowers the
             probability of picking a pathological pivot value and
             skips a comparison for both the LEFT_PTR and RIGHT_PTR in
             the while loops. */
          char *mid = lo + size * ((hi - lo) / size >> 1);
          if ((*cmp) ((void *) mid, (void *) lo) < 0)
            SWAP (mid, lo, size);
          if ((*cmp) ((void *) hi, (void *) mid) < 0)
            SWAP (mid, hi, size);
          else
            goto jump_over;
          if ((*cmp) ((void *) mid, (void *) lo) < 0)
            SWAP (mid, lo, size);
        jump_over:;
          left_ptr  = lo + size;
          right_ptr = hi - size;
          /* Here's the famous ``collapse the walls'' section of quicksort.
             Gotta like those tight inner loops!  They are the main reason
             that this algorithm runs much faster than others. */
          do
            {
              while ((*cmp) ((void *) left_ptr, (void *) mid) < 0)
                left_ptr += size;
              while ((*cmp) ((void *) mid, (void *) right_ptr) < 0)
                right_ptr -= size;
              if (left_ptr < right_ptr)
                {
                  SWAP (left_ptr, right_ptr, size);
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                  left_ptr += size;
                  right_ptr -= size;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += size;
                  right_ptr -= size;
                  break;
                }
            }
          while (left_ptr <= right_ptr);
          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */
          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
                /* Ignore both small partitions. */
                POP (lo, hi);
              else
                /* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
            /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
              /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }
  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */
#define min(x, y) ((x) < (y) ? (x) : (y))
  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = min(end_ptr, base_ptr + max_thresh);
    char *run_ptr;
    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */
    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0)
        tmp_ptr = run_ptr;
    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);
    /* Insertion sort, running from left-hand-side up to right-hand-side.  */
    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
        tmp_ptr = run_ptr - size;
        while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0)
          tmp_ptr -= size;
        tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;
            trav = run_ptr + size;
            while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;
                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }
}

double log(double x)
{
	edge_data_t pkgstr;
  double number;
  ocall(26,&x,sizeof(double),&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

double exp(double x)
{
	edge_data_t pkgstr;
  double number;
  ocall(28,&x,sizeof(double),&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

typedef struct pow_struct
{
	double num1;
	double num2;
}pow_struct;

double pow(double x, double y)
{
	edge_data_t pkgstr;
  double number;
	pow_struct pass;
	pass.num1 = x;
	pass.num2 = y;
  ocall(29,&pass,sizeof(pow_struct),&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

int rand(void)
{
	edge_data_t pkgstr;
  int number;
  ocall(30,NULL,0,&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

double sqrt(double x)
{
	edge_data_t pkgstr;
  double number;
  ocall(32,&x,sizeof(double),&pkgstr,sizeof(edge_data_t),&number);
  return number;
}

void srand(unsigned int seed)
{
  ocall(31,&seed,sizeof(unsigned int),NULL,0,0);
}


//////////////////////////////////////////////////////////////////////////////////////////////

void __attribute__((__section__(".text._start"))) recommend_rating(){

	size_t i =0;
	learned_factors_t* learned;
	training_set_t* tset;
	recommended_items_t* r_items = NULL;

	learning_model_t model = {0};

	//Model configuration
	//Setup model parameters
	model_parameters_t params = { 0 };
  unsigned int num_users;
  unsigned int num_items;

  s_rand((unsigned long)&num_users, (unsigned long)sizeof(int));
  s_rand((unsigned long)&num_items, (unsigned long)sizeof(int));

  num_users=229;//added by me uncommet after roram trace
  num_items=229;//added by me uncommet after roram trace

  num_users = (num_users % 230) + 470;
  num_items = (num_items % 230) + 470;

  ocall(2,&num_users,sizeof(int),NULL,0,0);
  ocall(2,&num_items,sizeof(int),NULL,0,0);

  ocall(1,"##########\n",12,NULL,0,0);

	params.dimensionality = 100;
	params.iteration_number = 1;
	params.items_number = num_items;
	params.users_number = num_users;
	params.lambda = 0.005;
	params.training_set_size = (num_items-1) * (num_users-1);
	params.step = 0.005;

	//Use the basic matrix factorization model
	model.learning_algorithm = learn_mf_bias;
	model.rating_estimator   = estimate_rating_mf_bias;
	model.parameters		 = params;

	//Learning
	//Initialize a training set
	tset = init_training_set(&params);

  for(int i=0; i<(num_users-1);i++)
  {
    for(int j=0; j<(num_items-1); j++)
    {
      unsigned int rating_given;
      s_rand((unsigned long)&rating_given, (unsigned long)sizeof(int));

      rating_given=2*i*i + 3*j*j;//added by me uncommet after roram trace

      rating_given = rating_given % 6;
      set_known_rating(i,j,rating_given,tset);
    }
  }
	//set_known_rating(0, 0, 4, tset);
	//set_known_rating(0, 1, 1, tset);
	//set_known_rating(0, 2, 6, tset);

	//set_known_rating(1, 1, 1, tset);
	//set_known_rating(1, 0, 2, tset);

	compile_training_set(tset);

	learned = learn(tset, &model);

	rating_estimator_parameters_t rating_params = { 0, 0, learned, tset};

	//Rating estimation
	/*printf(	"users [0] item [0], rating = %f \n",
		 estimate_rating_from_factors(&rating_params, &model));*/
		ocall(1,"users[0] item[0], rating = \n",29,NULL,0,0);
		double estimated_rating = (double)estimate_rating_from_factors(&rating_params, &model);
		ocall(27,(void*)&estimated_rating,sizeof(double),NULL,0,0);

    rating_params.user_index = 0;
    rating_params.item_index = 1;

	/*printf(	"users [0] item [1], rating = %f \n",
		 estimate_rating_from_factors(&rating_params, &model));*/
		ocall(1,"users[0] item[1], rating = \n",29,NULL,0,0);
		estimated_rating = (double)estimate_rating_from_factors(&rating_params, &model);
		ocall(27,(void*)&estimated_rating,sizeof(double),NULL,0,0);

    rating_params.user_index = 0;
    rating_params.item_index = 2;

	/*printf(	"users [0] item [2], rating = %f \n",
		estimate_rating_from_factors(&rating_params, &model));*/
		ocall(1,"users[0] item[2], rating = \n",29,NULL,0,0);
		estimated_rating = (double)estimate_rating_from_factors(&rating_params, &model);
		ocall(27,(void*)&estimated_rating,sizeof(double),NULL,0,0);

    rating_params.user_index = 1;
    rating_params.item_index = 1;

	/*printf(	"users [1] item [1], rating = %f \n",
		estimate_rating_from_factors(&rating_params, &model));*/
		ocall(1,"users[1] item[1], rating = \n",29,NULL,0,0);
		estimated_rating = (double)estimate_rating_from_factors(&rating_params, &model);
		ocall(27,(void*)&estimated_rating,sizeof(double),NULL,0,0);

    rating_params.user_index = 1;
    rating_params.item_index = 0;

	/*printf(	"users [1] item [0], rating = %f \n",
		 estimate_rating_from_factors(&rating_params, &model));*/
		 ocall(1,"users[1] item[0], rating = \n",29,NULL,0,0);
		 estimated_rating = (double)estimate_rating_from_factors(&rating_params, &model);
 		ocall(27,(void*)&estimated_rating,sizeof(double),NULL,0,0);

	// rating_params.user_index = 0;
	r_items = recommend_items(&rating_params, &model);

	for (i = 0; i < r_items->items_number; i++)
	{
		//printf("%f \n", r_items->items[i].rating);
		double rated = (double)(r_items->items[i].rating);
		ocall(27,(void*)&rated,sizeof(double),NULL,0,0);
	}

	free_recommended_items(r_items);
	free_learned_factors(learned);
	free_training_set(tset);


  char* j= (char*)0;
  *j=*j;


	EAPP_RETURN(0);
}
