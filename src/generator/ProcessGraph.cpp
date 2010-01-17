/*
 * ProcessGraph.cpp
 *
 *  Created on: Dec 28, 2009
 *      Author: Silviu
 */
#include <iostream>
#include "ProcessGraph.h"
#include "StreamDirectionAnalyzer.h"

DataStream::DataStream(clang::VarDecl *var, clang::Expr *init):varp(var),call(init){
	//this->name=name;
	headsiz=0;
}

DataStream::~DataStream(){

}
clang::DeclRefExpr *getProcessFunction(clang::Expr *e){
	if (dyn_cast<clang::DeclRefExpr>(e))
			return dyn_cast<clang::DeclRefExpr>(e);
		if (dyn_cast<clang::ImplicitCastExpr>(e)){
			//return obtainDeclR(dyn_cast<clang::ImplicitCastExpr>(e));
			clang::ImplicitCastExpr *ice = dyn_cast<clang::ImplicitCastExpr>(e);
			return getProcessFunction(ice->getSubExpr());
		}
		if (dyn_cast<clang::CStyleCastExpr>(e)){
			clang::CStyleCastExpr *cse = dyn_cast<clang::CStyleCastExpr>(e);
			return getProcessFunction(cse->getSubExpr());
		}
		return 0;
}

void DataStream::addProcess(ProcessGraphNode *node){
	heads[headsiz++]=node;
}

ProcessGraphNode *DataStream::getStartProcess(){
	if (headsiz==0)
		return NULL;
	return heads[0];
}

ProcessGraphNode *DataStream::getEndProcess(){
	if (headsiz<2)
		return NULL;
	return heads[1];
}

void DataStream::reverse(){
	ProcessGraphNode *tmp = heads[0];
	heads[0]=heads[1];
	heads[1] = tmp;
}

const char *DataStream::getName(){
	return name.c_str();
}

ProcessGraph::ProcessGraph(clang::SourceManager *sm):SM(sm){
	numNodes=0;
	numVertices=0;
}

ProcessGraph::~ProcessGraph(){

}

ProcessGraphNode *ProcessGraph::lookupGraphNode(int number){
	return nodes[number];
}

int ProcessGraph::addGraphNode(ProcessGraphNode *pgn){
	nodes[numNodes]=pgn;
	inverse[pgn->getVarDecl()]=numNodes;
	//std::cout << "adaugat graph node\n";

	clang::CallExpr *e=dyn_cast<clang::CallExpr>(pgn->getExpr());
	clang::DeclRefExpr *fun = getProcessFunction(e->getArg(1));

	clang::FunctionDecl *funD = dyn_cast<clang::FunctionDecl>(fun->getDecl());
	//if (funD!=NULL)
	//	std::cout << "gasit decl\n";
	pgn->setName(funD->getNameAsString());
	std::cout << "adaugat "<<pgn->getName() << std::endl;
	pgn->setNumber(numNodes);
	int numP = funD->getNumParams();
	for (int i=3; i<3+numP; i++){
		clang::ParmVarDecl *pvd = funD->getParamDecl(i-3);
		if (pvd->getType().getAsString().compare("co_stream")==0){
			clang::DeclRefExpr *str_ref = getProcessFunction(e->getArg(i));

			if (!dyn_cast<clang::VarDecl>(str_ref->getDecl())){
				std::cerr << "error: not referencing a variabile in call to co_process_create\n";
				exit(-1);
			}
			int str_id = inverse[dyn_cast<clang::VarDecl>(str_ref->getDecl())];
			DataStream *ds = vertices[str_id];
			/*
			if (ds==NULL)
				std::cout<<"ds null";
			*/
			ds->addProcess(pgn);
			pgn->addStream(ds);
			std::cout << "added stream " << ds->getVarDecl()->getNameAsString() << "\n";
		}
	}

	numNodes++;
	return numNodes-1;
}
void ProcessGraph::establishDirection(){
	for (int i=0; i<this->numNodes; i++){
		ProcessGraphNode *pgn = this->nodes[i];
		/*
		 *	establish direction of streams
		 */
		std::cout<<"----------------------------------------------"<<std::endl;
		clang::CallExpr *e=dyn_cast<clang::CallExpr>(pgn->getExpr());
		clang::DeclRefExpr *fun = getProcessFunction(e->getArg(1));
		clang::FunctionDecl *funD = dyn_cast<clang::FunctionDecl>(fun->getDecl());
		StreamDirectionAnalyzer sda(funD,SM);
		sda.Visit(funD->getBody());
		std::vector<DataStream *> *prstreams = pgn->getStreams();
		std::vector<int> *dirstreams = sda.getDirections();
		std::cout << pgn->getName() <<" ";
		for (int j=0; j<prstreams->size(); j++){
			std::cout << (*dirstreams)[j] << " ";

			if ((*dirstreams)[j]==read_stream && (*prstreams)[j]->getStartProcess()==pgn){
				std::cout<<"\nreversed read "<<(*prstreams)[j]->getStartProcess()->getName()<<","<<
										(*prstreams)[j]->getEndProcess()->getName()<<" "<<
												(*prstreams)[j]->getVarDecl()->getNameAsString()<<"\n";
				(*prstreams)[j]->reverse();
			}else
			if ((*dirstreams)[j]==write_stream && (*prstreams)[j]->getEndProcess()==pgn)
			{
				std::cout<<"\nreversed write "<<(*prstreams)[j]->getStartProcess()->getName()<<","<<
														(*prstreams)[j]->getEndProcess()->getName()<<" "<<
																(*prstreams)[j]->getVarDecl()->getNameAsString()<<"\n";
				(*prstreams)[j]->reverse();

			}

		}
		std::cout<<"\n";
		std::cout<<"----------------------------------------------"<<std::endl;
	}
	std::cout << "Finished orientation of process graph\n";
}

int ProcessGraph::addStream(DataStream *ds){
	vertices[numVertices]=ds;
	inverse[ds->getVarDecl()]=numVertices;
	numVertices++;
	std::cout << "adaugat stream\n";

	return numVertices-1;
}

DataStream *ProcessGraph::lookupDataStream(int number){
	return vertices[number];
}

void ProcessGraph::attach(int graphNodeNumber, int dataStreamNumber){
	ProcessGraphNode *pgn = lookupGraphNode(graphNodeNumber);
	DataStream *ds = lookupDataStream(dataStreamNumber);
	if (ds && pgn)
		pgn->addStream(ds);
}

ProcessGraphNode::ProcessGraphNode(clang::VarDecl *var, clang::Expr *init):varp(var),call(init){
	this->hardwareImplementable = false;
}

ProcessGraphNode::~ProcessGraphNode(){

}

std::vector<DataStream *> *ProcessGraphNode::getStreams(){
	return &streams;
}

void ProcessGraphNode::addStream(DataStream *ds){
	streams.push_back(ds);
}


