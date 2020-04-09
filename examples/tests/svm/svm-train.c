
extern "C"
{
  #include <edge_call.h>
  #include "eapp_utils.h"
  #include "syscall.h"

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>
}

#include "svm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

void exit(int status)
{
  ocall(1,"Exitting\n",10,NULL,0,0);
  ocall (11,&status,sizeof(int),NULL,0,0);
}

void print_null(const char *s) {}

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

void exit_with_help()
{
	ocall(1,"Invalid Usage\n",15,NULL,0,0);
	/*printf(
	"Usage: svm-train [options] training_set_file [model_file]\n"
	"options:\n"
	"-s svm_type : set type of SVM (default 0)\n"
	"	0 -- C-SVC		(multi-class classification)\n"
	"	1 -- nu-SVC		(multi-class classification)\n"
	"	2 -- one-class SVM\n"
	"	3 -- epsilon-SVR	(regression)\n"
	"	4 -- nu-SVR		(regression)\n"
	"-t kernel_type : set type of kernel function (default 2)\n"
	"	0 -- linear: u'*v\n"
	"	1 -- polynomial: (gamma*u'*v + coef0)^degree\n"
	"	2 -- radial basis function: exp(-gamma*|u-v|^2)\n"
	"	3 -- sigmoid: tanh(gamma*u'*v + coef0)\n"
	"	4 -- precomputed kernel (kernel values in training_set_file)\n"
	"-d degree : set degree in kernel function (default 3)\n"
	"-g gamma : set gamma in kernel function (default 1/num_features)\n"
	"-r coef0 : set coef0 in kernel function (default 0)\n"
	"-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)\n"
	"-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)\n"
	"-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)\n"
	"-m cachesize : set cache memory size in MB (default 100)\n"
	"-e epsilon : set tolerance of termination criterion (default 0.001)\n"
	"-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)\n"
	"-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)\n"
	"-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)\n"
	"-v n: n-fold cross validation mode\n"
	"-q : quiet mode (no outputs)\n"
);*/
	exit(1);
}

void exit_input_error(int line_num)
{
	ocall(1,"Wrong format\n",14,NULL,0,0);
	/*fprintf(stderr,"Wrong input format at line %d\n", line_num);*/
	exit(1);
}

void parse_command_line(char *input_file_name, char *model_file_name);
void read_problem(const char *filename);
void do_cross_validation();

struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space;
int cross_validation;
int nr_fold;

static char *line = NULL;
static int max_line_len;

extern "C"
{
  char* readline(FILE *input);
  double strtod(const char* str, char** endptr);
  long strtol(const char *nptr, char **endptr, register int base);
  int ISUPPER(char c);
  int ISALPHA(char c);
  int ISDIGIT(char c);
  int ISSPACE(char c);
  char *strrchr(const char *s, int c);
  char *strtok(char *str,const char *delim);
}

/*  https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c */

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define	LONG_MAX	((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#endif

int ISSPACE(char c)
{
  if(c == ' ')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISDIGIT(char c)
{
  if((int)c <= 57 && (int)c >= 48)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISALPHA(char c)
{
  if((int)c <= 122 && (int)c >= 97)
  {
    return 1;
  }
  else if((int)c <= 90 && (int)c >= 65)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ISUPPER(char c)
{
  if((int)c <= 90 && (int)c >= 65)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/* https://clc-wiki.net/wiki/strtok */

char *strchr(const char *s, int c)
{
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *)s;
}

size_t strcspn(const char *s1, const char *s2)
{
    size_t ret=0;
    while(*s1)
        if(strchr(s2,*s1))
            return ret;
        else
            s1++,ret++;
    return ret;
}

size_t strspn(const char *s1, const char *s2)
{
    size_t ret=0;
    while(*s1 && strchr(s2,*s1++))
        ret++;
    return ret;
}

char *strtok(char * str, const char * delim)
{
    static char* p=0;
    if(str)
        p=str;
    else if(!p)
        return 0;
    str=p+strspn(p,delim);
    p=str+strcspn(str,delim);
    if(p==str)
        return p=0;
    p = *p ? *p=0,p+1 : 0;
    return str;
}
/*char * strtok(char * str,const char *comp)
{
	static int pos;
	static char *s;
	int i =0, start = pos;

	// Copying the string for further calls of strtok
	if(str!=NULL)
		s = str;

  //ocall(1,s,strlen(s),NULL,0,0);
  //ocall(1,comp,strlen(comp),NULL,0,0);

	i = 0;
	int j = 0;
	//While not end of string
	while(s[pos] != '\0')
	{
		j = 0;
		//Comparing of one of the delimiter matches the character in the string
		while(comp[j] != '\0')
		{
			//Pos point to the next location in the string that we have to read
			if(s[pos] == comp[j])
			{
				//Replace the delimter by \0 to break the string
				//s[pos] = '\0';
				pos = pos+1;
				//Checking for the case where there is no relevant string before the delimeter.
				//start specifies the location from where we have to start reading the next character
				if(s[start] != '\0')
        {
          char * ans = (char *)malloc(pos-start+1);
          strncpy(ans,s+start,pos-start);
          ans[pos-start] = '\0';
          return ans;
					//return (&s[start]);
        }
				else
				{
					// Move to the next string after the delimiter
					start = pos;
					// Decrementing as it will be incremented at the end of the while loop
					pos--;
					break;
				}
        //return &s[start];
			}
			j++;
		}
		pos++;
	}//End of Outer while
	//s[pos] = '\0';
	if(s[start] == '\0')
  {
    //ocall(2,&start,sizeof(start),NULL,0,0);
		return NULL;
  }
	else
  {
    char * ans = (char *)malloc(pos-start+1);
    strncpy(ans,s+start,pos-start);
    ans[pos-start] = '\0';
    return ans;
		//return &s[start];
  }
}*/

long strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (ISSPACE(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (ISDIGIT(c))
			c -= '0';
		else if (ISALPHA(c))
			c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		//errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

int isspace(int c)
{
  char ch = (char)c;
  if( ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


/* Taken from https://codereview.stackexchange.com/questions/44869/implement-strtod-parsing */
double strtod(const char* str, char** endptr)
{
    double result = 0.0;
    char signedResult = '\0';
    char signedExponent = '\0';
    int decimals = 0;
    bool isExponent = false;
    bool hasExponent = false;
    bool hasResult = false;
    // exponent is logically int but is coded as double so that its eventual
    // overflow detection can be the same as for double result
    double exponent = 0;
    char c;
    for (; '\0' != (c = *str); ++str)
    {
        if ((c >= '0') && (c <= '9'))
        {
            int digit = c - '0';
            if (isExponent)
            {
                exponent = (10 * exponent) + digit;
                hasExponent = true;
            }
            else if (decimals == 0)
            {
                result = (10 * result) + digit;
                hasResult = true;
            }
            else
            {
                result += (double)digit / decimals;
                decimals *= 10;
            }
            continue;
        }
        if (c == '.')
        {
            if (!hasResult)
            {
                // don't allow leading '.'
                break;
            }
            if (isExponent)
            {
                // don't allow decimal places in exponent
                break;
            }
            if (decimals != 0)
            {
                // this is the 2nd time we've found a '.'
                break;
            }
            decimals = 10;
            continue;
        }
        if ((c == '-') || (c == '+'))
        {
            if (isExponent)
            {
                if (signedExponent || (exponent != 0))
                    break;
                else
                    signedExponent = c;
            }
            else
            {
                if (signedResult || (result != 0))
                    break;
                else
                    signedResult = c;
            }
            continue;
        }
        if (c == 'E')
        {
            if (!hasResult)
            {
                // don't allow leading 'E'
                break;
            }
            if (isExponent)
                break;
            else
                isExponent = true;
            continue;
        }
        // else unexpected character
        break;
    }
    if (isExponent && !hasExponent)
    {
        while (*str != 'E')
            --str;
    }
    if (!hasResult && signedResult)
        --str;

    if (endptr)
        *endptr = const_cast<char*>(str);
    for (; exponent != 0; --exponent)
    {
        if (signedExponent == '-')
            result /= 10;
        else
            result *= 10;
    }
    if (signedResult == '-')
    {
        if (result != 0)
            result = -result;
        // else I'm not used to working with double-precision numbers so I
        // was surprised to find my assert for "-0" failing, saying -0 != +0.
    }
    return result;
}
static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

void __attribute__((__section__(".text._start"))) train_model()
{
	char input_file_name[1024];
	char model_file_name[1024];
	const char *error_msg;

	parse_command_line(input_file_name, model_file_name);
	read_problem(input_file_name);
	error_msg = svm_check_parameter(&prob,&param);

	if(error_msg)
	{
		//fprintf(stderr,"ERROR: %s\n",error_msg);
		ocall(1,"ERROR:\n",8,NULL,0,0);
		ocall(1,error_msg,strlen(error_msg),NULL,0,0);
		exit(1);
	}

	if(cross_validation)
	{
		do_cross_validation();
	}
	else
	{
		model = svm_train(&prob,&param);
    ocall(1,"Model not saved\n",17,NULL,0,0);
		/*if(svm_save_model(model_file_name,model))
		{
			//fprintf(stderr, "can't save model to file %s\n", model_file_name);
			ocall(1,"Can't save model\n",18,NULL,0,0);
			exit(1);
		}*/
		svm_free_and_destroy_model(&model);
	}
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);

	EAPP_RETURN(0);
}

void do_cross_validation()
{
	int i;
	int total_correct = 0;
	double total_error = 0;
	double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
	double *target = Malloc(double,prob.l);

	svm_cross_validation(&prob,&param,nr_fold,target);
	if(param.svm_type == EPSILON_SVR ||
	   param.svm_type == NU_SVR)
	{
		for(i=0;i<prob.l;i++)
		{
			double y = prob.y[i];
			double v = target[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}
		/*printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
			((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
			((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
		);*/
		double cvmse = total_error/prob.l;
		ocall(1,"Cross Validation Mean squared error =\n",39,NULL,0,0);
		ocall(27,&cvmse,sizeof(double),NULL,0,0);
		double cvscc = ((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy));
		ocall(1,"Cross Validation Squared correlation coefficient =\n",52,NULL,0,0);
		ocall(27,&cvscc,sizeof(double),NULL,0,0);
	}
	else
	{
		for(i=0;i<prob.l;i++)
			if(target[i] == prob.y[i])
				++total_correct;
		//printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
		double cvscc = 100.0*total_correct/prob.l;
		ocall(1,"Cross Validation Accuracy = \n",30,NULL,0,0);
		ocall(27,&cvscc,sizeof(double),NULL,0,0);
	}
	free(target);
}

void parse_command_line(char *input_file_name, char *model_file_name)
{
	int i;
	void (*print_func)(const char*) = NULL;	// default printing to stdout

	// default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	cross_validation = 0;

	// parse options
	/*for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();
		switch(argv[i-1][1])
		{
			case 's':
				param.svm_type = atoi(argv[i]);
				break;
			case 't':
				param.kernel_type = atoi(argv[i]);
				break;
			case 'd':
				param.degree = atoi(argv[i]);
				break;
			case 'g':
				param.gamma = atof(argv[i]);
				break;
			case 'r':
				param.coef0 = atof(argv[i]);
				break;
			case 'n':
				param.nu = atof(argv[i]);
				break;
			case 'm':
				param.cache_size = atof(argv[i]);
				break;
			case 'c':
				param.C = atof(argv[i]);
				break;
			case 'e':
				param.eps = atof(argv[i]);
				break;
			case 'p':
				param.p = atof(argv[i]);
				break;
			case 'h':
				param.shrinking = atoi(argv[i]);
				break;
			case 'b':
				param.probability = atoi(argv[i]);
				break;
			case 'q':
				print_func = &print_null;
				i--;
				break;
			case 'v':
				cross_validation = 1;
				nr_fold = atoi(argv[i]);
				if(nr_fold < 2)
				{
					fprintf(stderr,"n-fold cross validation: n must >= 2\n");
					exit_with_help();
				}
				break;
			case 'w':
				++param.nr_weight;
				param.weight_label = (int *)realloc(param.weight_label,sizeof(int)*param.nr_weight);
				param.weight = (double *)realloc(param.weight,sizeof(double)*param.nr_weight);
				param.weight_label[param.nr_weight-1] = atoi(&argv[i-1][2]);
				param.weight[param.nr_weight-1] = atof(argv[i]);
				break;
			default:
				fprintf(stderr,"Unknown option: -%c\n", argv[i-1][1]);
				exit_with_help();
		}
	}*/

	svm_set_print_string_function(print_func);

	// determine filenames

	/*if(i>=argc)
		exit_with_help();*/

	strncpy(input_file_name, "heart_scale",1024);
	strncpy(model_file_name, "heart_scale_result",1024);

	/*if(i<argc-1)
		strncpy(model_file_name,argv[i+1],1024);
	else
	{
		char *p = strrchr(argv[i],'/');
		if(p==NULL)
			p = argv[i];
		else
			++p;
		sprintf(model_file_name,"%s.model",p);
	}*/
}

// read in a problem (in svmlight format)

void read_problem(const char *filename)
{
	int max_index, inst_max_index, i;
	size_t elements, j;
	FILE *fp = fopen(filename,"r");
	char *endptr;
	char *idx, *val, *label;

	if(fp == NULL)
	{
		//fprintf(stderr,"can't open input file %s\n",filename);
		ocall(1,"Cant open input file\n",22,NULL,0,0);
		exit(1);
	}

	prob.l = 0;
	elements = 0;

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	while(readline(fp)!=NULL)
	{
		char *p = strtok(line," \t"); // label

		// features
		while(1)
		{
			p = strtok(NULL," \t");
			if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++prob.l;
	}
	rewind(fp);

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,elements);

	max_index = 0;
	j=0;
	for(i=0;i<prob.l;i++)
	{
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp);
		prob.x[i] = &x_space[j];
    int length = strlen(line);
    //ocall(2,&length,sizeof(int),NULL,0,0);
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
    {
      ocall(1,"Empty line\n",12,NULL,0,0);
      //ocall(1,line,strlen(line),NULL,0,0);
      //ocall(2,&i,sizeof(i),NULL,0,0);
			exit_input_error(i+1);
    }

		prob.y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
    {
      ocall(1,"Strtod error\n",14,NULL,0,0);
			exit_input_error(i+1);
    }

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			//errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || /*errno != 0 ||*/ *endptr != '\0' || x_space[j].index <= inst_max_index)
      {
        ocall(1,"Strtol error\n",14,NULL,0,0);
				exit_input_error(i+1);
      }
			else
				inst_max_index = x_space[j].index;

			//errno = 0;
			x_space[j].value = strtod(val,&endptr);
			if(endptr == val || /*errno != 0 ||*/ (*endptr != '\0' && !isspace(*endptr)))
      {
        ocall(1,"Strtod again\n",14,NULL,0,0);
				exit_input_error(i+1);
      }

			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if(param.gamma == 0 && max_index > 0)
		param.gamma = 1.0/max_index;

	if(param.kernel_type == PRECOMPUTED)
		for(i=0;i<prob.l;i++)
		{
			if (prob.x[i][0].index != 0)
			{
				//fprintf(stderr,"Wrong input format: first column must be 0:sample_serial_number\n");
				ocall(1,"Wrong Input Format\n",20,NULL,0,0);
				exit(1);
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				//fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
				ocall(1,"Wrong Input Format\n",20,NULL,0,0);
				exit(1);
			}
		}

	fclose(fp);
}
