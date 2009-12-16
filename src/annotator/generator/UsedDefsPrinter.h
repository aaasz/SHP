#include <clang/AST/ASTContext.h>
#include <clang/AST/StmtVisitor.h>
#include <map>

class UsedDefsPrinter : public clang::StmtVisitor<UsedDefsPrinter> {


	clang::FunctionDecl *FD;
	llvm::raw_fd_ostream *OS;
	std::map<clang::Decl *, clang::Decl *> *MAP;

public:
	UsedDefsPrinter(clang::FunctionDecl *fd, llvm::raw_fd_ostream *os,
			std::map<clang::Decl *, clang::Decl *> *map) :
		FD(fd), OS(os), MAP(map){
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
