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
#include <iostream>

#include "UsedDefsPrinter.h"
#include "Splitter.h"
#include "GraphDataAppender.h"
#include "Partitioner.h"

using namespace clang;
using namespace idx;

static llvm::cl::list<std::string>
InputFilenames(llvm::cl::Positional, llvm::cl::desc("<input AST files>"));

int main(int argc, char **argv) {
  llvm::cl::ParseCommandLineOptions(argc-2, argv, "callgraph");

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

  //std::cout<<"ajuns aici\n";
  std::cout<<"start = "<<argv[argc-2]<<"\n";
  std::cout<<"end = "<<argv[argc-1]<<"\n";

  std::vector<ProcessGraph *> pg1;
  for (unsigned i = 0, e = InputFilenames.size(); i != e; ++i) {
		const std::string &InFile = InputFilenames[i];
		Splitter splitter(InFile);

		// std::cout << "begin1\n";
		if (!splitter.hasErrors()) {
		//	 std::cout << "begin2\n";
			splitter.splitFiles(i == 0, i == 0 ? "hw" : "sw");
		//	 std::cout << "begin3\n";
			std::vector<ProcessGraph *> *pg = splitter.getConfigs();
			if (pg->size() != 0) {
				if (pg->size() != 1) {
					std::cout << "Error :(too many)/none config functions\n";
					exit(-1);
				}
				pg1 = *pg;
			}
			/*Partitioner p(ProcessGraph *pg, int populationSize, int maxIter,unsigned int maxLUTS,
			 unsigned int maxSLICES);
			 */

		}
		// std::cout << "end1\n";
	}


	GraphDataAppender gda(pg1[0], "/home/Silviu/workspace/big_test");
	Partitioner p(pg1[0], 400, 400,false,argv[argc-2],argv[argc-1]);
	bool *bestSolution = p.getBestSolution();
	ProcessGraph *pgc = pg1[0];
	std::cout<<pgc->getMaxLUTS()<<" "<<pgc->getMaxSLICES()<<"\n";
	for (int i=0; i<pgc->getNumVertices(); i++){
		ProcessGraphNode *pgn = pgc->lookupGraphNode(i);
		std::cout<<pgn->getName()<<":\n";
		VertexData vd = pgn->getVertexData();
		std::cout << vd.hadrwareExecutionTime << "\n";
		std::cout << vd.communicationTime << "\n";
		std::cout << vd.softwareExecutionTime << "\n";
		std::cout << vd.LUTS << "\n";
		std::cout << vd.SLICES << "\n";
	}

	std::cout<<"Solutia partitionarii:\n";
	for (int i=0; i<pgc->getNumVertices(); i++){
		std::cout<<pgc->lookupGraphNode(i)->getName()<<" :"<<(bestSolution[i] == HARDWARE ? "Hardware" : "Software") << "\n";
	}

}

