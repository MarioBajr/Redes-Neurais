//
//  main.c
//  LVQ
//
//  Created by Mário Barbosa on 6/25/12.
//  Copyright (c) 2012 Partec. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "lvq.h"

int main(int argc, const char * argv[])
{
	char current_path[100] = "/Users/Mario/Documents/workspaces/mestrado workspace/workspace/LVQ/LVQ/data/";
//	char current_path[100] = "/Users/Mario/Documents/workspaces/mestrado workspace/workspace/LVQ/LVQ/data/testing/";
	
	char net[300];
	strcpy(net, current_path);
	strcat(net, "network.dat");
	
	char train[300];
	strcpy(train, current_path);
	strcat(train, "train.dat");
	
	char weights[300];
	strcpy(weights, current_path);
	strcat(weights, "weights.dat");
	
	char test[300];
	strcpy(test, current_path);
	strcat(test, "test.dat");
	
	char output[300];
	strcpy(output, current_path);
	strcat(output, "output.dat");
	
//	lvq(TASK_LEARNING, net, train, weights);//Train
	lvq(TASK_OUTPUT, weights, test, output);//Test
	
	return 0;
}