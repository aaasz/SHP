/*
 * ConfigDeclAnalyzer.h
 *
 *  Created on: Dec 28, 2009
 *      Author: Silviu
 */

#ifndef CONFIGDECLANALYZER_H_
#define CONFIGDECLANALYZER_H_

#include <map>
#include <clang/AST/StmtVisitor.h>

#include "Splitter.h"
#include "ProcessGraph.h"
class ConfigDeclAnalyzer : public clang::StmtVisitor<ConfigDeclAnalyzer> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::Decl *, clang::Decl *> *MAP;
	Splitter *SPLT;
	ProcessGraph *pg;

public:
	ConfigDeclAnalyzer(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::Decl *, clang::Decl *> *map, Splitter *splt,ProcessGraph *PG) :
		FD(fd), OS(os), MAP(map), SPLT(splt),pg(PG){
	}



	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	//void VisitBinAssign(clang::BinaryOperator *BO);
	void VisitDeclStmt(clang::DeclStmt *stmt);

	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<ConfigDeclAnalyzer*> (this)->Visit(*I);
	}
};

#endif /* CONFIGDECLANALYZER_H_ */
