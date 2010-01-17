/*
 * Splitter.cpp
 *
 *  Created on: Dec 17, 2009
 *      Author: Silviu
 */
#include <llvm/Support/raw_ostream.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/ASTUnit.h>
#include <map>
#include <vector>
#include <iostream>

#include "UsedDefsPrinter.h"
#include "Splitter.h"
#include "ProgramAnalyzer.h"
using namespace clang;
using namespace idx;

void printTypedefDeclaration(TypedefDecl *td,llvm::raw_fd_ostream &out,PrintingPolicy &pp){

	std::string S = td->getNameAsString();
	td->getUnderlyingType().getAsStringInternal(S,pp);
	if (!(td->getNameAsString()+" "+td->getNameAsString()==S)){
		if (S.length()>=6 && S.substr(0,6).compare("typeof")==0){
			out <<"typedef __typeof__"<<S.substr(6)<<";\n";
		}else{
			out << "typedef " << S << ";\n";
		}
	}
}

void printTagDeclaration(TagDecl *td,llvm::raw_fd_ostream &out,PrintingPolicy &pp){
	int b = (td->getQualifiedNameAsString()=="") && td->isDefinition() ;
	if (b){
		if (RecordDecl *rd= dyn_cast<RecordDecl>(td)){
			if (rd->getTypedefForAnonDecl()){
				out<<"typedef ";
			}
		}else if (EnumDecl *ed= dyn_cast<EnumDecl>(td)){
			if (ed->getTypedefForAnonDecl()){
				out<<"typedef ";
			}
		}
	}
	td->print(out,0);
	if (b){
		if (RecordDecl *rd= dyn_cast<RecordDecl>(td)){
			if (rd->getTypedefForAnonDecl()){
				out << rd->getTypedefForAnonDecl()->getNameAsString();
			}
		}else if (EnumDecl *ed= dyn_cast<EnumDecl>(td)){
			if (ed->getTypedefForAnonDecl()){
				out << ed->getTypedefForAnonDecl()->getNameAsString();
			}
		}
	}
	out<<";\n";
}

void printUsedVars(FunctionDecl *FD, llvm::raw_fd_ostream &out, Splitter *splt){
	std::map<Decl *, Decl *> map;

	UsedDefsPrinter *udp = new UsedDefsPrinter(FD,&out,&map,splt);
	udp->Visit(FD->getBody());
}

void callProgramAnalyzer(FunctionDecl *FD, llvm::raw_fd_ostream &out, Splitter *splt, clang::SourceManager *sm){
	std::map<Decl *, Decl *> map;

	ProgramAnalyzer *udp = new ProgramAnalyzer(FD,&out,&map,splt,sm);

	//std::cout << "begin program analyzer visit\n";

	udp->Visit(FD->getBody());

	//std::cout << "end program analyzer visit\n";

	if (udp->getProcessGraph()->getNumVertices()!=0){
			//configs.push_back(udp->getProcessGraph());
			splt->getConfigs()->push_back(udp->getProcessGraph());
		}else{
			delete udp->getProcessGraph();

		}
}
Splitter::Splitter(const std::string &InFile){
    errors=false;
    this->headers = initHeaders();
	AST.reset(ASTUnit::LoadFromPCHFile(InFile,diagnostic,false,true));

	//std::cout<<"aici2\n";
    if (!AST) {
      llvm::errs() << "[" << InFile << "] error: " << ErrMsg << '\n';
      errors=true;
    //  std::cout<<"aici3\n";
    }
    //std::cout<<"aici4\n";
}

bool Splitter::hasErrors(){
	return errors;
}

Splitter::~Splitter(){

}

void Splitter::splitFiles(bool analize,std::string subdir){
    ASTContext &ast = AST->getASTContext();
	clang::PrintingPolicy pp(ast.getLangOptions());
	SourceManager &mgr = ast.getSourceManager();
	DeclContext *DC = ast.getTranslationUnitDecl();
	for (DeclContext::decl_iterator I = DC->decls_begin(), E = DC->decls_end(); I
			!= E; ++I) {

		if (FunctionDecl *FD = dyn_cast<FunctionDecl>(*I)) {
			if (FD->isThisDeclarationADefinition()) {
								std::string fname = "splitterOutput/"+subdir+"/"+FD->getNameAsString();
				fname = fname.append(".c");
				std::string err;
				llvm::raw_fd_ostream out(fname.c_str(), err, 0);
				//start analyzer
				//std::cout << "begin program analyzer\n";
				if (analize){
					callProgramAnalyzer(FD,out,this,&AST->getSourceManager());

				//end analyzer
					out << this->defaultIncludes;

				}
				//std::cout << "end program analyzer\n";
				for (DeclContext::decl_iterator Id = DC->decls_begin(), Ed =
						DC->decls_end(); Id != Ed; ++Id) {
					if (TypedefDecl *td = dyn_cast<TypedefDecl>(*Id)) {
						if (!td->isImplicit()) {
							//std::cout << "!td->isimplicit\n";
							if (td->getLocation().isValid()
									&& td->getLocation().isFileID()) {

								PresumedLoc pl = mgr.getPresumedLoc(
										td->getLocation());
								//out << pl.getFilename() << "\n";
								if (strcmp(pl.getFilename(), "<built-in>") != 0
										&& this->isCoHDefenined(&pl,&out,td->getNameAsString()).compare("")==0) {
									printTypedefDeclaration(td, out, pp);
								}
							} else {
								PresumedLoc pl = mgr.getPresumedLoc(td->getLocation());
								if (this->isCoHDefenined(&pl,&out,td->getNameAsString()).compare("")==0)
									printTypedefDeclaration(td, out, pp);
							}
						}
					} else if (TagDecl *td = dyn_cast<TagDecl>(*Id)) {
						if (td->getLocation().isValid()
								&& td->getLocation().isFileID()) {
							PresumedLoc pl = mgr.getPresumedLoc(
									td->getLocation());
							if (this->isCoHDefenined(&pl,&out,td->getNameAsString()).compare("")==0) {
								printTagDeclaration(td, out, pp);
							}
						} else{
							PresumedLoc pl = mgr.getPresumedLoc(td->getLocation());
							if (this->isCoHDefenined(&pl,&out,td->getNameAsString()).compare("")==0)
								printTagDeclaration(td, out, pp);
						}
					} else if (ValueDecl *vd = dyn_cast<ValueDecl>(*Id)) {
						/*
						TranslationUnitDecl::lookup_result lr =
								FD->getTranslationUnitDecl()->lookup(
										vd->getDeclName());
						*/
					}

				}
				printUsedVars(FD, out, this);

				FD->print(out, pp, 0);
			}
		}
	}
}


std::vector<std::string> Splitter::initHeaders(){
	std::vector<std::string> hh;
	//co_error.h  co.h  co_if_sim.h  co_import.h  co_math.h  cosim_log.h  co_types.h
	hh.push_back("/co_error.h");
	hh.push_back("/co.h");
	hh.push_back("/co_if_sim.h");
	hh.push_back("/co_import.h");
	hh.push_back("/co_math.h");
	hh.push_back("/cosim_log.h");
	hh.push_back("/co_types.h");
	hh.push_back("\\co_error.h");
	hh.push_back("\\co.h");
	hh.push_back("\\co_if_sim.h");
	hh.push_back("\\co_import.h");
	hh.push_back("\\co_math.h");
	hh.push_back("\\cosim_log.h");
	hh.push_back("\\co_types.h");
	hh.push_back("\\xstatus.h");
	hh.push_back("\\xtmrctr.h");


	defaultIncludes = "#include \"co.h\"\n\
#ifdef IMPULSE_C_TARGET\n#define printf xil_printf\n#include <xparameters.h>\n\
#else\n#include <cosim_log.h>\n#endif\n\
#if defined(XPAR_MICROBLAZE_ID)\n#include <mb_interface.h>\n\
#include <xbasic_types.h>\n#include <xutil.h>\n#include <xtmrctr.h>\n\
#define printf xil_printf\n#endif\n";


	return hh;
}

std::string Splitter::isCoHDefenined(PresumedLoc *pl,llvm::raw_fd_ostream *out,std::string name){
	clang::SourceManager &sm = this->AST->getSourceManager();

	PresumedLoc pl1,pl2,plt;// = sm.getPresumedLoc(pl->getIncludeLoc());
	pl1 = *pl;
	//*out << "ceva ceva\n";
	//std::cout << "called\n";
	do {
	//	std::cout << "pl1 " << pl1.getFilename() <<"\n";
		pl2=sm.getPresumedLoc(pl1.getIncludeLoc());
		plt=pl1;
		pl1=pl2;

	}while (pl1.getIncludeLoc().isValid());
	//std::string s(pl1.getFilename());
	std::string s(plt.getFilename());
	//if (out)
	//	*out << name << ": " << s << "\n";
	//*out << "pl1 " << plt.getFilename() <<" "<<s.substr(s.length()-5,5) <<"\n";

	for (int i=0; i<this->headers.size(); i++){
		std::string head = this->headers[i];
	//	*out << "lal "<<s.substr(s.length()-head.size(),head.size())<<"\n";
		if (s.length()>=head.size()
				&& s.substr(s.length()-head.size(),head.size()).compare(head)==0)
				return head;
	}

	return std::string("");
}

