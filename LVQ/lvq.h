//
//  lvq.h
//  LVQ
//
//  Created by Mário Barbosa on 6/27/12.
//  Copyright (c) 2012 Partec. All rights reserved.
//

#ifndef LVQ_lvq_h
#define LVQ_lvq_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#define TASK_LEARNING	1
#define TASK_OUTPUT		2
#define READFILE		0
#define WRITEFILE		1
#define MAXRAND			(1<<16)

void lvq(), open_file(), deallocate_network();
void init_network(), train_network();
void closest(), update_best(), get_outputs();
double get_rand();
struct Network *read_input(), *get_network(), *read_from_file();
void write_output_weights(), write_output_data();

#endif
