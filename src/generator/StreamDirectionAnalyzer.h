/*
 * StreamDirectionAnalyzer.h
 *
 *  Created on: Dec 30, 2009
 *      Author: Silviu
 */

#ifndef STREAMDIRECTIONANALYZER_H_
#define STREAMDIRECTIONANALYZER_H_

#include <map>
#include <vector>
#include <clang/AST/StmtVisitor.h>

#include "ProcessGraph.h"

#define unused_stream 0
#define read_stream 1
#define write_stream 2

#define co_r 0
#define co_w 1

class StreamDirectionAnalyzer : public clang::StmtVisitor<StreamDirectionAnalyzer> {


	clang::FunctionDecl *FD;
	std::vector<int> directions;
	std::map<clang::ParmVarDecl *,int> positions;
	clang::SourceManager *SM;

public:
	StreamDirectionAnalyzer(clang::FunctionDecl *fd,clang::SourceManager *sm);


	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	void VisitCallExpr(clang::CallExpr *CE);


	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<StreamDirectionAnalyzer*> (this)->Visit(*I);
	}

	std::vector<int> *getDirections(){return &directions;}
};

#endif /* STREAMDIRECTIONANALYZER_H_ */
