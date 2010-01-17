/*
 * ConfigAnalyzer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: Silviu
 */

#include "ConfigAnalyzer.h"
#include "ProcessGraph.h"
#include <iostream>

void ConfigAnalyzer::VisitBinAssign(clang::BinaryOperator *BO){

	clang::CallExpr *expr = dyn_cast<clang::CallExpr>(BO->getRHS());
	clang::DeclRefExpr *rde = dyn_cast<clang::DeclRefExpr>(BO->getLHS());

	if (expr && expr->getDirectCallee() && rde){
		clang::VarDecl *ND = dyn_cast<clang::VarDecl>(rde->getDecl());

		clang::FunctionDecl *decl= expr->getDirectCallee();
		if (ND && (decl->getNameAsString().compare("co_process_create")==0)){


			ProcessGraphNode *pgn = new ProcessGraphNode(ND, expr);
			pg->addGraphNode(pgn);

		}else if (ND && (decl->getNameAsString().compare("co_stream_create")==0)){

			DataStream *str = new DataStream(ND,expr);
			pg->addStream(str);

		}
	}

}

