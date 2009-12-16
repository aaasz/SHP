#include <clang/AST/ParentMap.h>
#include <clang/Index/CallGraph.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <string>
#include <map>
#include <clang/Frontend/Utils.h>
#include <clang/Basic/SourceLocation.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/Analysis/PathSensitive/AnalysisContext.h>
#include <clang/Analysis/Analyses/LiveVariables.h>
#include <math.h>

#include "UsedDefsPrinter.h"

using namespace clang;
using namespace idx;

static llvm::cl::list<std::string>
InputFilenames(llvm::cl::Positional, llvm::cl::desc("<input AST files>"));

void writeAT(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<TypeDecl *, TypeDecl *> &mset );
void writeATdef(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<TypeDecl *, TypeDecl *> &map );

Type *getNonPtrType(Type *t){
	if (t->isAnyPointerType())
		return getNonPtrType(t->getPointeeType().getTypePtr());
	else
		return t;
}

void writeTypesStrDS(llvm::raw_fd_ostream &out, clang::RecordDecl *type,
		std::map<TypeDecl *, TypeDecl *> &mset) {
	mset[type]=type;
	//run through the component fields of type and print them
	for (RecordDecl::field_iterator it = type->field_begin(); it!=type->field_end(); ++it){
		//TagDecl d;
		QualType t = it->getType();
		writeAT(out,t,mset);
	}

	if (!(type->getTypedefForAnonDecl() == 0)) {
		out << "typedef ";
	}

	type->print(out, 0);
	if (!(type->getTypedefForAnonDecl() == 0))
		out << type->getTypedefForAnonDecl()->getNameAsString() << ";\n";
	else
		out << ";\n";

}

void writeTypesdefStrDS(llvm::raw_fd_ostream &out, clang::RecordDecl *type,
		std::map<TypeDecl *, TypeDecl *> &mset) {
	mset[type]=type;
	//run through the component fields of type and print the required typedefs

	for (RecordDecl::field_iterator it = type->field_begin(); it!=type->field_end(); ++it){
		QualType t = it->getType();
		writeATdef(out,t,mset);
	}
	out<<"apelat " << type->getNameAsString() << "\n";
	type->getTypedefForAnonDecl();
	if (!(type->getTypedefForAnonDecl() == 0)) {
		out<<"apelat2 " << type->getNameAsString() << "\n";

		clang::TypedefDecl *tdecl = type->getTypedefForAnonDecl();
		if (tdecl){
			tdecl->print(out,0);
			out<<";\n";
		}
	}

}

void writeAT(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<TypeDecl *, TypeDecl *> &map ){
	if (type->isStructureType()) {
		const RecordType *rt = type.getTypePtr()->getAsStructureType();
		RecordDecl *decl = rt->getDecl();
		if (map.find(decl) == map.end()) {
			writeTypesStrDS(out, decl, map);
		type->isStructureType();
		}
	} else if (type->isUnionType()) {
		const RecordType *rt = type.getTypePtr()->getAsStructureType();
		RecordDecl *decl = rt->getDecl();
		if (map.find(decl) == map.end()) {
			writeTypesStrDS(out, decl, map);
		}
	}
}
/*
void writeTypes(FunctionDecl *FD, llvm::raw_fd_ostream &out){
	std::map<TypeDecl *,TypeDecl *> map;
	//iterrate over argument types

	for (FunctionDecl::param_iterator it = FD->param_begin(); it!=FD->param_end(); ++it){
		QualType type = (*it)->getType();
		writeAT(out,type,map);

	}
	//obtain declarations within function's body and write types
	FunctionUsedTypesPrinter fudp(FD, &out, &map);
	fudp.Visit(FD->getBody());

}
*/
void writeATdef(llvm::raw_fd_ostream &out, clang::QualType type,
		std::map<TypeDecl *, TypeDecl *> &map ){
	Type *tt = getNonPtrType(type.getLocalUnqualifiedType().getTypePtr());

	if (tt->isStructureType()) {
		out << "intrat in struct type\n";
		const RecordType *rt = tt->getAsStructureType();
		RecordDecl *decl = rt->getDecl();
		if (map.find(decl) == map.end()) {
			writeTypesdefStrDS(out, decl, map);
			//type->isStructureType();
		}
	} else if (tt->isUnionType()) {
		const RecordType *rt = tt->getAsStructureType();
		RecordDecl *decl = rt->getDecl();
		if (map.find(decl) == map.end()) {
			writeTypesdefStrDS(out, decl, map);
		}
	}
/*
	if (!(tt->getTypedefForAnonDecl() == 0)) {
		out << "apelat2 " << tt->getNameAsString() << "\n";

		clang::TypedefDecl *tdecl = type->getTypedefForAnonDecl();
		if (tdecl) {
			tdecl->print(out, 0);
			out << ";\n";
		}
	}
*/
}
/*
void writeTypedefs(FunctionDecl *FD, llvm::raw_fd_ostream &out){
	std::map<TypeDecl *, TypeDecl *> map;
	//iterate over argument types
	for (FunctionDecl::param_iterator it = FD->param_begin(); it
			!= FD->param_end(); ++it) {
		QualType type = (*it)->getType();
		writeATdef(out, type, map);
	}
	FunctionTypedefPrinter fudp(FD, &out, &map);
	fudp.Visit(FD->getBody());

}
*/
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

void printCalledFunctions(FunctionDecl *FD,CallGraph *CG, llvm::raw_fd_ostream &out,
		ASTContext &ast,PrintingPolicy &pp)
{
	std::map<Decl *, Decl *> map;

	CallGraphNode *cgn = CG->getOrInsertFunction(Entity::get(FD, CG->getProgram()));
	for (CallGraphNode::iterator it = cgn->begin(); it!= cgn->end(); ++it) {
		Decl *d = it->second->getDecl(ast);
		if (map.find(d)==map.end()){
			d->print(out, pp, 0);
			out << ";\n";
		}else
			map[d]=d;

	}
}

void printUsedVars(FunctionDecl *FD, llvm::raw_fd_ostream &out){
	std::map<Decl *, Decl *> map;

	UsedDefsPrinter *udp = new UsedDefsPrinter(FD,&out,&map);
	udp->Visit(FD->getBody());
}

int main(int argc, char **argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv, "callgraph");

  FileManager FileMgr;
  std::vector<ASTUnit*> ASTUnits;

  if (InputFilenames.empty())
    return 0;

  TextDiagnosticBuffer DiagClient;

  for (unsigned i = 0, e = InputFilenames.size(); i != e; ++i) {
    const std::string &InFile = InputFilenames[i];

    std::string ErrMsg;
    llvm::OwningPtr<ASTUnit> AST;
    clang::Diagnostic diagnostic;

    AST.reset(ASTUnit::LoadFromPCHFile(InFile,diagnostic,false,true));

    if (!AST) {
      llvm::errs() << "[" << InFile << "] error: " << ErrMsg << '\n';
      return 1;
    }

    ASTUnits.push_back(AST.take());
  }

  llvm::OwningPtr<CallGraph> CG;
  CG.reset(new CallGraph());

  for (unsigned i = 0, e = ASTUnits.size(); i != e; ++i){
    CG->addTU(ASTUnits[i]->getASTContext());

  }

  for (unsigned i = 0, e = ASTUnits.size(); i != e; ++i) {
		ASTContext &ast = ASTUnits[i]->getASTContext();
		clang::PrintingPolicy pp(ast.getLangOptions());
		SourceManager &mgr = ast.getSourceManager();
		DeclContext *DC = ast.getTranslationUnitDecl();
		for (DeclContext::decl_iterator I = DC->decls_begin(), E = DC->decls_end(); I != E; ++I) {

			if (FunctionDecl *FD = dyn_cast<FunctionDecl>(*I)) {
				if (FD->isThisDeclarationADefinition()) {

					std::string fname = FD->getNameAsString();
					fname = fname.append(".c");
					std::string err;
					llvm::raw_fd_ostream out(fname.c_str(), err, 0);

					for (DeclContext::decl_iterator Id = DC->decls_begin(), Ed = DC->decls_end(); Id != Ed; ++Id) {
						if (TypedefDecl *td = dyn_cast<TypedefDecl>(*Id)){

							if (!td->isImplicit()){
								if (td->getLocation().isValid() && td->getLocation().isFileID()){
									PresumedLoc pl = mgr.getPresumedLoc(td->getLocation());
									if (strcmp(pl.getFilename(),"<built-in>")!=0){
										printTypedefDeclaration(td,out,pp);
									}
								}else{
									printTypedefDeclaration(td,out,pp);
								}
							}
						}else if (TagDecl *td = dyn_cast<TagDecl>(*Id)){
							printTagDeclaration(td,out,pp);
						}else if (ValueDecl *vd = dyn_cast<ValueDecl>(*Id)){

							TranslationUnitDecl::lookup_result lr =
									FD->getTranslationUnitDecl()->lookup(vd->getDeclName());

						}

					}//
					//AnalysisContextManager * mgr = new AnalysisContextManager();
					//delete mgr;
					//AnalysisContext analysisContext(FD);
					//LiveVariables *lv = analysisContext.getLiveVariables();
					//lv->dumpBlockLiveness(mgr);
					printUsedVars(FD, out);
					//printCalledFunctions(FD,CG.get(),out,ast,pp);
					//out << lv->getNumDecls() << "\n";
					FD->print(out, pp, 0);
				}
			}
		}
	}
}

