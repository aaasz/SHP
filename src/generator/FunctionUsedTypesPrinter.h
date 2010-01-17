/*
 * FunctionUsedTypesPrinter.h
 *
 *  Created on: Dec 1, 2009
 *      Author: Silviu
 */

#ifndef FUNCTIONUSEDTYPESPRINTER_H_
#define FUNCTIONUSEDTYPESPRINTER_H_

#include <clang/Analysis/CallGraph.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/StmtVisitor.h>
#include <map>

class FunctionUsedTypesPrinter : public clang::StmtVisitor<FunctionUsedTypesPrinter> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::TypeDecl *, clang::TypeDecl *> *MAP;

public:
	FunctionUsedTypesPrinter(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::TypeDecl *, clang::TypeDecl *> *map) :
		FD(fd), OS(os), MAP(map){
	}

	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	void VisitDeclStmt(clang::DeclStmt *E);
	//void VisitCallExpr(clang::CallExpr *CE);
	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<FunctionUsedTypesPrinter*> (this)->Visit(*I);
	}
};

#endif /* FUNCTIONUSEDTYPESPRINTER_H_ */
