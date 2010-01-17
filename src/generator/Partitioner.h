/*
 * Partitioner.h
 *
 *  Created on: Jan 9, 2010
 *      Author: Silviu
 */

#ifndef PARTITIONER_H_
#define PARTITIONER_H_


#include "ProcessGraph.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <gaul.h>

#define HARDWARE 0
#define SOFTWARE 1

boolean partitioner_score(population *pop, entity *entity);
boolean partitioner_generation_callback(int generation, population *pop);
boolean partitioner_seed(population *pop, entity *adam);
void partitioner_mutate_bool_singlepoint_drift( population *pop,
	                                          entity *father, entity *son );
boolean partitioner_score_critical(population *pop, entity *entity);
#ifdef __cplusplus
 }
#endif

typedef struct
{
  ProcessGraph *graph;
  unsigned int maxLUTS;
  unsigned int maxSLICES;
} partitioner_data_t;

class Partitioner {
public:
	Partitioner(ProcessGraph *pg, int populationSize, int maxIter,bool criticalPath,
			std::string nameIn, std::string nameOut);
	virtual ~Partitioner();
	bool *getBestSolution(){return bestSolution;}
private:
	bool *bestSolution;
};





#endif /* PARTITIONER_H_ */
