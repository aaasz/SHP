/*
 * ProgramAnalyzer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: Silviu
 */
#include <clang/AST/ASTContext.h>
#include <clang/AST/StmtVisitor.h>
#include <iostream>

#include "ProgramAnalyzer.h"
#include "ConfigAnalyzer.h"
#include "ConfigDeclAnalyzer.h"
#include "ProcessGraph.h"
#include "HardwareImplementableAnalyzer.h"

clang::DeclRefExpr *obtainDeclR(clang::Expr *e){
	if (dyn_cast<clang::DeclRefExpr>(e))
		return dyn_cast<clang::DeclRefExpr>(e);
	if (dyn_cast<clang::ImplicitCastExpr>(e)){
		//return obtainDeclR(dyn_cast<clang::ImplicitCastExpr>(e));
		clang::ImplicitCastExpr *ice = dyn_cast<clang::ImplicitCastExpr>(e);
		return obtainDeclR(ice->getSubExpr());
	}
	if (dyn_cast<clang::CStyleCastExpr>(e)){
		clang::CStyleCastExpr *cse = dyn_cast<clang::CStyleCastExpr>(e);
		return obtainDeclR(cse->getSubExpr());
	}
	return 0;
}
void ProgramAnalyzer::VisitCallExpr(clang::CallExpr *CE){
	clang::FunctionDecl *fd = CE->getDirectCallee();

	//determinte if it is the setup function
	if (fd->getNameAsString().compare("co_architecture_create")!=0)
		return;

	/*called function is co_architecture_create
	the	function pointer is the third argument of the call
	We assume for now that the function pointer is our init function, not some alias.
	In the future, we will solve that case to, just not now.
	*/

	clang::Expr *e = CE->getArg(2);
	clang::DeclRefExpr *dref = obtainDeclR(e);

	if (dref){
		clang::FunctionDecl * conff;

		if (conff = dyn_cast<clang::FunctionDecl>(dref->getDecl())){
			//conf is the pointer to the configuration function that will return the
			//architecture
			//we will build the process graph from this function
			//obtain processes from calls to co_process_create
			std::cout << conff->getNameAsString()<<std::endl;

			ConfigDeclAnalyzer *cfdAnalyzer = new ConfigDeclAnalyzer(conff,OS,MAP,SPLT,pg);
			ConfigAnalyzer *cfAnalyzer = new ConfigAnalyzer(conff,OS,MAP,SPLT,pg);
			HardwareImplementableAnalyzer *hia  = new HardwareImplementableAnalyzer(fd,pg);
			cfdAnalyzer->Visit(conff->getBody());
			cfAnalyzer->Visit(conff->getBody());
			this->pg->establishDirection();
			hia->Visit(conff->getBody());
		}
	}
}
