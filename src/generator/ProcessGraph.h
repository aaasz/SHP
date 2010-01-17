/*
 * ProcessGraph.h
 *
 *  Created on: Dec 27, 2009
 *      Author: Silviu
 */
#ifndef PROCESSGRAPH_H_
#define PROCESSGRAPH_H_
#include <string>
#include <vector>
#include <map>
#include <clang/AST/StmtVisitor.h>

class DataStream;

typedef struct {
	unsigned int hadrwareExecutionTime; //in ms
	unsigned int softwareExecutionTime; //in ms
	unsigned int LUTS;
	unsigned int SLICES;
	unsigned int communicationTime;
}VertexData;

typedef struct{
	unsigned int dataTransferTime; //in ms
}EdgeData;

class ProcessGraphNode{
public:
	ProcessGraphNode(clang::VarDecl *var, clang::Expr *init);
	~ProcessGraphNode();
	clang::VarDecl *getVarDecl(){return varp;}
	clang::Expr *getExpr(){return call;}

	std::vector<DataStream *> *getStreams();
	void addStream(DataStream *ds);
	VertexData getVertexData(){return data;}
	void setVertexData(VertexData data){this->data = data;}
	std::string getName(){return this->name;}
	void setName(std::string name){this->name = name;}
	bool isHardwareImplementable(){return hardwareImplementable;}
	void setHardwareImplementable(bool a){this->hardwareImplementable = a;}
	void setNumber(int num){this->number = num;}
	int getNumber(){return number;}

private:
	int number;
	std::string name;
	VertexData data;
	std::vector<DataStream *> streams;
	clang::VarDecl *varp;
	clang::Expr *call;
	bool hardwareImplementable;

};


class DataStream{
public:
	DataStream(clang::VarDecl *var, clang::Expr *init);
	~DataStream();
	void addProcess(ProcessGraphNode *node);
	ProcessGraphNode *getStartProcess();
	ProcessGraphNode *getEndProcess();
	clang::Expr *getExpr(){return call;}
	clang::VarDecl *getVarDecl(){return varp;}
	void reverse();
	const char *getName();
	EdgeData getEdgeData(){
		return data;
	}
	void setEdgeData(EdgeData data){
		this->data = data;
	}
private:
	EdgeData data;
	std::string name;
	ProcessGraphNode *heads[2];
	int headsiz;
	clang::VarDecl *varp;
	clang::Expr *call;

};

class ProcessGraph{
public:
	ProcessGraph(clang::SourceManager *sm);
	~ProcessGraph();
	ProcessGraphNode *lookupGraphNode(int number);
	int addGraphNode(ProcessGraphNode *pgn); //returns the assigned number of the graph node
	int addStream(DataStream *ds); //same as addGraphNode
	void attach(int graphNodeNumber, int dataStreamNumber);
	void establishDirection();
	DataStream *lookupDataStream(int number);
	int getNumVertices(){return this->numNodes;}
	int getNumEdges(){return this->numVertices;}
	ProcessGraphNode *lookupNodeByVar(clang::VarDecl *decl){return nodes[inverse[decl]];}
	void setMaxLUTS(unsigned int value) {this->maxLUTS = value;}
	unsigned int getMaxLUTS() {return maxLUTS;}
	void setMaxSLICES(unsigned int value) {this->maxSLICES = value;}
	unsigned int getMaxSLICES() {return maxSLICES;}
	void setProcessorLUTS(unsigned int value) {this->processorLUTS = value;}
	unsigned int getProcessorLUTS() {return processorLUTS;}
	void setProcessorSLICES(unsigned int value) {this->processorSLICES = value;}
	unsigned int getProcessorSLICES() {return processorSLICES;}
private:
	std::map<int,ProcessGraphNode *> nodes;
	std::map<int,DataStream *> vertices;
	std::map<clang::VarDecl *,int> inverse;
	int numNodes;
	int numVertices;
	unsigned int maxLUTS;
	unsigned int maxSLICES;
	unsigned int processorLUTS;
	unsigned int processorSLICES;
	clang::SourceManager *SM;
};


#endif /* PROCESSGRAPH_H_ */
