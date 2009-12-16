/*
 * FunctionUsedTypesPrinter.cpp
 *
 *  Created on: Dec 2, 2009
 *      Author: Silviu
 */
#include <clang/AST/Decl.h>

#include "FunctionUsedTypesPrinter.h"

extern void writeTypesStrDS(llvm::raw_fd_ostream &out, clang::RecordDecl *type,
		std::map<clang::TypeDecl *, clang::TypeDecl *> &mset);
extern void writeAT(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<clang::TypeDecl *, clang::TypeDecl *> &mset);

void FunctionUsedTypesPrinter::VisitDeclStmt(clang::DeclStmt *E){

	clang::DeclGroupRef ref = E->getDeclGroup();

	for (clang::DeclGroupRef::iterator it = ref.begin(); it!=ref.end(); ++it){
		//must obtain type from it
		clang::ASTContext &c = (*it)->getASTContext();
		clang::DeclContext *DC = (*it)->getTranslationUnitDecl();

		if (clang::ValueDecl *ND = dyn_cast<clang::ValueDecl>(*it)){
			clang::QualType qt = ND->getType();
			writeAT(*OS,qt,*MAP);
			/*if (qt.getTypePtr()->isStructureType()){
				clang::RecordDecl *decl = qt.getTypePtr()->getAsStructureType()->getDecl();
				if (MAP->find(decl)==MAP->end())
					writeTypesStrDS(*OS,decl,*MAP);
			}
			*/
		}
	}

}
