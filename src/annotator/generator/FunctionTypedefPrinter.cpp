/*
 * FunctionTypedefPrinter.cpp
 *
 *  Created on: Dec 3, 2009
 *      Author: Silviu
 */

#include <clang/AST/Decl.h>

#include "FunctionTypedefPrinter.h"

extern void writeATdef(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<clang::TypeDecl *, clang::TypeDecl *> &mset);

void FunctionTypedefPrinter::VisitDeclStmt(clang::DeclStmt *E){
	clang::DeclGroupRef ref = E->getDeclGroup();
	*OS <<"-----------------------------------\n";
	for (clang::DeclGroupRef::iterator it = ref.begin(); it!=ref.end(); ++it){
		//must obtain type from iterator
		clang::ASTContext &c = (*it)->getASTContext();
		clang::DeclContext *DC = (*it)->getTranslationUnitDecl();

		if (clang::ValueDecl *ND = dyn_cast<clang::ValueDecl>(*it)){


			clang::QualType qt = ND->getType();
			writeATdef(*OS,qt,*MAP);
		}
	}
	*OS <<"-----------------------------------\n";

}
