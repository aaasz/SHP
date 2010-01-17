#include <clang/AST/ASTContext.h>
#include <clang/AST/StmtVisitor.h>
#include <map>

#include "Splitter.h"

class UsedDefsPrinter : public clang::StmtVisitor<UsedDefsPrinter> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::Decl *, clang::Decl *> *MAP;
	Splitter *SPLT;

public:
	UsedDefsPrinter(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::Decl *, clang::Decl *> *map, Splitter *splt) :
		FD(fd), OS(os), MAP(map), SPLT(splt){
	}

	void VisitStmt(clang::Stmt *S) {
		VisitChildren(S);
	}

	void VisitDeclRefExpr(clang::DeclRefExpr *E);
	//void VisitCallExpr(clang::CallExpr *CE);
	void VisitChildren(clang::Stmt *S) {
		for (clang::Stmt::child_iterator I = S->child_begin(), E = S->child_end(); I
				!= E; ++I)
			if (*I)
				static_cast<UsedDefsPrinter*> (this)->Visit(*I);
	}
};
