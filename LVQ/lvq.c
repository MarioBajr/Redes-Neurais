/******
 * The program is used for training a neural network according to the lvq algorithm
 ******/
#include "lvq.h"

#pragma mark - Struct Definicitons

enum BOOL 
{
	TRUE =	1,
	FALSE = 0,
};

struct Node
{	double *weight;	/* position of the node in input space */
	int class;		/* class associated with the node */
};

struct Pattern
{	double *input;	/* input vector */
	int class;		/* class of the input vector */
	int class_obtained;
};

struct Network
{
	/* The structure */
	int indim;			/* # input dimensions */
	int num_nodes;		/* # nodes in the network */
	int num_classes;	/* # classes of inputs. Start from 0 */
	struct Node *nodes;	/* Linear storage for the nodes */
	
	/* The training parameters */
	double eta_start, eta_end;
	double convergence;
	int num_iter;
	int learning;
	
	/* The inputs */
	int num_inputs;			/* # inputs */
	struct Pattern *inputs; /* Storage for input vectors */
};

#pragma mark - Math Methods

/*
 Get random number between low and high values
 */
double get_rand(low, high)
double low, high;
{
	double x = rand() % MAXRAND / (double)MAXRAND;
	return( x*(high-low) + low );
}

/* 
 Get euclidian distance 
 */
double get_euclidean( x, y, n)
double *x, *y;
int n;
{
	double dist = 0;
	int i;
	
	for( i=0; i<n; i++ )
	    dist += (x[i]-y[i]) * (x[i]-y[i]);
	
	return(dist);
}

/*
 Get the closest node and return the distance and the index 
 */
void closest(p, in, n, mindist, best)
struct Network *p;
double *in;
int n;
double *mindist;
int *best;
{	
	double distance, mindistance;
	int index, i;
	
	mindistance = INT_MAX;
	
	for( i=0; i<p->num_nodes; i++)
	{
		distance = get_euclidean(p->nodes[i].weight, in, n);
	    if( distance < mindistance )
	    {
			mindistance = distance;
			index = i;
	    }
	}
	
	*mindist = mindistance;
	*best = index;
}

/*
 Update the best, if the class match, aproximate the node with the input, else increse the distance 
 */
void update_best(best, in, n, eta)
struct Node best;
struct Pattern in;
int n;
double eta;
{	
	int sign;
	int i;
	
	sign = ( best.class == in.class ) ? 1 : -1;
	
	for( i=0; i<n; i++)
	    best.weight[i] += sign * eta * (in.input[i] - best.weight[i]);
}

#pragma mark - IO Methods

/*
 Open file and put the content inside fp variable
 */
void open_file( filename, fp, type)
char *filename;
FILE **fp;
int type;
{
	if( filename )
	{
		if( type == READFILE )
			*fp = fopen(filename, "rt");
		else
			*fp = fopen(filename, "wt");
		
	    if( *fp == NULL )
	    {
			printf("Unable to open file %s for %s\n",filename, (type == READFILE) ? "reading" : "writing");
			_exit(0);
	    }
	}
}

/*
 Classify input samples
 */
void get_outputs(p)
struct Network *p;
{
	int sample, i;
	double dist, mindist;
	int winner;
	double *in;
	
	for( sample=0; sample<p->num_inputs; sample++)
	{
		in = p->inputs[sample].input;
		winner = 0;
		mindist = INT_MAX;
	    for( i=0; i<p->num_nodes; i++)
	    {	
			dist = get_euclidean(p->nodes[i].weight, in, p->indim);
			if( dist < mindist )
			{  
				mindist = dist;
				winner = i;
			}
	    }
	    p->inputs[sample].class_obtained = p->nodes[winner].class;
	}
}

/* 
 Reads the input files and initializes the structure networs. Places all values
 of interest in the structure. If one of the arrays in the structure cannot be
 allocated, it deallocates the whole structure and returns NULL
 */
struct Network *read_from_file(task, in, data)
int task;
FILE *in, *data;
{	
	struct Network *p = NULL;
	int i,j;
	
	if( (p = get_network()) == NULL)
		return( NULL );
	
	if( task == TASK_LEARNING )
	    p->learning = TRUE;
	else
	    p->learning = FALSE;
	
	if (p->learning)
	{
		fscanf(data, "%d %d %d\n", &p->num_inputs, &p->indim, &p->num_classes);
		fscanf(in, "%d\n", &p->num_nodes);
	}
	else
	{		
		fscanf(in, "%d\n", &p->indim);
		fscanf(in, "%d %d\n", &p->num_nodes, &p->num_classes);
	}
	
	if( p->num_classes > p->num_nodes )
	{
		printf("Error: More classes than nodes\n");
	    deallocate_network(p);
	    return(NULL);
	}
	
	p->nodes = (struct Node *)malloc(p->num_nodes * sizeof(struct Node));
	if( p->nodes == NULL )
	{
		deallocate_network(p);
		return(NULL);
	}
	for( i=0; i<p->num_nodes; i++)
		p->nodes[i].weight = NULL;
	for( i=0; i<p->num_nodes; i++)
	{   
		p->nodes[i].weight = (double *)malloc(p->indim * sizeof(double));
	    if( p->nodes[i].weight == NULL )
	    {	
			deallocate_network(p);
			return(NULL);
	    }
	}
	
	if( p->learning )
	{
	    fscanf(in, "%lf %lf\n", &p->eta_start, &p->eta_end);
		fscanf(in, "%d %lf\n", &p->num_iter, &p->convergence);
	}
	else
	{
	    for( i=0; i<p->num_nodes; i++)
	    {	
			for( j=0; j<p->indim; j++)
				fscanf(in, "%lf ", p->nodes[i].weight + j);
			fscanf(in, "%d\n", &p->nodes[i].class);
	    }
	}
	
	p->inputs = (struct Pattern *)malloc(p->num_inputs*sizeof(struct Pattern));
	if( p->inputs == NULL )
	{	
		deallocate_network(p);
		return(NULL);
	}
	for( i=0; i<p->num_inputs; i++)
	    p->inputs[i].input = NULL;
	for( i=0; i<p->num_inputs; i++ )
	{   
		p->inputs[i].input = (double *)malloc(p->indim * sizeof(double));
	    if( p->inputs[i].input == NULL )
	    {	
			deallocate_network(p);
			return(NULL);
	    }
	}
	
	for( i=0; i<p->num_inputs; i++)
	{
		for( j=0; j<p->indim; j++)
			fscanf(data, "%lf ", p->inputs[i].input + j);
		fscanf(data, "%d\n", &p->inputs[i].class);
	}
	
	return(p);
}

/*
 Write the weights, with the structural parameters of the LVQ network, to the file
 out
 */
void write_output_weights(p, out)
struct Network *p;
FILE *out;
{
	int i,j;
	
	/* Output the number of dimensions */
	fprintf(out, "%d\n", p->indim);
	
	/* Output the # of nodes and # of classes */
	fprintf(out, "%d %d\n",p->num_nodes, p->num_classes);
	
	/* Now for each node, print the weights and the class number */
	for( i=0; i<p->num_nodes; i++)
	{
		for(j=0; j<p->indim; j++)
			fprintf(out, "%lf ",p->nodes[i].weight[j]);
	    fprintf(out, "%d \n",p->nodes[i].class);
	}
	
	fclose(out);
}

/* Write the data, with the derived classes, to the file out */
void write_output_data(p, out)
struct Network *p;
FILE *out;
{
	int i,j;
	int tp = 0;
	
	for( i=0; i<p->num_inputs; i++)
	{	
		for(j=0; j<p->indim; j++)
			fprintf(out, "%lf ",p->inputs[i].input[j]);
	    fprintf(out, "%d %d\n",p->inputs[i].class, p->inputs[i].class_obtained);
		tp += (p->inputs[i].class == p->inputs[i].class_obtained);
	}
	
	double correct_rate = tp / (double)p->num_inputs;
	printf("Correct Rate: (%d/%d) %lf%%\n", tp, p->num_inputs, correct_rate*100);
	fprintf(out, "%lf", correct_rate);
	
	fclose(out);
}

#pragma mark - LVQ Logic

void init_network(p)
struct Network *p;
{	
	int i,j;
	int *classes;
	int *class_nums;
	double x, min, max;
	int number;
	
	//Initilize Random
	srand ( time(NULL) );
	
	/* Allocate temporary storage for the classes */
	classes = (int *)malloc(p->num_classes*sizeof(int));
	class_nums = (int*)malloc(p->num_classes*sizeof(int));
	
	/* Count samples per class */
	for( i=0; i<p->num_classes; i++)
	{   
		classes[i] = 0;
	    class_nums[i] = i;
	}
	for( i=0; i<p->num_inputs; i++)
	    classes[p->inputs[i].class]++;
	
	/* Sort classes by ascending number of patterns */
	for( i=0; i<p->num_classes; i++)
	{
	    for( j=i+1; j<p->num_classes; j++)
		{
			if( classes[i] > classes[j] )
			{  
				number = classes[i];
				classes[i] = classes[j];
				classes[j] = number;
				number = class_nums[i];
				class_nums[i] = class_nums[j];
				class_nums[j] = number;
			}
		}
	}
	
	j = 0;
	for( i=0; i<p->num_classes-1; i++)
	{   
		x = p->num_nodes * classes[i] / (double)p->num_inputs;
	    number = (x == (int)x) ? (int)x : (int)(x+1) ;
	    while( number > 0 )
	    {	
			p->nodes[j].class = class_nums[i];
			j++;
			number--;
	    }
	}
	/* assign left-over nodes to the last class (class_nums[p->num_classes-1]) 
	 */ 
	for( i=j; i<p->num_nodes; i++)
	    p->nodes[i].class = class_nums[p->num_classes-1];
	
	/* For each input dimension, find the min. and max. value, and assign all
	 weights from that input node between those two values.
	 */
	for( i=0; i<p->indim; i++)
	{   
		min = max = p->inputs[0].input[i];
	    for( j=0; j<p->num_inputs; j++)
			if( p->inputs[j].input[i] > max )
				max = p->inputs[j].input[i];
			else if( p->inputs[j].input[i] < min )
				min = p->inputs[j].input[i];
		
	    /* Otherwise, the get_rand would divide by zero */
	    if( min == max )
			max = min * 1.1;
		
	    for( j=0; j<p->num_nodes; j++)
			p->nodes[j].weight[i] = get_rand(min, max);
	}
}

void train_network(p)
struct Network *p;
{
	double eta_start, step, eta;
	int t, i, best;
	double distance, error; 
	
	eta_start = p->eta_start;
	step = p->eta_start * (p->num_iter - 1)/(p->eta_start - p->eta_end);
	t = 0;
	error = p->convergence + 1;
	while( t < p->num_iter && error > p->convergence )
	{
		eta = eta_start * (1 - t/step);
	    error = 0;
	    for( i=0; i<p->num_inputs; i++)
	    {
			closest(p, p->inputs[i].input, p->indim, &distance, &best);
			update_best(p->nodes[best], p->inputs[i], p->indim, eta);
			error += distance;
	    }
	    t++;
	    error /= p->num_inputs;
	    printf("Iteration: (%d/%d) Error: %lf\n", t, p->num_iter, error);
	}
}

/* 
 Allocates the structure network, initializes all pointers to NULL, and all integers to 0
 */
struct Network *get_network()
{
	struct Network *p;
	
	p = (struct Network *)malloc(sizeof(struct Network));
	if( p == NULL )
		return(NULL);
	
	p->indim = p->num_nodes = p->num_classes = p->num_iter = 0;
	p->eta_start = p->eta_end = 0.0;
	p->nodes = NULL;
	p->inputs = NULL;
	return p;
}


/* 
 Deallocates all already allocated arrays and structures in the network
 */
void deallocate_network(p)    
struct Network *p;
{
	int i;
	
	if( p->nodes )
	{
		for( i=0; i<p->num_nodes; i++)
		if( p->nodes[i].weight )
		    free( p->nodes[i].weight);
	    free(p->nodes);
	}
	
	if( p->inputs )    
	{
		for( i=0; i<p->num_inputs; i++)
			if( p->inputs[i].input )
				free( p->inputs[i].input );
	    free(p->inputs);
	}
	
	free(p);
}

/* 
 Execute Program
 */
void lvq(task, inputfile, datafile, outfile)
int task;
char *inputfile, *datafile, *outfile;
{
	FILE *in, *data, *out;
	struct Network *p;
	long t1, t2;
	
	t1 = clock();
	in = out = data = NULL;
	open_file( inputfile, &in, READFILE);
	open_file( datafile, &data, READFILE);
	open_file( outfile, &out, WRITEFILE);
	
	p = read_from_file(task, in, data);
	
	if( p->learning )
	{
		init_network(p);
		train_network(p);
		write_output_weights(p, out);
	}
	else
	{
		get_outputs(p);
		write_output_data(p, out);
	}
	deallocate_network(p);
	
	t2 = clock();
	printf("Elapsed time %ld milliseconds\n",(t2-t1)/1000);
}

