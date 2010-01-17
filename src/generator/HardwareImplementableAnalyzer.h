/*
 * HardwareImplementableAnalyzer.h
 *
 *  Created on: Jan 11, 2010
 *      Author: Silviu
 */

#ifndef HARDWAREIMPLEMENTABLEANALYZER_H_
#define HARDWAREIMPLEMENTABLEANALYZER_H_


#include <map>
#include <vector>
#include <clang/AST/StmtVisitor.h>

#include "ProcessGraph.h"



class HardwareImplementableAnalyzer : public clang::StmtVisitor<HardwareImplementableAnalyzer> {


	clang::FunctionDecl *FD;
	ProcessGraph *pg;

public:
	HardwareImplementableAnalyzer(clang::FunctionDecl *fd, ProcessGraph *pg);


	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	void VisitCallExpr(clang::CallExpr *CE);


	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<HardwareImplementableAnalyzer*> (this)->Visit(*I);
	}


};


#endif /* HARDWAREIMPLEMENTABLEANALYZER_H_ */
