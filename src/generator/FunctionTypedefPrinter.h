/*
 * FunctionTypedefPrinter.h
 *
 *  Created on: Dec 3, 2009
 *      Author: Silviu
 */

#ifndef FUNCTIONTYPEDEFPRINTER_H_
#define FUNCTIONTYPEDEFPRINTER_H_

#include <clang/Analysis/CallGraph.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/StmtVisitor.h>
#include <map>
#include "Splitter.h"

class FunctionTypedefPrinter : public clang::StmtVisitor<FunctionTypedefPrinter> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::TypeDecl *, clang::TypeDecl *> *MAP;
	Splitter *SPLT;
public:
	FunctionTypedefPrinter(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::TypeDecl *, clang::TypeDecl *> *map, Splitter *splt) :
		FD(fd), OS(os), MAP(map), SPLT(splt){
	}

	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	void VisitDeclStmt(clang::DeclStmt *E);

	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<FunctionTypedefPrinter*> (this)->Visit(*I);
	}
};

#endif /* FUNCTIONTYPEDEFPRINTER_H_ */
