/*
 * GraphDataAppender.h
 *
 *  Created on: Jan 10, 2010
 *      Author: Silviu
 */

#ifndef GRAPHDATAAPPENDER_H_
#define GRAPHDATAAPPENDER_H_

#include "ProcessGraph.h"

class GraphDataAppender {
public:
	GraphDataAppender(ProcessGraph *pg,std::string test_path);
	virtual ~GraphDataAppender();
};

#endif /* GRAPHDATAAPPENDER_H_ */
