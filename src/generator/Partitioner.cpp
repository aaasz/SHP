/*
 * Partitioner.cpp
 *
 *  Created on: Jan 9, 2010
 *      Author: Silviu
 */
#include <iostream>
#include <map>
#include "Partitioner.h"

ProcessGraphNode *startNode;
ProcessGraphNode *endNode;
std::vector<ProcessGraphNode *> sortedNodes;
std::vector<unsigned int> val;
std::vector<unsigned int> pred;

/*
L ← Empty list that will contain the sorted nodes
S ← Set of all nodes

function visit(node n)
    if n has not been visited yet then
        mark n as visited
        for each node m with an edge from n to m do
            visit(m)
        add n to L

for each node n in S do
    visit(n)
 */

void visit(ProcessGraphNode *n,std::map<ProcessGraphNode *,bool> *visited){
	if (!(*visited)[n]){
		(*visited)[n]=true;
		for (int i=0; i<n->getStreams()->size(); i++){
			DataStream *ds = (*(n->getStreams()))[i];
			if (ds->getStartProcess()==n && ds->getStartProcess()!=endNode
					&& ds->getEndProcess()!=startNode){
				visit(ds->getEndProcess(),visited);
			}
		}
		sortedNodes.push_back(n);
	}
}



Partitioner::Partitioner(ProcessGraph *pg, int populationSize, int maxIter, bool criticalPath,
		std::string nameIn, std::string nameOut) {
	// TODO Auto-generated constructor stub
	population            *pop;
	partitioner_data_t data = {pg, pg->getMaxLUTS(), pg->getMaxSLICES()};
	random_seed(23091975);

	//data.pg = pg;

	//tmp = new bool[pg->getNumVertices()];

	if (criticalPath) {
		std::map<ProcessGraphNode *,bool> visited;
		for (int i = 0; i < pg->getNumVertices(); i++) {
			if (pg->lookupGraphNode(i)->getName() == nameIn) {
				startNode = pg->lookupGraphNode(i);
			}
			if (pg->lookupGraphNode(i)->getName() == nameOut) {
				endNode = pg->lookupGraphNode(i);
			}
			visited[pg->lookupGraphNode(i)] = false;
			val.push_back(0);
			pred.push_back(0);
		}
		//do topological sort
		for (int i = 0; i < pg->getNumVertices(); i++) {
			visit(pg->lookupGraphNode(i),&visited);
		}
	}

	pop = ga_genesis_double(
	       populationSize,                             /* const int              population_size */
	       1,                               /* const int num_chromo */
	       pg->getNumVertices(),            /* const int len_chromo */
	       NULL,     /* GAgeneration_hook      generation_hook */
	       NULL,                            /* GAiteration_hook       iteration_hook */
	       NULL,                            /* GAdata_destructor      data_destructor */
	       NULL,                            /* GAdata_ref_incrementor data_ref_incrementor */
	       criticalPath ? partitioner_score_critical : partitioner_score,                   /* GAevaluate             evaluate */
	       partitioner_seed,                    /* GAseed                 seed */
	       NULL,                            /* GAadapt                adapt */
	       ga_select_one_linearrank,        /* GAselect_one           select_one */
	       ga_select_two_linearrank,        /* GAselect_two           select_two */
	       partitioner_mutate_bool_singlepoint_drift, /* GAmutate               mutate */
	       ga_crossover_boolean_mixing,        /* GAcrossover            crossover */
	       NULL,
	       NULL                             /* vpointer     User data */
	);
	pop->data = &data;
	ga_population_set_parameters(
	       pop,                             /* population      *pop */
	       GA_SCHEME_DARWIN,                /* const ga_scheme_type     scheme */
	       GA_ELITISM_PARENTS_DIE,          /* const ga_elitism_type   elitism */
	       0.8,                             /* double  crossover */
	       0.8,                             /* double  mutation */
	       0.0                              /* double  migration */
	);

	int rez_pos = ga_evolution(
	       pop,
	       maxIter
	);
	entity *ent = ga_get_entity_from_rank(pop,0);

	this->bestSolution = new bool[pg->getNumVertices()];
	for (int i=0; i<pg->getNumVertices(); i++){
		this->bestSolution[i] = ((bool *)(ent->chromosome[0]))[i];
	}

	ga_extinction(pop);



//	delete []tmp;
}

Partitioner::~Partitioner() {
	// TODO Auto-generated destructor stub
	delete []this->bestSolution;
}


boolean partitioner_score(population *pop, entity *entity){
	partitioner_data_t        *data = (partitioner_data_t *)pop->data;
	ProcessGraph *pg = data->graph;

/*
 * Preliminary eval function
 */
	bool *params = (bool *)(entity->chromosome[0]);
	unsigned int totalLUTS = 0;
	unsigned int totalSLICES = 0;
	bool partInSoft = FALSE;

	unsigned int totalExecutionTime = 0;
	for (int i=0; i<pg->getNumVertices(); i++){
		VertexData vd = pg->lookupGraphNode(i)->getVertexData();
		if (params[i] == HARDWARE){
			totalLUTS += vd.LUTS;
			totalSLICES += vd.SLICES;
			totalExecutionTime += vd.hadrwareExecutionTime;
			if (vd.hadrwareExecutionTime == INT_MAX){
				return FALSE;
			}
		}else{
			partInSoft = TRUE;
			totalExecutionTime += vd.softwareExecutionTime;
		}
	}

	if (partInSoft){
		totalLUTS += pg->getProcessorLUTS();
		totalSLICES += pg->getProcessorSLICES();
	}
	if (totalLUTS >= data->maxLUTS || totalSLICES >= data->maxSLICES){
		return FALSE;
	}

	entity->fitness = 100000. / ((float)totalExecutionTime);
	return TRUE;

}

boolean partitioner_seed(population *pop, entity *adam){

	if (!pop)
		die("Null pointer to population structure passed.");
	if (!adam)
		die("Null pointer to entity structure passed.");
	partitioner_data_t        *data = (partitioner_data_t *)pop->data;
	ProcessGraph *pg = data->graph;

	/* Seeding. */
	for (int i=0; i<pg->getNumVertices(); i++){
		((bool *)(adam->chromosome[0]))[i] = random_boolean();
	}


	return TRUE;

}
void partitioner_mutate_bool_singlepoint_drift( population *pop,
                                          entity *father, entity *son ){
	int chromo = (int) random_int(pop->num_chromosomes);
	int point = (int) random_int(pop->len_chromosomes);

	for (int i = 0; i < pop->num_chromosomes; i++) {
		memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes * sizeof(bool));
		if (i != chromo) {
			ga_copy_data(pop, son, father, i);
		} else {
			ga_copy_data(pop, son, NULL, i);
		}
	}
	((bool *)son->chromosome[chromo])[point] = ! ((bool *)son->chromosome[chromo])[point];
}

/*
 algorithm dag-longest-path is
    input:
         Directed acyclic graph G
    output:
         Length of the longest path

    length_to = array with |V(G)| elements of type int with default value 0

    for each vertex v in topOrder(G) do
        for each edge (v, w) in E(G) do
            if length_to[w] <= length_to[v] + weight(G,(v,w)) then
                length_to[w] = length_to[v] + weight(G, (v,w))

    return max(length_to[v] for v in V(G))

 */
int getMaximumLen(ProcessGraph *pg, bool *entity){

	for (int i=0; i<sortedNodes.size(); i++){
		val[i]=0;
	}
	for (int i=0; i<sortedNodes.size(); i++){
		ProcessGraphNode *v = sortedNodes[i];
		for (int j=0; j<v->getStreams()->size(); j++){
			DataStream *ds = (*(v->getStreams()))[j];
			if (ds->getStartProcess()==v && ds->getStartProcess()!=endNode
					&& ds->getStartProcess()!=endNode){
				unsigned int qval = entity[ds->getEndProcess()->getNumber()] == HARDWARE ?
						ds->getEndProcess()->getVertexData().hadrwareExecutionTime :
						ds->getEndProcess()->getVertexData().softwareExecutionTime;
				//if (val[ds->getEndProcess()]<=val[v]+ds->getEndProcess())
				if (val[ds->getEndProcess()->getNumber()]
				        <= val[v->getNumber()]+qval){
					val[ds->getEndProcess()->getNumber()] = val[v->getNumber()]+qval;
					pred[ds->getEndProcess()->getNumber()] = v->getNumber();
				}
			}
		}
	}
	unsigned int max =0;
	for (int i=0; i<sortedNodes.size(); i++){
		if (max<val[i])
			max = val[i];
	}

	unsigned int pos = endNode->getNumber();
	while (pos!=startNode->getNumber()){
		if (entity[pos] == SOFTWARE)
			max-= pg->lookupGraphNode(pos)->getVertexData().softwareExecutionTime;
		pos = pred[pos];
	}
	return max + (entity[startNode->getNumber()]==HARDWARE ?
						startNode->getVertexData().hadrwareExecutionTime :
						0);
}

boolean partitioner_score_critical(population *pop, entity *entity){
	partitioner_data_t *data = (partitioner_data_t *) pop->data;
	ProcessGraph *pg = data->graph;

	/*
	 * Preliminary eval function
	 */
	bool *params = (bool *) (entity->chromosome[0]);
	unsigned int totalLUTS = 0;
	unsigned int totalSLICES = 0;
	bool partInSoft = FALSE;

	unsigned int totalExecutionTime = 0;

	for (int i = 0; i < pg->getNumVertices(); i++) {
		VertexData vd = pg->lookupGraphNode(i)->getVertexData();
		if (params[i] == HARDWARE) {
			totalLUTS += vd.LUTS;
			totalSLICES += vd.SLICES;
//			totalExecutionTime += vd.hadrwareExecutionTime;
			if (vd.hadrwareExecutionTime == INT_MAX) {
				return FALSE;
			}
		} else {
			partInSoft = TRUE;
			totalExecutionTime += vd.softwareExecutionTime;
		}
	}

	if (partInSoft) {
		totalLUTS += pg->getProcessorLUTS();
		totalSLICES += pg->getProcessorSLICES();
	}
	if (totalLUTS >= data->maxLUTS || totalSLICES >= data->maxSLICES) {
		return FALSE;
	}
	totalExecutionTime += getMaximumLen(pg,params);
	entity->fitness = 100000. / ((float) totalExecutionTime);
	return TRUE;

}
