/*
 * HardwareImplementableAnalyzer.cpp
 *
 *  Created on: Jan 11, 2010
 *      Author: Silviu
 */

#include "HardwareImplementableAnalyzer.h"
#include <iostream>

extern clang::DeclRefExpr *getDref(clang::Expr *e);

HardwareImplementableAnalyzer::HardwareImplementableAnalyzer(clang::FunctionDecl *fd,ProcessGraph *pg){
	this->FD = fd;
	this->pg = pg;
}

void HardwareImplementableAnalyzer::VisitCallExpr(clang::CallExpr *CE){
	if (CE->getDirectCallee()->getNameAsString().compare("co_process_config")==0){
		clang::DeclRefExpr *dre = getDref(CE->getArg(0));
		clang::VarDecl *vd = dyn_cast<clang::VarDecl>(dre->getDecl());
		if (vd){
			ProcessGraphNode *pgn = pg->lookupNodeByVar(vd);
			pgn->setHardwareImplementable(true);
			std::cout << "setat hardwareImplementable pentru "<<pgn->getName() << std::endl;
		}else{
			std::cerr << "unable to make dyncast\n";
		}
	}
}
