//
//  LVQ1Algorithm.c
//  LVQ
//
//  Created by Mário Barbosa on 6/25/12.
//  Copyright (c) 2012 Partec. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>

void polly();

enum BOOLEAN  /* Declares an enumeration data type called BOOLEAN */
{
    false,     /* false = 0, true = 1 */
    true 
};

struct LearningRateKernel {
	float learningRate;
	int totalIterations;
};

struct CommonModel {
	
};

struct LVQ1Algorithm {
	struct LearningRateKernel learningKernel;
	struct CommonModel commonModel;
};

void polly(a)
int a;
{
	printf("Ola Popo! %d\n", a);
	
	struct LearningRateKernel *list;
	list = (struct LearningRateKernel *)malloc(a * sizeof(struct LearningRateKernel));
	
	for (int i=0; i<a; i++)
	{
//		struct LearningRateKernel *kernel; 
//		kernel = (struct LearningRateKernel *)malloc(sizeof(struct LearningRateKernel));
//		kernel->learningRate = 1.1+i;
//		
//		list[i] = *kernel;
		
		struct LearningRateKernel kernel;
		kernel.learningRate = 1.1+i;
		
		list[i] = kernel;
	}
	
	for (int i=0; i<a; i++)
	{
//		struct LearningRateKernel *kernel = &list[i];
//		printf("Lista: %f", kernel->learningRate);
		
		struct LearningRateKernel kernel = list[i];
		printf("Lista: %f", kernel.learningRate);
	}
}