/*
 * ProgramAnalyzer.h
 *
 *  Created on: Dec 26, 2009
 *      Author: Silviu
 */

#ifndef PROGRAMANALYZER_H_
#define PROGRAMANALYZER_H_
#include <clang/AST/StmtVisitor.h>
#include <map>
#include "Splitter.h"
#include "ProcessGraph.h"

class ProgramAnalyzer : public clang::StmtVisitor<ProgramAnalyzer> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::Decl *, clang::Decl *> *MAP;
	Splitter *SPLT;
	ProcessGraph *pg;
	clang::SourceManager *SM;
public:

	ProgramAnalyzer(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::Decl *, clang::Decl *> *map, Splitter *splt, clang::SourceManager *sm) :
		FD(fd), OS(os), MAP(map), SPLT(splt), SM(sm){
		pg = new ProcessGraph(SM);
	}



	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}
	ProcessGraph *getProcessGraph(){return pg;}

	//void VisitDeclRefExpr(clang::DeclRefExpr *E);
	void VisitCallExpr(clang::CallExpr *CE);
	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<ProgramAnalyzer*> (this)->Visit(*I);
	}
};

#endif /* PROGRAMANALYZER_H_ */
