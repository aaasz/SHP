/*
 * ConfigDeclAnalyzer.cpp
 *
 *  Created on: Dec 28, 2009
 *      Author: Silviu
 */
#include <iostream>
#include "ConfigDeclAnalyzer.h"
#include "ProcessGraph.h"


clang::CallExpr *obtainCallR(clang::Expr *e){
	if (dyn_cast<clang::CallExpr>(e))
		return dyn_cast<clang::CallExpr>(e);
	if (dyn_cast<clang::ImplicitCastExpr>(e)){
		//return obtainDeclR(dyn_cast<clang::ImplicitCastExpr>(e));
		clang::ImplicitCastExpr *ice = dyn_cast<clang::ImplicitCastExpr>(e);
		return obtainCallR(ice->getSubExpr());
	}
	if (dyn_cast<clang::CStyleCastExpr>(e)){
		clang::CStyleCastExpr *cse = dyn_cast<clang::CStyleCastExpr>(e);
		return obtainCallR(cse->getSubExpr());
	}
	return 0;
}

void ConfigDeclAnalyzer::VisitDeclStmt(clang::DeclStmt *stmt){

	clang::DeclGroupRef ref = stmt->getDeclGroup();

	for (clang::DeclGroupRef::iterator it = ref.begin(); it!=ref.end(); ++it){

		if (clang::VarDecl *ND = dyn_cast<clang::VarDecl>(*it)){

			clang::Expr *expr=ND->getInit();
			clang::CallExpr *ce;
			if (expr && (ce=obtainCallR(expr))){
				//obtained call to co_stream_create or co_process_create
				//create node in process graph
				if (ce->getDirectCallee()->getNameAsString().compare("co_stream_create")==0){
					DataStream *str = new DataStream(ND,ce);
					pg->addStream(str);
				}else
				if (ce->getDirectCallee()->getNameAsString().compare("co_process_create")==0){
					ProcessGraphNode *pgn = new ProcessGraphNode(ND,ce);
					pg->addGraphNode(pgn);

				}
			}

		}

	}

}
