/*
 * UsedDefsPrinter.cpp
 *
 *  Created on: Dec 12, 2009
 *      Author: Silviu
 */

#include "UsedDefsPrinter.h"

void UsedDefsPrinter::VisitDeclRefExpr(clang::DeclRefExpr *E){

	//*OS<<E->getDecl()->isDefinedOutsideFunctionOrMethod()<<" "<< E->getDecl()->getNameAsString()<<"\n";
	clang::NamedDecl *nd = E->getDecl();
	if (nd->isDefinedOutsideFunctionOrMethod() && MAP->find(nd)==MAP->end()){
		(*MAP)[nd]=nd;
		if (clang::VarDecl *vd = dyn_cast<clang::VarDecl>(nd)){
			if (!vd->Extern)
				*OS<<"extern ";
			vd->print(*OS,0);
			*OS<<";\n";
		}
		if (clang::FunctionDecl *fd = dyn_cast<clang::FunctionDecl>(nd)){
			if (!fd->getBody()) {

				fd->print(*OS, 0);
			} else {

				*OS << "extern ";
				*OS << fd->getResultType().getAsString() << " ";
				*OS << fd->getDeclName().getAsString() << "(";

				for (clang::FunctionDecl::param_iterator pi = fd->param_begin(); pi
						!= fd->param_end(); ++pi) {
					if (pi != fd->param_begin())
						*OS << ",";
					(*pi)->print(*OS, 0);

				}
				*OS << ")";
			}
			*OS << ";\n";
		}
	}
}
