
#include "eapp_utils.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "malloc.h"
#include "syscall.h"

typedef struct node
{
	int data;
	struct node *left,*right;
	int ht;
}node;

node *insert(node *,int);
node *Delete(node *,int);
void preorder(node *);
void inorder(node *);
int height( node *);
node *rotateright(node *);
node *rotateleft(node *);
node *RR(node *);
node *LL(node *);
node *LR(node *);
node *RL(node *);
int BF(node *);

void EAPP_ENTRY eapp_entry()
{
	node *root=NULL;
	int x,n,i;

  int max_limit=30000;max_limit=max_limit;
  s_rand((unsigned long)&n, (unsigned long)sizeof(int));
  if(n<0)n=n*-1;
  n=n%max_limit;
	n=70000+n;


	n=100000;//change to 100000


  ocall(2, &n, sizeof(int), 0, 0,0);
	root=NULL;
  int *data = malloc(n * sizeof(int));
	int sign=1;sign=sign;
	int sign2=-1;
	int v=1;
	for(i=0;i<n;i++)
	{
		s_rand((unsigned long)&x, (unsigned long)sizeof(int));
    //x=i;
		//root=insert(root,x);//uncommetn after roram trace

    data[i]= ((i%1432)*sign2+v)*sign  ; sign=-sign;

		//data[i]=x;//commetn after roram trace
		root=insert(root,data[i]);//commetn after roram trace

		if(i%3==0) sign2=-sign2;
		v++;
	}

	if(n&2)
	{
  	for(i=0;i<n;i++)
  	{
				root=Delete(root,data[i]);
		}
	}
	else
	{
		for(i=n-1;i>=0;i--)
  	{
				root=Delete(root,data[i]);
		}
	}
  char *success_msg="AVl TREE OPS DONE\n";success_msg=success_msg;
  unsigned long retval;
  ocall(1, success_msg, 19, &retval ,sizeof(unsigned long),0);
  retval=retval;;

	char* j= (char*)0x0;
	*j=*j;

	EAPP_RETURN(0);
}

node * insert(node *T,int x)
{
	if(T==NULL)
	{
		T=(node*)malloc(sizeof(node));
		T->data=x;
		T->left=NULL;
		T->right=NULL;
	}
	else
  {
		if(x > T->data)		// insert in right subtree
		{
			T->right=insert(T->right,x);
			if(BF(T)==-2)
      {
				if(x>T->right->data)
					T=RR(T);
				else
					T=RL(T);
      }
		}
		else
			if(x<T->data)
			{
				T->left=insert(T->left,x);
				if(BF(T)==2)
        {
					if(x < T->left->data)
						T=LL(T);
					else
						T=LR(T);
        }
			}
  }

		T->ht=height(T);

		return(T);
}

node * Delete(node *T,int x)
{
	node *p;

	if(T==NULL)
	{
		return NULL;
	}
	else
		if(x > T->data)		// insert in right subtree
		{
			T->right=Delete(T->right,x);
			if(BF(T)==2)
      {
				if(BF(T->left)>=0)
					T=LL(T);
				else
					T=LR(T);
      }
		}
		else

			if(x<T->data)
			{
				T->left=Delete(T->left,x);
				if(BF(T)==-2)	//Rebalance during windup
        {
					if(BF(T->right)<=0)
						T=RR(T);
					else
						T=RL(T);
        }
			}
			else
			{
				//data to be deleted is found
				if(T->right!=NULL)
				{	//delete its inorder succesor
					p=T->right;

					while(p->left!= NULL)
						p=p->left;

					T->data=p->data;
					T->right=Delete(T->right,p->data);

					if(BF(T)==2)//Rebalance during windup
          {
          	if(BF(T->left)>=0)
							T=LL(T);
						else
							T=LR(T);
          }
				}

				else
					return(T->left);
			}
	T->ht=height(T);
	return(T);
}

int height(node *T)
{
	int lh,rh;
	if(T==NULL)
		return(0);

	if(T->left==NULL)
		lh=0;
	else
		lh=1+T->left->ht;

	if(T->right==NULL)
		rh=0;
	else
		rh=1+T->right->ht;

	if(lh>rh)
		return(lh);

	return(rh);
}

node * rotateright(node *x)
{
	node *y;
	y=x->left;
	x->left=y->right;
	y->right=x;
	x->ht=height(x);
	y->ht=height(y);
	return(y);
}

node * rotateleft(node *x)
{
	node *y;
	y=x->right;
	x->right=y->left;
	y->left=x;
	x->ht=height(x);
	y->ht=height(y);

	return(y);
}

node * RR(node *T)
{
	T=rotateleft(T);
	return(T);
}

node * LL(node *T)
{
	T=rotateright(T);
	return(T);
}

node * LR(node *T)
{
	T->left=rotateleft(T->left);
	T=rotateright(T);

	return(T);
}

node * RL(node *T)
{
	T->right=rotateright(T->right);
	T=rotateleft(T);
	return(T);
}

int BF(node *T)
{
	int lh,rh;
	if(T==NULL)
		return(0);

	if(T->left==NULL)
		lh=0;
	else
		lh=1+T->left->ht;

	if(T->right==NULL)
		rh=0;
	else
		rh=1+T->right->ht;

	return(lh-rh);
}

void preorder(node *T)
{
	if(T!=NULL)
	{
    T->data=T->data;
		//printf("%d(Bf=%d)",T->data,BF(T));
		preorder(T->left);
		preorder(T->right);
	}
}

void inorder(node *T)
{
	if(T!=NULL)
	{
		inorder(T->left);
    T->data=T->data;
		//printf("%d(Bf=%d)",T->data,BF(T));
		inorder(T->right);
	}
}
