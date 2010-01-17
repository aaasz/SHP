/*
 * StreamDirectionAnalyzer.cpp
 *
 *  Created on: Dec 30, 2009
 *      Author: Silviu
 */
#include <iostream>
#include "StreamDirectionAnalyzer.h"
StreamDirectionAnalyzer::StreamDirectionAnalyzer(clang::FunctionDecl *fd,clang::SourceManager *sm) :
		FD(fd),SM(sm){
		int pos=0;
		for (clang::FunctionDecl::param_iterator it = fd->param_begin();
				it!=fd->param_end(); ++it){
			clang::QualType tt = (*it)->getType();
			if (tt.getAsString().compare("co_stream")==0){
				directions.push_back(unused_stream);
				clang::ParmVarDecl *pvd = *it;
				positions[pvd]=pos;
				pos++;
				std::cout << pvd <<" "<< pvd->getNameAsString()<<"\n";
				pvd->getLocation().print(llvm::outs(),*sm);

			}

		}

	}

clang::IntegerLiteral *getIntEx(clang::Expr *e){
	if (dyn_cast<clang::IntegerLiteral>(e))
			return dyn_cast<clang::IntegerLiteral>(e);
		if (dyn_cast<clang::ImplicitCastExpr>(e)){
			//return obtainDeclR(dyn_cast<clang::ImplicitCastExpr>(e));
			clang::ImplicitCastExpr *ice = dyn_cast<clang::ImplicitCastExpr>(e);
			return getIntEx(ice->getSubExpr());
		}
		if (dyn_cast<clang::CStyleCastExpr>(e)){
			clang::CStyleCastExpr *cse = dyn_cast<clang::CStyleCastExpr>(e);
			return getIntEx(cse->getSubExpr());
		}
		return 0;
}

clang::DeclRefExpr *getDref(clang::Expr *e){
	if (dyn_cast<clang::DeclRefExpr>(e))
			return dyn_cast<clang::DeclRefExpr>(e);
		if (dyn_cast<clang::ImplicitCastExpr>(e)){
			//return obtainDeclR(dyn_cast<clang::ImplicitCastExpr>(e));
			clang::ImplicitCastExpr *ice = dyn_cast<clang::ImplicitCastExpr>(e);
			return getDref(ice->getSubExpr());
		}
		if (dyn_cast<clang::CStyleCastExpr>(e)){
			clang::CStyleCastExpr *cse = dyn_cast<clang::CStyleCastExpr>(e);
			return getDref(cse->getSubExpr());
		}
		return 0;
}
void StreamDirectionAnalyzer::VisitCallExpr(clang::CallExpr *CE){
	if (CE->getDirectCallee()->getNameAsString().compare("co_stream_open")==0){
		clang::IntegerLiteral *ie = getIntEx(CE->getArg(1));
		clang::DeclRefExpr *dre = getDref(CE->getArg(0));



		if (clang::ParmVarDecl *pvd = dyn_cast<clang::ParmVarDecl>(dre->getDecl())){
			//pvd->getFirstDeclaration();

			if (positions.find(pvd)!=positions.end()) {


				if (*(ie->getValue().getRawData()) == co_r) {
					this->directions[positions[pvd]] = read_stream;
					std::cout << "read_stream " << positions[pvd] << " " << pvd << FD->getNameAsString()
							<< "\n";
				} else if (*(ie->getValue().getRawData()) == co_w) {
					this->directions[positions[pvd]] = write_stream;
					std::cout << "write_stream " << positions[pvd] << " "
							<< pvd << FD->getNameAsString()<<"\n";
				}
			}else{
				CE->getArg(0)->dumpAll();
				std::cout<<pvd->getFirstDeclaration()<<FD->getNameAsString()<<"\n";
				pvd->getLocation().print(llvm::outs(),*SM);
				std::cout<<"\n";
			}
		}
	}
}
