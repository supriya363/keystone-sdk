/*
 * This is a test program for avl_tree.h and avl_tree.c.  Compile with:
 *
 *	$ gcc test.c avl_tree.c -o test -std=c99 -Wall -O2
 *
 * The test strategy isn't very sophisticated; it just relies on repeated random
 * operations to cover as many cases as possible.  Feel free to improve it.
 *
 * -----------------------------------------------------------------------------
 *
 * Written in 2014-2016 by Eric Biggers <ebiggers3@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide via the Creative Commons Zero 1.0 Universal Public Domain
 * Dedication (the "CC0").
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the CC0 for more details.
 *
 * You should have received a copy of the CC0 along with this software; if not
 * see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include "avl_tree.h"
#include "eapp_utils.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "malloc.h"
#include "syscall.h"


/* Change this to 0, and this turns into a benchmark program.  */
#define VERIFY 0
int rand()
{
	return 1;
}
struct test_node {
	int n;
	struct avl_tree_node node;
};

static struct avl_tree_node *root = NULL;

static struct test_node *nodes;
static int node_idx;

static inline int
avl_get_balance_factor(const struct avl_tree_node *node)
{
	return (int)(node->parent_balance & 3) - 1;
}

#define TEST_NODE(__node) avl_tree_entry(__node, struct test_node, node)
#define INT_VALUE(node) TEST_NODE(node)->n
#define HEIGHT(node) ((node) ? TEST_NODE(node)->height : 0)
#define max(a, b) ((a) > (b) ? (a) : (b))

static int
cmp_int_nodes(const struct avl_tree_node *node1,
	      const struct avl_tree_node *node2)
{
	return INT_VALUE(node1) - INT_VALUE(node2);
}

static void
insert(int n)
{
	struct test_node *i = &nodes[node_idx++];
	i->n = n;
	avl_tree_insert(&root, &i->node, cmp_int_nodes);
}

static struct test_node *
lookup(int n)
{
	struct test_node query;
	struct avl_tree_node *result;

	query.n = n;

	result = avl_tree_lookup_node(root, &query.node, cmp_int_nodes);

	return result ? TEST_NODE(result) : NULL;
}

static void
deletenode(struct test_node *node)
{
	avl_tree_remove(&root, &node->node);
}

static void
delete(int n)
{
	struct test_node *node;

	node = lookup(n);
	//assert(node);
	deletenode(node);
}
/*
static void
shuffle(int data[], int count)
{
	for (int i = 0; i < count; i++) {
		const int x = rand() % count;
		const int v0 = data[0];
		const int vx = data[x];
		data[0] = vx;
		data[x] = v0;
	}
}
*/


static void
test(int data[], int count)
{
	//shuffle(data, count);
	//return ;
	node_idx = 0;

	/* Insert the data, checking the AVL tree invariants after each step.  */
	for (int i = 0; i < count; i++) {
		insert(data[i]);
	#if VERIFY
		setheights();
		checktree();
		verify(data, i + 1);
	#endif
	}

	/* Delete the data in random order, checking the AVL tree invariants
	 * after each step.  */
	//shuffle(data, count);


	for (int i = 0; i < count ; i++) {
		delete(data[i]);
	#if VERIFY
		setheights();
		checktree();
		verify(data + (i + 1), count - (i + 1));
	#endif
	}


}

void EAPP_ENTRY eapp_entry()
{
	//const int num_iterations = 1;
	int max_limit=2000;max_limit=max_limit;
	int v=0;
  s_rand((unsigned long)&v, (unsigned long)sizeof(int));
	if(v<0)v=v*-1;
	//int max_node_count = v%max_limit;
	int max_node_count = 1000;
	int *data = malloc(max_node_count * sizeof(nodes[0]));;
	ocall(2, &max_node_count, sizeof(int), 0, 0,0);
	for (int i = 0; i < max_node_count; i++)
		s_rand((unsigned long)&data[i], (unsigned long)sizeof(int));
		//data[i]=i;
	//test(data,  max_node_count-1);

	nodes = malloc(max_node_count * sizeof(nodes[0]));

	// printf("Using max_node_count=%d\n", max_node_count);


	root = NULL;

	test(data, max_node_count-1);
	if(lookup(500)!=NULL)
	{
		int *badaddress=(int*)0x999999;
	  int c=*badaddress;
	  c=c+1;
	}
		//shuffle(data, max_node_count);


	// printf("Done.\n");

	//free(nodes);

	EAPP_RETURN(0);
}
